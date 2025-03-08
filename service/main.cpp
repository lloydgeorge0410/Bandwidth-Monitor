// main.cpp
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <windows.h>
#include "rotatingfilelogger.h"

#define SERVICE_NAME L"MyQtService"

// Global variables
SERVICE_STATUS g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;

QCoreApplication* g_app = nullptr;

// Forward declarations
VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler(DWORD);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);

// Main entry point
int main(int argc, char *argv[])
{
    RotatingFileLogger::init("C:\\logs", "myapp_%d.log", 5 * 1024 * 1024, 10);

    qDebug() << "This is a debug message";
    qInfo() << "This is an info message";
    qWarning() << "This is a warning message";
    qCritical() << "This is a critical error message";

    // If command-line contains "--console", run as a normal application
    for (int i = 1; i < argc; ++i) {
        if (QString(argv[i]) == "--console") {
            QCoreApplication app(argc, argv);
            qDebug() << "Running in console mode";
            // Your service logic here
            return app.exec();
        }
    }

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

    // Create the worker thread
    qDebug() << "Create Thread";
    HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);

    // Set service as running
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

    // Wait for the worker thread to exit
    if (hThread) {
        qDebug() << "Wait Thread";
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }

    CloseHandle(g_ServiceStopEvent);

    // Service has stopped
    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
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

// Service worker thread
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
    // Create a Qt application object
    int argc = 0;
    g_app = new QCoreApplication(argc, nullptr);

    // Your Qt service logic here
    // For example, start a timer for periodic work
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, []() {
        qDebug() << "timer run";
        // Do your periodic work here
        // For example, log something or check a resource
    });
    timer.start(60000); // Every minute

    // Run the Qt event loop
    g_app->exec();

    // Cleanup
    delete g_app;
    g_app = nullptr;

    return ERROR_SUCCESS;
}
