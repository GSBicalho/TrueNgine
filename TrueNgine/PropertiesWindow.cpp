#include "PropertiesWindow.h"
#include <iostream>
#include <QScrollArea>
#include <QPushButton>
#include <QCheckBox>
#include <QDebug>
#include <QLine>
#include <QWidget>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QComboBox>
#include <QVector>

#include <vector>

#include "CameraNPropertyComponent.h"
#include "CutNPropertyComponent.h"
#include "Camera3D.h"
#include "CameraND.h"

PropertiesWindow::PropertiesWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	// Setting up Menu Bar
	QMenuBar* mainMenuBar = this->findChild<QMenuBar*>("menuBar");
	QMenu* fileMenu = mainMenuBar->findChild<QMenu*>("menuFile");
	connect(fileMenu->actions()[0], SIGNAL(triggered()), this, SLOT(receiveOpenFile()));

	QMenu* optionsMenu = mainMenuBar->findChild<QMenu*>("menuOptions");
	connect(optionsMenu->actions()[0], SIGNAL(triggered()), this, SLOT(receiveOpenCutChangeWindow()));

	//generateDimensionViewing(cameras, camera3D, true);
}

std::string getNameOfDimension(int i){
	static const char cartesianDimensions[] =
		"XYZWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba";

	std::stringstream ss;

	if (i < 52) {
		ss << cartesianDimensions[i];
	} else {
		ss << i;
	}

	return ss.str();
}

void PropertiesWindow::generateDimensionViewing(std::vector<int> cutLocations, std::vector<CameraND> *cameras, Camera3D *camera3D, bool allowFaceculling, double maxValue) {
	currentNumberOfCuts = cutLocations.size();

	// Setting up Dimension viewing
	QWidget* area = (QScrollArea*)this->centralWidget()->findChild<QScrollArea*>("mainScrollArea")->findChild<QWidget*>("scrollAreaWidgetContents");
	
	QList<QWidget *> widgets = area->findChildren<QWidget *>(QString(),Qt::FindDirectChildrenOnly);
	foreach(QWidget * widget, widgets) {
		delete widget;
	}

	Eigen::VectorXd position3d(3);
	Eigen::VectorXd target3d(3);
	for (int i = 0; i < 3; i++) {
		position3d(i) = camera3D->Position[i];
		target3d(i) = camera3D->Position[i] + camera3D->Front[i];
	}

	//QPushButton *testButton = new QPushButton("Do Test!");
	//area->layout()->addWidget(testButton);
	//connect(testButton, SIGNAL(released()), this, SLOT(receiveTestButton()));

	{
		QWidget* w = new QWidget();
		area->layout()->addWidget(w);

		QHBoxLayout* outerLayout = new QHBoxLayout();
		w->setLayout(outerLayout);

		QVBoxLayout* checkboxLayout = new QVBoxLayout();
		outerLayout->addLayout(checkboxLayout);

		QCheckBox* checkboxFaceCulling = new QCheckBox("Face Culling");
		checkboxFaceCulling->setChecked(false);
		checkboxFaceCulling->setEnabled(allowFaceculling);
		checkboxLayout->addWidget(checkboxFaceCulling);
		QObject::connect(checkboxFaceCulling, SIGNAL(stateChanged(int)), this, SLOT(changedFaceCulling(int)));

		QCheckBox* checkboxWireframe = new QCheckBox("Wireframe");
		checkboxWireframe->setChecked(true);
		checkboxLayout->addWidget(checkboxWireframe);
		QObject::connect(checkboxWireframe, SIGNAL(stateChanged(int)), this, SLOT(changedWireframe(int)));

		QVBoxLayout* rotationLayout = new QVBoxLayout();
		outerLayout->addLayout(rotationLayout);

		QHBoxLayout* rotationPickerLayout = new QHBoxLayout();
		rotationLayout->addLayout(rotationPickerLayout);

		QLabel* labelRot = new QLabel("Rotation Plane:");
		rotationPickerLayout->addWidget(labelRot);

		QComboBox* cbRot = new QComboBox();
		rotationPickerLayout->addWidget(cbRot);
		connect(cbRot, SIGNAL(activated(int)), this, SLOT(receiveRotationPlaneChange(int)));

		int numberOfDimensions = cutLocations.size() + cameras->size() + 3;

		for (int i = 0; i < numberOfDimensions; i++) {
			std::string nameDim1 = getNameOfDimension(i);
			for (int j = i + 1; j < numberOfDimensions; j++) {
				std::string nameDim2 = getNameOfDimension(j);
				QString str = QString((nameDim1 + "-" + nameDim2).c_str());

				QVector<int> axis;
				axis.append(i);
				axis.append(j);

				cbRot->addItem(str, QVariant::fromValue(axis));
			}
		}

		QHBoxLayout* rotationObserverLayout = new QHBoxLayout();
		rotationLayout->addLayout(rotationObserverLayout);

		QLabel* labelDegrees = new QLabel("Degrees Rotated:");
		rotationObserverLayout->addWidget(labelDegrees);

		QLineEdit* lineEd = new QLineEdit("0.0");
		rotationObserverLayout->addWidget(lineEd);
		lineEd->setReadOnly(true);
		
		QPalette palette = lineEd->palette();
		palette.setColor(QPalette::Base, palette.color(QPalette::Button));
		palette.setColor(QPalette::Text, QPalette::WindowText);
		lineEd->setPalette(palette);

		rotationAmmountEdit = lineEd;
	}

	QWidget* horizontalLineWidget = new QWidget;
	horizontalLineWidget->setFixedHeight(2);
	horizontalLineWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	horizontalLineWidget->setStyleSheet(QString("background-color: #c0c0c0;"));

	area->layout()->addWidget(horizontalLineWidget);

	{
		firstPropertyComponent = new CameraNPropertyComponent(3, &position3d, &target3d, false);
		area->layout()->addWidget(firstPropertyComponent);
	}

	numberOfCameras = cameras->size();

	for (int i = 0; i < cameras->size(); i++) {
		QWidget *horizontalLineWidget = new QWidget;
		horizontalLineWidget->setFixedHeight(2);
		horizontalLineWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		horizontalLineWidget->setStyleSheet(QString("background-color: #c0c0c0;"));

		area->layout()->addWidget(horizontalLineWidget);

		CameraNPropertyComponent* propertiesOfDimension = new CameraNPropertyComponent(i + 4, &cameras->at(cameras->size() - i - 1).Position, &cameras->at(cameras->size() - i - 1).Target);
		area->layout()->addWidget(propertiesOfDimension);

		QObject::connect(propertiesOfDimension, SIGNAL(signalCameraOriginMovement(int, int, double)), this, SLOT(receiveCameraOriginMovement(int, int, double)));
		QObject::connect(propertiesOfDimension, SIGNAL(signalCameraTargetMovement(int, int, double)), this, SLOT(receiveCameraTargetMovement(int, int, double)));
		QObject::connect(propertiesOfDimension, SIGNAL(signalCameraChangedPerspective(int, bool)), this, SLOT(receiveCameraChangedPerspective(int, bool)));
	}

	for (int i = 0; i < cutLocations.size(); i++) {
		QWidget *horizontalLineWidget = new QWidget;
		horizontalLineWidget->setFixedHeight(2);
		horizontalLineWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		horizontalLineWidget->setStyleSheet(QString("background-color: #c0c0c0;"));

		area->layout()->addWidget(horizontalLineWidget);

		CutNPropertyComponent* propertiesOfCut = new CutNPropertyComponent(cameras->size() + i + 4, 0.f, maxValue + 0.1);
		area->layout()->addWidget(propertiesOfCut);

		QObject::connect(propertiesOfCut, SIGNAL(signalCutMovement(int, double)), this, SLOT(receiveCutLocationEditChange(int, double)));
	}

	emit signalRotationPlaneChange(0, 1);
}

