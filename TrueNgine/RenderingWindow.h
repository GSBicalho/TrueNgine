#pragma once

#include "OpenGLWindow.h"
#include <Eigen/Dense>

class RenderingWindow : public OpenGLWindow {
	Q_OBJECT

public:
	RenderingWindow();
	~RenderingWindow();

	using OpenGLWindow::keys;

	void initialize() override;
	void render() override;

	void closeEvent(QEvent *event) override;
	void resizeEvent(QResizeEvent* event);

	void doMovement();

	// Camera Variables
	// The camera used to show the perspective in 3D space
	class Camera3D *camera3D;
	// The cameras used to show the perspective in ND space.
	// The cameras are stored ordered by number of dimensions, from highest to lowest.
	class std::vector<class CameraND> camerasND;
	Eigen::VectorXd cutLocations;
private:
	class QOpenGLShaderProgram *shaderProgram;

	GLuint inColor_uniform;
	GLuint model_uniform;
	GLuint view_uniform;
	GLuint proj_uniform;

	int m_frame;
	qint64 last_frame = 0;
	double delta_time = 0.f;

	int lastX, lastY;
	bool wasActive;

	int rotPlane1 = 0;
	int rotPlane2 = 1;

	//class std::vector<class ObjectND>* objectsToRender;
	class ObjectNDManager* objectManager;

	bool isClosing = false;
	bool isAllowingFaceculling = false;

	void openFile(QString fileName);
	void generateCameras(int numCameras, double xOffset = 2.0f);

public slots:
	void moveCameraTarget(int currentCamera, int changedIndex, double newValue);
	void moveCameraOrigin(int currentCamera, int changedIndex, double newValue);
	void changeCameraPerspective(int currentCamera, bool newValue);

	void doClose();

	void mouseMoveEvent(QMouseEvent *event);

	bool event(QEvent *event) override;

	void receiveChangeFaceCulling(bool newValue);
	void receiveChangeWireframe(bool newValue);

	void receiveOpenFile(QString fileName);
	void receiveNewNumberOfCuts(int numberOfCuts);
	void receiveCutLocationChange(int index, double newValue);

	void receiveRotationPlaneChange(int axis1, int axis2);

	void receiveTest();

signals:
	void signalClose();
	void signal3DLocation(double ox, double oy, double oz, double tx, double ty, double tz);
	void signalGenerateDimensionVieweing(std::vector<int> cutLocations, std::vector<class CameraND> *cameras, class Camera3D *camera3D, bool allowFaceculling);
	void signalPossibleNumberOfCuts(int num);
};
