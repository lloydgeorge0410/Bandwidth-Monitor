#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QThread>

class MainThread : public QThread
{
    Q_OBJECT
public:
    explicit MainThread(QObject *parent = 0);
    void run();
};

#endif // MAINTHREAD_H
