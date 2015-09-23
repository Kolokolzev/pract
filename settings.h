#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QVariantMap>

class Settings
{
public:
    Settings();

    static QString access_token;

    static void Load(QString file_name = "app.settings");
    static void Save(QString file_name = "app.settings");
};

#endif // SETTINGS_H
