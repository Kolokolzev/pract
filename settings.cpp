#include "settings.h"

QString Settings::access_token = "token not found";

Settings::Settings() {}

void Settings::Load(QString file_name)
{
    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Settings load error!";
        return;
    }

    QVariantMap params;
    QDataStream stream(&file);

    stream >> params;
    file.close();

    if(params.contains("access_token"))
    {
        access_token = params.value("access_token").toString();
    }
}

void Settings::Save(QString file_name)
{
        QFile file(file_name);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            qDebug() << "Settings save error!";
            return;
        }

        QVariantMap params;
        QDataStream stream(&file);

        params.insert("access_token", access_token);

        stream << params;
        file.close();
}

