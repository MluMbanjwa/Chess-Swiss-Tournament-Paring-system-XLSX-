#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QFile file("./styles.qss"); // or "style.qss"
    if (!file.exists()) {
        qDebug() << "QSS file does NOT exist!";
    }

    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Failed to open QSS file!";
    } else {
        qDebug() << "QSS file loaded!";
        QString style = QLatin1String(file.readAll());
        qApp->setStyleSheet(style);
    }
    MainWindow w;
    w.show();
    w.showMaximized();
    return a.exec();
}
