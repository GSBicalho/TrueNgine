#include <vector>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QCloseEvent>
#include <QDateTime>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <iostream>
#include <fstream>
#include <QOpenGLPaintDevice>
#include <QtMath>

#include "NMatrixOperations.h"
#include "Camera3D.h"
#include "CameraND.h"
#include "ObjectND.h"
#include "ObjectNDManager.h"

#include <Eigen/Dense>

#include "RenderingWindow.h"

RenderingWindow::RenderingWindow() : shaderProgram(0), m_frame(0) {
	//setting up 3d camera
	camera3D = new Camera3D(QVector3D(0.0f, 0.0f, 3.0f));
	objectManager = nullptr;
}

RenderingWindow::~RenderingWindow() {}

void RenderingWindow::initialize() {
	glViewport(0, 0, width(), height());
	glEnable(GL_DEPTH_TEST);

	this->setCursor(Qt::BlankCursor);

	lastX = 0;
	lastY = 0;
	wasActive = this->isActive();

	shaderProgram = new QOpenGLShaderProgram(this);
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "simple_color_shader.vs");
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "simple_color_shader.frag");
	shaderProgram->link();

	inColor_uniform = shaderProgram->uniformLocation("inColor");
	model_uniform = shaderProgram->uniformLocation("model");
	view_uniform = shaderProgram->uniformLocation("view");
	proj_uniform = shaderProgram->uniformLocation("projection");

	this->openFile("hypercube.ndp");
	

}

void RenderingWindow::render() {
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Get delta time to check how much to move
	qint64 currentFrame = QDateTime::currentMSecsSinceEpoch();
	delta_time = ((double)(currentFrame - last_frame))/1000.f;
	last_frame = currentFrame;

	doMovement();

	//Set current shader
	//TODO: Get this piece out of here. It probably could be on initialization since it is the only shader we're using (maybe)
	shaderProgram->bind();

	//Set color of wireframe
	QVector4D wireColor = { 1, 0, 0, 1 };
	shaderProgram->setUniformValue(inColor_uniform, wireColor);

	//Get camera transformation
	QMatrix4x4 view, projection, model, asd;
	view = camera3D->GetViewMatrix();

	//For some reason, the commented line below using Qt gives us the wrong matrix
	//projection.perspective(camera3D->Zoom, ((double)width()) / ((double)height()), 0.1f, 1000.f);
	//So we're going to set up our projection matrix by hand
	double one_over_tan_eyeangle = 1 / qTan(camera3D->Zoom / 2);
	double aspect = ((double)width()) / ((double)height());

	projection(0, 0) = one_over_tan_eyeangle / aspect;
	projection(1, 1) = one_over_tan_eyeangle;
	projection(2, 2) = (0.1f + 1000.f) / (0.1f - 1000.f);
	projection(3, 3) = 0;
	projection(2, 3) = 2 * (0.1f * 1000.f) / (0.1f - 1000.f);
	projection(3, 2) = -1;
	
	model.translate(0.f, 0.f, 0.f);

	//Pass matrices to shader
	shaderProgram->setUniformValue(model_uniform, model);
	shaderProgram->setUniformValue(view_uniform, view);
	shaderProgram->setUniformValue(proj_uniform, projection);

	objectManager->render();

	QVector3D v = camera3D->Position + camera3D->Front;
	emit signal3DLocation(camera3D->Position.x(), camera3D->Position.y(), camera3D->Position.z(), v.x(), v.y(), v.z());
}

#define print(something) {std::stringstream ss; ss << something; qDebug() << ss.str().c_str(); }

void RenderingWindow::doMovement() {
	// Camera controls
	//int mask = 1 << 24;
	
	if (keys[OPENGL_WINDOW_W]) {
		camera3D->ProcessKeyboard(FORWARD, delta_time);
	}
	if (keys[OPENGL_WINDOW_S]) {
		camera3D->ProcessKeyboard(BACKWARD, delta_time);
	}
	if (keys[OPENGL_WINDOW_A]) {
		camera3D->ProcessKeyboard(LEFT, delta_time);
	}
	if (keys[OPENGL_WINDOW_D]) {
		camera3D->ProcessKeyboard(RIGHT, delta_time);
	}
	if (keys[OPENGL_WINDOW_SPACE]) {
		camera3D->ProcessKeyboard(UP, delta_time);
	}
	if (keys[OPENGL_WINDOW_SHIFT]) {
		camera3D->ProcessKeyboard(DOWN, delta_time);
	}

	if (keys[OPENGL_WINDOW_Q]) {
		if (objectManager->composingPolytopes.size()) {
			for (int i = 0; i < objectManager->composingPolytopes.at(0)->size(); i++) {
				objectManager->composingPolytopes.at(0)->at(i) = rotateMatrixN(objectManager->numberOfDimensions, rotPlane2, rotPlane1, delta_time) * objectManager->composingPolytopes.at(0)->at(i);
			}
			objectManager->updateObjects(cutLocations, camerasND);
			emit signalRotation(-delta_time);
		}
	}

	if (keys[OPENGL_WINDOW_E]) {
		if (objectManager->composingPolytopes.size()) {
			for (int i = 0; i < objectManager->composingPolytopes.at(0)->size(); i++) {
				objectManager->composingPolytopes.at(0)->at(i) = rotateMatrixN(objectManager->numberOfDimensions, rotPlane1, rotPlane2, delta_time) * objectManager->composingPolytopes.at(0)->at(i);
			}
			objectManager->updateObjects(cutLocations, camerasND);

			emit signalRotation(delta_time);
		}
	}

	if (keys[OPENGL_WINDOW_O]) {
		qDebug() << "CurrentVertexes:";
		objectManager->printStructure();
	}

}

