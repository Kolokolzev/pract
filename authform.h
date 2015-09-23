#ifndef AUTHFORM_H
#define AUTHFORM_H

#include <QWidget>
#include <QMessageBox>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

namespace Ui {
    class AuthForm;
}

class AuthForm : public QWidget
{
    Q_OBJECT
public:
    bool isAuthSuccess;
    explicit AuthForm(QWidget *parent = 0);
    ~AuthForm();    
    void auth();

private:
    Ui::AuthForm *ui;
    QString token;

signals:
    void auth_success(QString token);

public slots:
    void url_canged (QUrl);
};

#endif // AUTHFORM_H
