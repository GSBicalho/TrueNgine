

#include "PropertiesWindow.h"
#include "RenderingWindow.h"
#include <QSurfaceFormat>
#include <QtWidgets/QApplication>

#include "Camera3D.h"
#include "CameraND.h"
#include "CameraNPropertyComponent.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	
	QSurfaceFormat format;
	format.setSamples(16);

	// Creating Windows
	RenderingWindow renderingWindow;
	renderingWindow.setFormat(format);
	renderingWindow.resize(800, 600);
	renderingWindow.show();

	renderingWindow.setAnimating(true);

	PropertiesWindow propertiesWindow;
	propertiesWindow.move(200, 200);
	propertiesWindow.show();

	// Connecting signals betweens windows
	QObject::connect(&propertiesWindow, SIGNAL(signalCameraOriginMovement(int, int, double)),
		&renderingWindow, SLOT(moveCameraOrigin(int, int, double)));

	QObject::connect(&propertiesWindow, SIGNAL(signalCameraTargetMovement(int, int, double)),
		&renderingWindow, SLOT(moveCameraTarget(int, int, double)));

	QObject::connect(&propertiesWindow, SIGNAL(signalCameraChangedPerspective(int, bool)),
		&renderingWindow, SLOT(changeCameraPerspective(int, bool)));
	
	QObject::connect(&renderingWindow, SIGNAL(signal3DLocation(double, double, double, double, double, double)),
		&propertiesWindow, SLOT(receiveChangedFirstThreePositions(double, double, double, double, double, double)));

	QObject::connect(&renderingWindow, SIGNAL(signalGenerateDimensionVieweing(std::vector<int>, std::vector<CameraND> *, Camera3D *, bool, double)),
		&propertiesWindow, SLOT(receiveGenerateDimensionVieweing(std::vector<int>, std::vector<CameraND> *, Camera3D *, bool, double)));

	QObject::connect(&renderingWindow, SIGNAL(signalPossibleNumberOfCuts(int)),
		&propertiesWindow, SLOT(receiveNewMaxNumberOfCuts(int)));

	QObject::connect(&renderingWindow, SIGNAL(signalRotation(double)),
		&propertiesWindow, SLOT(receiveAddRotation(double)));

	QObject::connect(&propertiesWindow, SIGNAL(signalCutLocationChange(int, double)),
		&renderingWindow, SLOT(receiveCutLocationChange(int, double)));

	QObject::connect(&propertiesWindow, SIGNAL(signalNewNumberOfCuts(int)),
		&renderingWindow, SLOT(receiveNewNumberOfCuts(int)));

	QObject::connect(&propertiesWindow, SIGNAL(signalFaceCullingChange(bool)),
		&renderingWindow, SLOT(receiveChangeFaceCulling(bool)));

	QObject::connect(&propertiesWindow, SIGNAL(signalWireframeChange(bool)),
		&renderingWindow, SLOT(receiveChangeWireframe(bool)));

	QObject::connect(&propertiesWindow, SIGNAL(signalOpenFile(QString)),
		&renderingWindow, SLOT(receiveOpenFile(QString)));

	QObject::connect(&propertiesWindow, SIGNAL(signalRotationPlaneChange(int, int)),
		&renderingWindow, SLOT(receiveRotationPlaneChange(int, int)));

	QObject::connect(&propertiesWindow, SIGNAL(signalClose()),
		&renderingWindow, SLOT(doClose()));

	QObject::connect(&renderingWindow, SIGNAL(signalClose()),
		&propertiesWindow, SLOT(doClose()));

	QObject::connect(&propertiesWindow, SIGNAL(signalTest()),
		&renderingWindow, SLOT(receiveTest()));

	return a.exec();
}
