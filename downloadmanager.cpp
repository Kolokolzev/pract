#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) :
 QObject(parent)
{}

void DownloadManager::get(QUrl reqUrl) {
    connect(&manager_, &QNetworkAccessManager::finished,
            this, &DownloadManager::fileDownloaded);

    QNetworkRequest request(reqUrl);
    manager_.get(request);
}

DownloadManager::~DownloadManager() { }

void DownloadManager::fileDownloaded(QNetworkReply* pReply) {
     downloadedData_ = pReply->readAll();
     pReply->deleteLater();
     emit downloaded(downloadedData_);
}

QByteArray DownloadManager::downloadedData() const {
    return downloadedData_;
}
