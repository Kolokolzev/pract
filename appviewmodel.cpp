#include "appviewmodel.h"

void ApplicationViewModel::downloadDoc(QString docurl) {
    qDebug() << "Start file loading: " << docurl;
    QDesktopServices::openUrl(QUrl(docurl));
}

void ApplicationViewModel::restartApp()
{
    if(QProcess::startDetached(QApplication::applicationFilePath(),
                               QStringList(),
                               QApplication::applicationDirPath()))
    {
        qDebug() << "Restarting application...";
        QApplication::quit();
    }
}

