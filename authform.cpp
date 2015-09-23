#include "authform.h"
#include "ui_authform.h"
#include "vksession.h"
#include "settings.h"

AuthForm::AuthForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthForm)
{
    ui->setupUi(this);
    setLayout(ui->gridLayout);

    Settings::Load();
    isAuthSuccess = Vk::Session::authCheck();

    connect(ui->webView, SIGNAL(urlChanged(QUrl)),
            this, SLOT(url_canged(QUrl)));

    setHidden(true);
}

void AuthForm::auth(){
    setHidden(false);
    show();
    if(!isAuthSuccess) {
        ui->webView->load(QUrl("https://oauth.vk.com/authorize?"
                              "client_id=4976727&"
                              "scope=offline,docs&"
                              "redirect_uri=https://oauth.vk.com/blank.html&"
                              "display=popup&"
                              "v=5.34&"
                              "response_type=token"
                              ));
    }
    else {
        emit auth_success(Settings::access_token);
        close();
    }
}

AuthForm::~AuthForm()
{
    delete ui;
}

void AuthForm::url_canged(QUrl url)
{
    if (!isAuthSuccess) {
        setEnabled(true);
        setWindowOpacity(1.0);

        if(!url.toString().contains("access_token"))
        {
            return;
        }

        url = url.toString().replace("#", "?");
        token = QUrlQuery(url).queryItemValue("access_token");

        Settings::access_token = token;
        Settings::Save();
    }
    else
    {
        token = Settings::access_token;
    }

    emit auth_success(token);
    close();
}