void RenderingWindow::mouseMoveEvent(QMouseEvent *event) {
	if (!this->isActive()) { 
		//If it has just changed away from this window, change the cursor back to arrow
		if (this->wasActive) {
			this->setCursor(Qt::ArrowCursor);
		}

		this->wasActive = false;
		return; 
	}

	//if has just changed into this window, change cursor to blank
	if (!this->wasActive) {
		this->setCursor(Qt::BlankCursor);

		QPoint glob = mapToGlobal(QPoint(width() / 2, height() / 2));
		QCursor::setPos(glob);
		lastX = width() / 2;
		lastY = height() / 2;
		this->wasActive = true;
		return;
	}

	int deltaX = event->pos().x() - lastX;
	int deltaY = lastY - event->pos().y();

	int signalX = deltaX > 0 ? 1 : -1;
	int signalY = deltaY > 0 ? 1 : -1;

	if (!(signalX * deltaX > 500 || signalY * deltaY > 500)) {
		camera3D->ProcessMouseMovement(deltaX, deltaY);
	}
	
	QPoint glob = mapToGlobal(QPoint(width() / 2, height() / 2));
	QCursor::setPos(glob);
	lastX = width() / 2;
	lastY = height() / 2;

	this->wasActive = true;
}

void RenderingWindow::moveCameraOrigin(int currentCamera, int changedIndex, double newValue) {
	//Camera should never be smaller than four, 
	//Because it receives the Dimension of the camera and not its index
	if (currentCamera > 3) {
		currentCamera -= 4;
		camerasND.at(camerasND.size() - currentCamera - 1).Position(changedIndex) = newValue;

		for (ObjectND* obj : (objectManager->currentObjects)) {
			obj->updateVertexMatrix(camerasND);
		}

		qDebug() << "Changing Origin of Dimension" << camerasND.at(camerasND.size() - currentCamera - 1).N;
		qDebug() << "Position " << changedIndex << " = " << newValue;
	}
}

void RenderingWindow::moveCameraTarget(int currentCamera, int changedIndex, double newValue) {
	//Camera should never be smaller than four, 
	//Because it receives the Dimension of the camera and not its index
	if (currentCamera > 3) {
		currentCamera -= 4;
		camerasND.at(camerasND.size() - currentCamera - 1).Target(changedIndex) = newValue;
		for (ObjectND* obj : (objectManager->currentObjects)) {
			obj->updateVertexMatrix(camerasND);
		}

		qDebug() << "Changing Target of Dimension" << camerasND.at(camerasND.size() - currentCamera - 1).N;
		qDebug() << "Position " << changedIndex << " = " << newValue;
	}
}

void RenderingWindow::changeCameraPerspective(int currentCamera, bool newValue) {
	if (currentCamera == 3) {
		//This should never occur
	}
	else {
		currentCamera -= 4;
		camerasND.at(camerasND.size() - currentCamera - 1).usePerspective = newValue;
		for (ObjectND* obj : (objectManager->currentObjects)) {
			obj->updateVertexMatrix(camerasND);
		}

		qDebug() << "Changing Perspective of Dimension" << camerasND.at(camerasND.size() - currentCamera - 1).N;
		qDebug() << "New Value is " << newValue;
	}
}

void RenderingWindow::closeEvent(QEvent *event) {
	if (isClosing) return;
	
	isClosing = true;

	emit signalClose();
}

void RenderingWindow::resizeEvent(QResizeEvent* event) {
	if (m_context) {
		glViewport(0, 0, width(), height());
	}
}

void RenderingWindow::doClose() {
	QWindow::close();
}

bool RenderingWindow::event(QEvent *event) {
	return OpenGLWindow::event(event);
}

void RenderingWindow::receiveTest() {
	for (ObjectND* obj : (objectManager->currentObjects)) {
		if (obj->getRenderingMode() == OBJECTND_RENDERINGMODE_FACES) {
			obj->setRenderingMode(OBJECTND_RENDERINGMODE_LINES);
		} else {
			obj->setRenderingMode(OBJECTND_RENDERINGMODE_FACES);
		}
	}
}

void RenderingWindow::receiveChangeFaceCulling(bool newValue){
	objectManager->setUsingCulling(newValue);
}

void RenderingWindow::receiveChangeWireframe(bool newValue) {
	objectManager->setRenderingMode(newValue ? OBJECTND_RENDERINGMODE_LINES : OBJECTND_RENDERINGMODE_FACES);
}