void PropertiesWindow::receiveRotationPlaneChange(int index) {
	QVector<int> planes = ((QComboBox*)QObject::sender())->itemData(index).value<QVector<int> >();
	emit signalRotationPlaneChange(planes.at(0), planes.at(1));
	rotationAmmountEdit->setText("0.0");
}

void PropertiesWindow::receiveAddRotation(double ammount) {
	double angle = rotationAmmountEdit->text().toDouble() + qRadiansToDegrees(ammount);
	if (angle > 360.f) {
		angle -= 360.f;
	} else if (angle < 0.f) {
		angle += 360.f;
	}

	rotationAmmountEdit->setText(QString::number(angle));
}

void PropertiesWindow::receiveCutLocationEditChange(int N, double value) {
	emit signalCutLocationChange(N - numberOfCameras - 4, value);
}

void PropertiesWindow::receiveTestButton() {
	emit signalTest();
}

void PropertiesWindow::receiveGenerateDimensionVieweing(std::vector<int> cutLocations, std::vector<CameraND> *cameras, Camera3D *camera3D, bool allowFaceculling, double maxValue) {
	generateDimensionViewing(cutLocations, cameras, camera3D, allowFaceculling, maxValue);
}

void PropertiesWindow::closeEvent(QCloseEvent *event) {
	emit signalClose();
}

void PropertiesWindow::doClose() {
	close();
}

void PropertiesWindow::changedFaceCulling(int newState) {
	emit signalFaceCullingChange(newState == Qt::Checked);
}

void PropertiesWindow::changedWireframe(int newState) {
	emit signalWireframeChange(newState == Qt::Checked);
}

void PropertiesWindow::receiveCameraOriginMovement(int currentCamera, int changedIndex, double newValue){
	emit signalCameraOriginMovement(currentCamera, changedIndex, newValue);
}

void PropertiesWindow::receiveCameraTargetMovement(int currentCamera, int changedIndex, double newValue) {
	emit signalCameraTargetMovement(currentCamera, changedIndex, newValue);
}

void PropertiesWindow::receiveCameraChangedPerspective(int currentCamera, bool newValue) {
	emit signalCameraChangedPerspective(currentCamera, newValue);
}

void PropertiesWindow::receiveOpenFile() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Polytope"), "",
		tr("All Files (*);;Function Approximation (*.pol);;Polytope Descriptor (*.ndp)"));

	if (fileName.isEmpty())
		return;

	emit signalOpenFile(fileName);
}

void PropertiesWindow::receiveNewMaxNumberOfCuts(int num) {
	maxNumberOfCuts = num;
}

void PropertiesWindow::receiveOpenCutChangeWindow() {

	bool ok = false;

	std::stringstream ss;
	ss << "Type in the number of Dimensions where Cuts will be applied (max " << maxNumberOfCuts << "):";

	int numberOfCuts = QInputDialog::getInt(this, tr("New Number of Cuts"), tr(ss.str().c_str()), 0, 0, maxNumberOfCuts, 1, &ok);

	if (ok && numberOfCuts != currentNumberOfCuts) {
		currentNumberOfCuts = numberOfCuts;
		emit signalNewNumberOfCuts(numberOfCuts);
	}
}

void PropertiesWindow::receiveChangedFirstThreePositions(double ox, double oy, double oz, double tx, double ty, double tz) {
	if (firstPropertyComponent) {
		firstPropertyComponent->changedFirstThreePositions(ox, oy, oz, tx, ty, tz);
	}
}