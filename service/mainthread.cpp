#include "mainthread.h"
#include "monitorthread.h"

#include <QDebug>
#include <QNetworkInformation>


MainThread::MainThread(QObject *parent) : QThread(parent)
{
}

bool checkInternetConnection() {
    if (!QNetworkInformation::loadDefaultBackend()) {
        return false;
    }
    if (QNetworkInformation::instance()->reachability() == QNetworkInformation::Reachability::Online) {
        return true;
    }
    return false;
}


void MainThread::run()
{
    MonitorThread *thread = NULL;
    qDebug() << "start running";

    while(true)
    {
        if(checkInternetConnection() && (thread == NULL || thread->isFinished()))
        {
            qDebug() << "start monitoring";
            if(thread != NULL)
            {
                delete thread;
                thread = NULL;
            }
            thread = new MonitorThread;
            thread->start();
        }
        else if(!checkInternetConnection() && thread != NULL)
        {
            qDebug() << "stop monitoring";
            thread->terminate();;
            delete thread;
            thread = NULL;
        }
        sleep(10);
    }
}
