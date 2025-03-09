// main.cpp
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QNetworkInformation>
#include <windows.h>
#include "rotatingfilelogger.h"
#include "mainthread.h"
#include "config.h"

#define SERVICE_NAME L"MyQtService"

// Global variables
SERVICE_STATUS g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;

QCoreApplication* g_app = nullptr;

// Forward declarations
VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler(DWORD);

// Main entry point
int main(int argc, char *argv[])
{
    RotatingFileLogger::init("C:\\logs", "myapp_%d.log", 5 * 1024 * 1024, 10);

    // If command-line contains "--console", run as a normal application
    for (int i = 1; i < argc; ++i) {
        if (QString(argv[i]) == "--console") {
            QCoreApplication app(argc, argv);
            qDebug() << "Running in console mode";
            // Your service logic here
            return app.exec();
        }
    }

    g_app = new QCoreApplication(argc, argv);
    qDebug() << g_app->applicationDirPath();
    Config::load();

    // Service entry table
    SERVICE_TABLE_ENTRY ServiceTable[] =
        {
            { SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
            { NULL, NULL }
        };

    // Start service control dispatcher
    if (StartServiceCtrlDispatcher(ServiceTable) == FALSE) {
        return GetLastError();
    }

    return 0;
}

// Service main function
VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
    // Register the service control handler
    g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);
    if (g_StatusHandle == NULL) {
        return;
    }

    // Initialize service status
    ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    // Create the event to signal the service should stop
    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL) {
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        g_ServiceStatus.dwWin32ExitCode = GetLastError();
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
        return;
    }

    // Set service as running
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    qDebug() << "Start Main Thread";
    MainThread *thread = new MainThread();
    thread->start();
    thread->wait();

    g_app->exec();
    delete g_app;

    CloseHandle(g_ServiceStopEvent);

    // Service has stopped
    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    qDebug() << "Main Thread is stopped";
}

// Service control handler
VOID WINAPI ServiceCtrlHandler(DWORD dwCtrl)
{
    switch (dwCtrl) {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
            break;

        g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

        // Signal the service to stop
        SetEvent(g_ServiceStopEvent);
        break;

    default:
        break;
    }
}
