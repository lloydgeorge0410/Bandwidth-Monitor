#include "mainthread.h"
#include "monitorthread.h"

#include <QDebug>
#include <QNetworkInformation>


MainThread::MainThread(QObject *parent) : QThread(parent)
{
}

bool checkInternetConnection() {
    if (!QNetworkInformation::loadDefaultBackend()) {
        qDebug() << "There is no available backend.";
        return false;
    }
    if (QNetworkInformation::instance()->reachability() == QNetworkInformation::Reachability::Online) {
        qDebug() << "Online";
        return true;
    }
    qDebug() << "Offline";
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
            thread->wait();
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