void RenderingWindow::receiveOpenFile(QString fileName) {
	this->openFile(fileName);
}

bool hasEnding(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

void RenderingWindow::generateCameras(int numCameras, double xOffset) {
	if (objectManager) {
		camerasND.clear();
		for (int i = numCameras; i > 3; i--) {
			//adding a ND camera to our array of cameras
			Eigen::VectorXd position = Eigen::VectorXd::Zero(i);
			position(i - 1) = xOffset;
			Eigen::VectorXd target = Eigen::VectorXd::Zero(i);
			camerasND.push_back(CameraND(i, position, target));
		}
	}
}

void RenderingWindow::receiveNewNumberOfCuts(int numberOfCuts) {
	generateCameras(objectManager->numberOfDimensions - numberOfCuts);
	cutLocations = Eigen::VectorXd::Zero(numberOfCuts);
	objectManager->updateObjects(cutLocations, camerasND);

	std::vector<int> cutLocationsVector;
	for (int i = 0; i < cutLocations.size(); i++) {
		cutLocationsVector.push_back(cutLocations(i));
	}

	qDebug() << "New Number Of Cuts: " << numberOfCuts;

	double maxValue = 0.0;
	for (int i = 0; i < objectManager->composingPolytopes.at(0)->size(); i++) {
		double maxCoeff = objectManager->composingPolytopes.at(0)->at(i).norm();
		if (abs(maxCoeff) > maxValue) {
			maxValue = maxCoeff;
		}
	}

	emit signalGenerateDimensionVieweing(cutLocationsVector, &camerasND, camera3D, true, maxValue);

	
}

void RenderingWindow::receiveCutLocationChange(int index, double newValue) {
	qDebug() << "Change Cut " << index << " to " << newValue;

	cutLocations(index) = newValue;
	objectManager->updateObjects(cutLocations, camerasND);
}

void RenderingWindow::openFile(QString openFile) {
	if (objectManager) {
		delete objectManager;
	}

	rotPlane1 = 0;
	rotPlane2 = 1;

	std::string asStdString = openFile.toLower().toStdString();
	print("Opening " << asStdString);
	std::ifstream inStream(asStdString);
	if (hasEnding(asStdString, ".ndp")) {
		objectManager = new ObjectNDManager(OBJECTNDMANAGER_NDP_FORMAT, inStream, this);
		isAllowingFaceculling = false;
	} else if (hasEnding(asStdString, ".pol")) {
		objectManager = new ObjectNDManager(OBJECTNDMANAGER_POL_FORMAT, inStream, this);
		isAllowingFaceculling = true;
	} else {
		objectManager = new ObjectNDManager(OBJECTNDMANAGER_NO_FORMAT, inStream, this);
		isAllowingFaceculling = false;
	}

	generateCameras(objectManager->numberOfDimensions);
	
	double maxValue = 0.0;

	if (objectManager->composingPolytopes.size()) {
		if (objectManager->composingPolytopes.at(0)->size()) {
			int polyD = objectManager->composingPolytopes.size() - 1;
			int spaceD = objectManager->composingPolytopes.at(0)->at(0).size();
			int maxPossibleCutsByPoly = polyD - 1;
			int numCutsBySpace = spaceD - 3;

			if (maxPossibleCutsByPoly < 0 && numCutsBySpace < 0) {
				emit signalPossibleNumberOfCuts(0);
			}
			else {
				emit signalPossibleNumberOfCuts(maxPossibleCutsByPoly > numCutsBySpace ? numCutsBySpace : maxPossibleCutsByPoly);
			}
		}

		for (int i = 0; i < objectManager->composingPolytopes.at(0)->size(); i++) {
			double maxCoeff = objectManager->composingPolytopes.at(0)->at(i).norm();
			if (abs(maxCoeff) > maxValue) {
				maxValue = maxCoeff;
			}
		}
	} else {
		emit signalPossibleNumberOfCuts(0);
	}

	std::vector<int> cutLocationsVector;
	emit signalGenerateDimensionVieweing(cutLocationsVector, &camerasND, camera3D, true, maxValue);

	Eigen::VectorXd aux(0);
	cutLocations = aux;
	objectManager->updateObjects(cutLocations, camerasND);
}

void RenderingWindow::receiveRotationPlaneChange(int axis1, int axis2) {
	qDebug() << "New Planes of Rotation: " << axis1 << " and " << axis2;
	rotPlane1 = axis1;
	rotPlane2 = axis2;
}

void RenderingWindow::setBackgroundColor(float newRed, float newGreen, float newBlue) {
	backgroundColor[0] = newRed;
	backgroundColor[1] = newGreen;
	backgroundColor[2] = newBlue;
}

void RenderingWindow::receiveChangeBackgroundColor(float newRed, float newGreen, float newBlue) {
	setBackgroundColor(newRed, newGreen, newBlue);

	qDebug() << "New Background Color: " << newRed << ", " << newGreen << ", " << newBlue;
}