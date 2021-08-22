#include "mainwindow.h"
#include "common/CryCommon.h"
#include "CalculationsTest/CalculationsTest.h"

#include <CryCore/Platform/platform_impl.inl>
#include <CryThreading/CryThreadImpl.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QGuiApplication app(argc, argv);

	qmlRegisterType<CalculationsTest>("CalcTest.gui", 1, 0, "CalculationsTest");
	qmlRegisterType<CMainWindow>("MainWindow.gui", 1, 0, "MainWindow");
	qmlRegisterRevision<QWindow, 1>("MainWindow.gui", 1, 0);

	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
					 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl) {
			QCoreApplication::exit(-1);
		}
	}, Qt::QueuedConnection);
	engine.load(url);
	if (engine.rootObjects().isEmpty()) {
		qDebug("Empty rootObjects");
		return -1;
	}

	CMainWindow *const mainWindow = qobject_cast<CMainWindow *>(engine.rootObjects().value(0));
	if (!mainWindow) {
		qDebug("Empty mainWindow");
		return -1;
	}

	QObject *const mainGrid = mainWindow->findChild<QObject *>("mainGrid");
	if (!mainGrid) {
		qDebug("Empty mainGrid");
		return -1;
	}

	CalculationsTest *const calculationTest = mainGrid->findChild<CalculationsTest *>("calculationsTest");
	if (!calculationTest) {
		qDebug("Empty calculationTest");
		return -1;
	}

	mainWindow->setCalculationsTest(calculationTest);

	return app.exec();
}
