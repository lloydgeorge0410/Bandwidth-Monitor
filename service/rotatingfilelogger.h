// rotatingfilelogger.h
#ifndef ROTATINGFILELOGGER_H
#define ROTATINGFILELOGGER_H

#include <QFile>
#include <QDateTime>
#include <QMutex>
#include <QDir>
#include <QtGlobal>
#include <QLoggingCategory>

class RotatingFileLogger
{
public:
    // Initialize the logger with configuration
    static void init(const QString &logDir = "logs",
                     const QString &fileNamePattern = "app_%d.log",
                     qint64 maxFileSize = 1024 * 1024, // 1MB
                     int maxBackupFiles = 5);

    // Message handler to be installed with qInstallMessageHandler
    static void messageHandler(QtMsgType type,
                               const QMessageLogContext &context,
                               const QString &msg);

private:
    static QFile logFile;
    static QMutex logMutex;
    static QString logFilePattern;
    static QString logDirectory;
    static qint64 maxSize;
    static int maxBackups;

    static void rotateLogFiles();
    static QString formatMessage(QtMsgType type,
                                 const QMessageLogContext &context,
                                 const QString &msg);
};

#endif // ROTATINGFILELOGGER_H
