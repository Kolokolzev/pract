#ifndef VKREQUEST_H
#define VKREQUEST_H

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

#include <QUrl>
#include <QString>
#include <QDebug>

#include "downloadmanager.h"
#include "vksession.h"

namespace Vk {

    class Session;

    class Request : public QObject
    {
        Q_OBJECT

    public:
        Request(){}
        virtual ~Request(){}
        Request(Session *vk, QString method);
        static Request *query(Session *vk, QString method);

    private slots:
        void onRequestFinished(QByteArray answer);

    public slots:
        Vk::Request *arg(QString key, QString value);
        Vk::Request *exec();

        void setSession(Vk::Session* session);
        void setMethod(QString method);

    signals:
        void done(QJsonValue response);
        void error(QJsonValue err);
        void sessionChanged(Vk::Session *session);
        void methodChanged(QString method);

    private:
        QUrl query_;
        QString method_;
        DownloadManager downloader_;
        Session *vkSession_;
    };

}
#endif // VKREQUEST_H
