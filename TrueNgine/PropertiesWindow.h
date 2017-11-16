#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PropertiesWindow.h"

class PropertiesWindow : public QMainWindow
{
	Q_OBJECT

public:
	PropertiesWindow(QWidget *parent = Q_NULLPTR);

	void closeEvent(QCloseEvent *event);

	class CameraNPropertyComponent* firstPropertyComponent;
	class QLineEdit* rotationAmmountEdit;

private:
	Ui::PropertiesWindowClass ui;

	int maxNumberOfCuts = 0;
	int currentNumberOfCuts = 0;
	int numberOfCameras = 0;

	void generateDimensionViewing(class std::vector<int> cutLocations, class std::vector<class CameraND> *cameras, class Camera3D *camera3D, bool allowFaceculling, double maxValue);

private slots:
	void doClose();

	void receiveGenerateDimensionVieweing(class std::vector<int> cutLocations, class std::vector<class CameraND> *cameras, class Camera3D *camera3D, bool allowFaceculling, double maxValue);

	void receiveCameraOriginMovement(int currentCamera, int changedIndex, double newValue);
	void receiveCameraTargetMovement(int currentCamera, int changedIndex, double newValue);

	void receiveCameraChangedPerspective(int currentCamera, bool newValue);

	void receiveChangedFirstThreePositions(double ox, double oy, double oz, double tx, double ty, double tz);

	void changedFaceCulling(int);
	void changedWireframe(int);

	void receiveOpenFile();
	void receiveOpenCutChangeWindow();

	void receiveTestButton();

	void receiveNewMaxNumberOfCuts(int num);
	void receiveCutLocationEditChange(int N, double value);

	void receiveRotationPlaneChange(int index);
	void receiveAddRotation(double ammount);
signals:
	void signalCameraOriginMovement(int currentCamera, int changedIndex, double newValue);
	void signalCameraTargetMovement(int currentCamera, int changedIndex, double newValue);

	void signalCameraChangedPerspective(int currentCamera, bool newValue);

	void signalClose();

	void signalFaceCullingChange(bool newState);
	void signalWireframeChange(bool newState);

	void signalTest();

	void signalOpenFile(QString fileName);
	void signalNewNumberOfCuts(int num);
	void signalCutLocationChange(int cameraIndex, double newValue);

	void signalRotationPlaneChange(int axis1, int axis2);
};
