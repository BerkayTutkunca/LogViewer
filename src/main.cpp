#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "application/AppController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    AppController appController;

    engine.rootContext()->setContextProperty(
        QStringLiteral("appController"),
        &appController
        );


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("LogViewer", "Main");

    return QGuiApplication::exec();
}
