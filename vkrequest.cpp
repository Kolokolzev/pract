#include "vkrequest.h"

namespace Vk {

    Request::Request(Session* vk, QString method)
    {
        vkSession_ = vk;
        method_ =  method;
        query_ =  QUrl("https://api.vk.com/method/" + method);

        connect(&downloader_, &DownloadManager::downloaded, this, &Request::onRequestFinished);
    }

    Request *Request::query(Session *vk, QString method)
    {
        return new Request(vk, method);
    }

    Request *Request::arg(QString key, QString value)
    {
        QUrlQuery newq(query_);
        newq.removeQueryItem(key);
        newq.addQueryItem(key, value);
        query_.setQuery(newq);

        return this;
    }

    Request *Request::exec()
    {
        arg("v", Session::API_VERSION);
        arg("access_token", vkSession_->token_);

        qDebug() << "vk-api: Start request " << method_;

        downloader_.get(query_);

        return this;
    }

    void Request::onRequestFinished(QByteArray answer)
    {
        QJsonObject response = QJsonDocument::fromJson(answer).object();
        QJsonValue responseData;
        if (response.contains("error")){
            responseData = response.value("error");

            qDebug() << "vk-api: Request filed " << method_ << ":" << responseData.toObject().value("error_msg").toString();

            vkSession_->eventEmitter_.exec(("error:" + method_).toStdString().c_str(),
                                           responseData);
            emit error(responseData);
        }
        else {
            responseData = response.value("response");

            qDebug() << "vk-api: End request " << method_;

            vkSession_->eventEmitter_.exec(("done:" + method_).toStdString().c_str(),
                                           responseData);
            emit done(responseData);
        }
    }

    void Request::setSession(Session *session)
    {
        if (vkSession_ == session)
            return;

        vkSession_ = session;
        emit sessionChanged(session);
    }

    void Request::setMethod(QString method)
    {
        if (method_ == method)
            return;

        method_ = method;
        query_ =  QUrl("https://api.vk.com/method/" + method);
        emit methodChanged(method);
    }

}
