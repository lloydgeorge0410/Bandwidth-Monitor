// rotatingfilelogger.cpp
#include "rotatingfilelogger.h"
#include <QTextStream>
#include <QStandardPaths>
#include <QFileInfo>

QFile RotatingFileLogger::logFile;
QMutex RotatingFileLogger::logMutex;
QString RotatingFileLogger::logFilePattern;
QString RotatingFileLogger::logDirectory;
qint64 RotatingFileLogger::maxSize = 1024 * 1024; // Default 1MB
int RotatingFileLogger::maxBackups = 5;           // Default 5 backups

void RotatingFileLogger::init(const QString &logDir, const QString &fileNamePattern,
                              qint64 maxFileSize, int maxBackupFiles)
{
    QMutexLocker locker(&logMutex);

    // Set configuration values
    logDirectory = logDir;
    logFilePattern = fileNamePattern;
    maxSize = maxFileSize;
    maxBackups = maxBackupFiles;

    // Create log directory if it doesn't exist
    QDir dir;
    if (!dir.exists(logDirectory)) {
        dir.mkpath(logDirectory);
    }

    // Open log file - name with current date
    QString logFileName = QString(logFilePattern).replace("%d",
                                                          QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    logFile.setFileName(logDirectory + QDir::separator() + logFileName);

    // Open in append mode
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        // Failed to open log file
        qWarning() << "Failed to open log file:" << logFile.fileName();
        return;
    }

    // Install the custom message handler
    qInstallMessageHandler(RotatingFileLogger::messageHandler);
}

void RotatingFileLogger::messageHandler(QtMsgType type,
                                        const QMessageLogContext &context,
                                        const QString &msg)
{
    QMutexLocker locker(&logMutex);

    // Check if we need to rotate the log file
    if (logFile.size() > maxSize) {
        rotateLogFiles();
    }

    // Format the message
    QString formattedMessage = formatMessage(type, context, msg);

    // Write to the log file
    QTextStream textStream(&logFile);
    textStream << formattedMessage << "\n";
    textStream.flush();

    // Also output to console in debug mode
#ifdef QT_DEBUG
    fprintf(stderr, "%s\n", qPrintable(formattedMessage));
#endif
}

QString RotatingFileLogger::formatMessage(QtMsgType type,
                                          const QMessageLogContext &context,
                                          const QString &msg)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString levelStr;

    // Set message type
    switch (type) {
    case QtDebugMsg:
        levelStr = "DEBUG";
        break;
    case QtInfoMsg:
        levelStr = "INFO ";
        break;
    case QtWarningMsg:
        levelStr = "WARN ";
        break;
    case QtCriticalMsg:
        levelStr = "ERROR";
        break;
    case QtFatalMsg:
        levelStr = "FATAL";
        break;
    }

    // Format with source info if available
    QString result;
    if (context.file && context.function) {
        result = QString("[%1] [%2] %3 (%4:%5, %6)")
        .arg(timestamp, levelStr, msg,
             QString(context.file),
             QString::number(context.line),
             QString(context.function));
    } else {
        result = QString("[%1] [%2] %3")
        .arg(timestamp, levelStr, msg);
    }

    return result;
}

void RotatingFileLogger::rotateLogFiles()
{
    // Close the current log file
    logFile.close();

    // Get current log file name
    QString currentFileName = logFile.fileName();
    QFileInfo fileInfo(currentFileName);
    QString baseName = fileInfo.baseName();
    QString suffix = fileInfo.completeSuffix();

    // Delete the oldest backup file if it exists
    QString oldestBackupName = QString("%1%2%3.%4.%5")
                                   .arg(logDirectory)
                                   .arg(QDir::separator())
                                   .arg(baseName)
                                   .arg(maxBackups)
                                   .arg(suffix);
    QFile oldestBackup(oldestBackupName);
    if (oldestBackup.exists()) {
        oldestBackup.remove();
    }

    // Rename existing backup files (shift them)
    for (int i = maxBackups - 1; i >= 1; --i) {
        QString oldName = QString("%1%2%3.%4.%5")
        .arg(logDirectory)
            .arg(QDir::separator())
            .arg(baseName)
            .arg(i)
            .arg(suffix);

        QString newName = QString("%1%2%3.%4.%5")
                              .arg(logDirectory)
                              .arg(QDir::separator())
                              .arg(baseName)
                              .arg(i + 1)
                              .arg(suffix);

        QFile backupFile(oldName);
        if (backupFile.exists()) {
            QFile::remove(newName);  // Remove the target file if it exists
            backupFile.rename(newName);
        }
    }

    // Rename the current log file to .1
    QString backupName = QString("%1%2%3.1.%4")
                             .arg(logDirectory)
                             .arg(QDir::separator())
                             .arg(baseName)
                             .arg(suffix);
    QFile::rename(currentFileName, backupName);

    // Create a new log file
    logFile.setFileName(currentFileName);
    logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}
