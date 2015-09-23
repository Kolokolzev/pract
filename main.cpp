#include <QQmlContext>
#include <QApplication>
#include <QQmlApplicationEngine>

#include "vk.h"
#include "appviewmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Vk::RegisterQmlTypes();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QQmlContext* context = engine.rootContext();
    context->setContextProperty("viewModel", new ApplicationViewModel());

    return app.exec();
}
