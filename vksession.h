#ifndef VKSESSION_H
#define VKSESSION_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

#include "eventemitter.h"
#include "authform.h"
#include "vkrequest.h"

namespace Vk {

    class Request;

    class Session : public QObject
    {
        Q_OBJECT

    public:
        friend class Request;

        typedef EventEmitter<const char*, QJsonValue> VkEventEmitter;

        Session();
        virtual ~Session();
        static const QString API_VERSION;

        VkEventEmitter::ListenerId on(VkEventEmitter::EventId event_id, VkEventEmitter::Handler cb);
        void off(VkEventEmitter::ListenerId listener_id);

    public slots:
        static bool authCheck();
        void login();
        void logout();
        Vk::Request *request(QString method);

    signals:
        void authSuccess();
        void authLogout();

    private:
        QString token_;
        AuthForm authForm_;
        VkEventEmitter eventEmitter_;
    };
}
#endif // VKSESSION_H
