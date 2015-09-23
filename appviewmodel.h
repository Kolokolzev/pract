#ifndef APPVIEWMODEL
#define APPVIEWMODEL

#include <QUrl>
#include <QDebug>
#include <QProcess>
#include <QApplication>
#include <QDesktopServices>

class ApplicationViewModel : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationViewModel() {}

public slots:
    void downloadDoc(QString docurl);
    void restartApp ();
};

#endif // APPVIEWMODEL
