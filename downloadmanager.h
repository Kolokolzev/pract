#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class DownloadManager : public QObject
{
 Q_OBJECT
 public:
  explicit DownloadManager(QObject *parent = 0);
  virtual ~DownloadManager();
  QByteArray downloadedData() const;

public slots:
  void get(QUrl reqUrl);

signals:
  void downloaded(QByteArray data);

 private slots:
  void fileDownloaded(QNetworkReply* pReply);

 private:
  QNetworkAccessManager manager_;
  QByteArray downloadedData_;
};

#endif // FILEDOWNLOADER_H
