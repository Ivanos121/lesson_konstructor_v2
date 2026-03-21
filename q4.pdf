#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

// Объявляем глобальные переменные для файла логов и его поток
//QFile logFile("application_log.txt");
QFile logFile;
QTextStream logStream;

// Обработчик сообщений
void customMessageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    if (!logFile.isOpen())
    {
        if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            fprintf(stderr, "Ошибка: не удалось открыть файл логов\n");
            return;
        }
        logStream.setDevice(&logFile);
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString level;

    switch (type)
    {
    case QtDebugMsg:    level = "DEBUG"; break;
    case QtInfoMsg:     level = "INFO"; break;
    case QtWarningMsg:  level = "WARNING"; break;
    case QtCriticalMsg: level = "CRITICAL"; break;
    case QtFatalMsg:    level = "FATAL"; break;
    }

    QString logLine = QString("[%1] %2: %3").arg(timestamp, level, msg);
    logStream << logLine << "\n";
    logStream.flush();

    // Выводим в консоль/в окно вывода Qt Creator
    fprintf(stderr, "%s\n", qPrintable(logLine));
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Формируем имя файла логов с датой и временем
    QString logFileName = QString("application_log_%1.txt")
                              .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss"));

    logFile.setFileName(logFileName);

    // Сбрасываем файл (удаляем старый, если существует, и создаем новый)
    if (logFile.exists()) {
        (void)logFile.remove();
    }

    qInstallMessageHandler(customMessageHandler);

    MainWindow w;
    w.showMaximized();

    qDebug() << "Приложение запущено";

    // Можно добавить дополнительные тестовые сообщения
    qInfo() << "Это информационное сообщение";
    qWarning() << "Это предупреждение";
    qCritical() << "Произошла критическая ошибка";

    return a.exec();
}
