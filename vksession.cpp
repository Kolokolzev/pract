#include "vksession.h"
#include "settings.h"

namespace Vk {

    const QString Session::API_VERSION = "5.34";

    Session::Session()
    {
        qDebug() << "vk-api: Constructor";
    }

    Session::~Session()
    {
        qDebug() << "vk-api: Destructor";
    }

    void Session::login()
    {
        qDebug() << "vk-api: Login";

        QObject::connect(&authForm_, &AuthForm::auth_success, [&](QString newToken){
            qDebug() << "vk-api: Auth successed";
            token_ = newToken;
            eventEmitter_.exec("login", QJsonValue());
            emit authSuccess();
        });

        authForm_.auth();
    }

    void Session::logout()
    {
        qDebug() << "vk-api: Logout";
        Settings::access_token = "token not found";
        Settings::Save();
        token_ = "";
        eventEmitter_.exec("logout", QJsonValue());
        emit authLogout();
    }

    bool Session::authCheck()
    {
        qDebug() << "vk-api: Auth check";

        QUrl query("https://api.vk.com/method/execute.authCheck");
        QUrlQuery newq(query);
        newq.addQueryItem("v", Session::API_VERSION);
        newq.addQueryItem("access_token", Settings::access_token);
        query.setQuery(newq);

        QNetworkAccessManager *manager = new QNetworkAccessManager();
        QNetworkReply* reply = manager->get(QNetworkRequest(query));
        QEventLoop wait;
        QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &wait, SLOT(quit()));
        QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), manager, SLOT(deleteLater()));
        QTimer::singleShot(10000, &wait, SLOT(quit()));
        wait.exec();

        QByteArray answer = reply->readAll();
        reply->deleteLater();

        QJsonObject resp = QJsonDocument::fromJson(answer).object();

        if (resp.contains("error")){
            qDebug() << "vk-api: Auth check failed";
            return false;
        }
        else {
            qDebug() << "vk-api: Auth check successed";
            return resp.toVariantMap().value("response").toBool();
        }
    }

    Session::VkEventEmitter::ListenerId Session::on(VkEventEmitter::EventId event_id, VkEventEmitter::Handler cb)
    {
       return eventEmitter_.on(event_id, cb);
    }

    void Session::off(VkEventEmitter::ListenerId listener_id)
    {
        eventEmitter_.off(listener_id);
    }

    Request *Session::request(QString method)
    {
        return Request::query(this, method);
    }

}
