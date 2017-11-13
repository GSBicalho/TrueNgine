#include "CameraNPropertyComponent.h"
#include <QLayout>
#include <QPushButton>
#include <sstream> 
#include <string> 
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QDoubleValidator>
#include <QCheckBox>

CameraNPropertyComponent::CameraNPropertyComponent(int N, Eigen::VectorXd *originPosition, Eigen::VectorXd *targetPosition, bool isEditable, QWidget *parent)
	: QWidget(parent) {
	static const char cartesianDimensions[] =
		"XYZWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba";

	dimension = N;

	QVBoxLayout *outerLayout = new QVBoxLayout();
	this->setLayout(outerLayout);

	{
		std::stringstream ss;
		ss << "Dimension " << N << " Camera";
		QLabel* titleLabel = new QLabel(ss.str().c_str());
		outerLayout->addWidget(titleLabel);
	}

	if(isEditable){
		QCheckBox* checkbox = new QCheckBox("Perspective");
		checkbox->setChecked(true);
		outerLayout->addWidget(checkbox);

		QObject::connect(checkbox, SIGNAL(stateChanged(int)), (CameraNPropertyComponent*)this, SLOT(changedCameraPerpective(int)));
	}

	{
		QHBoxLayout* innerLayout = new QHBoxLayout();
		outerLayout->addLayout(innerLayout);

		QVBoxLayout* originLayout = new QVBoxLayout();
		QVBoxLayout* targetLayout = new QVBoxLayout();
		innerLayout->addLayout(originLayout);
		innerLayout->addLayout(targetLayout);

		{
			QLabel* originLabel = new QLabel("Eye Point");
			originLayout->addWidget(originLabel);

			QGridLayout* dimTagContainer = new QGridLayout();
			originLayout->addLayout(dimTagContainer);

			dimTagContainer->setColumnStretch(0, 0);
			dimTagContainer->setColumnStretch(1, 1);

			for (int i = 0; i < N; i++) {
				{
					char aux[] = "   ";

					if (i < 52) {
						aux[2] = cartesianDimensions[i];
					}
					else {
						aux[0] = i / 100 + '0';
						aux[1] = i % 100 / 10 + '0';
						aux[2] = i % 10 + '0';
					}

					QLabel* dimLabel = new QLabel(aux);
					dimTagContainer->addWidget(dimLabel);
				}

				QLineEdit* textEdit = new QLineEdit();
				dimTagContainer->addWidget(textEdit);

				if (i < 3) {
					firstThree[i] = textEdit;
				}

				textEdit->setReadOnly(!isEditable);
				if (!isEditable) {
					QPalette palette = textEdit->palette();
					palette.setColor(QPalette::Base, palette.color(QPalette::Button));
					palette.setColor(QPalette::Text, QPalette::WindowText);
					textEdit->setPalette(palette);
				}
				textEdit->setText(QString::number((*originPosition)(i)));
				textEdit->setValidator(new QDoubleValidator(-1000000, 100000, 10, this));
				textEdit->setProperty("my_dimension", i);
				QObject::connect(textEdit, SIGNAL(textEdited(const QString &)), (CameraNPropertyComponent*)this, SLOT(changedEditOrigin(const QString &)));
			}
		}

		{
			QLabel* targetLabel = new QLabel("Target Point");
			targetLayout->addWidget(targetLabel);

			QGridLayout* dimTagContainer = new QGridLayout();
			targetLayout->addLayout(dimTagContainer);

			dimTagContainer->setColumnStretch(0, 0);
			dimTagContainer->setColumnStretch(1, 1);

			for (int i = 0; i < N; i++) {

				char aux[] = "   ";

				if (i < 52) {
					aux[2] = cartesianDimensions[i];
				}
				else {
					aux[0] = (i + 1) / 100 + '0';
					aux[1] = (i + 1) % 100 / 10 + '0';
					aux[2] = (i + 1) % 10 + '0';
				}

				QLabel* dimLabel = new QLabel(aux);
				dimTagContainer->addWidget(dimLabel);

				QLineEdit* textEdit = new QLineEdit();
				dimTagContainer->addWidget(textEdit);

				if (i < 3) { 
					firstThree[i + 3] = textEdit; 
				}

				textEdit->setReadOnly(!isEditable);
				if (!isEditable) {
					QPalette palette = textEdit->palette();
					palette.setColor(QPalette::Base, palette.color(QPalette::Button));
					palette.setColor(QPalette::Text, QPalette::WindowText);
					textEdit->setPalette(palette);
				}
				textEdit->setText(QString::number((*targetPosition)(i)));
				textEdit->setValidator(new QDoubleValidator(-1000000, 100000, 10, this));
				textEdit->setProperty("my_dimension", i);
				QObject::connect(textEdit, SIGNAL(textEdited(const QString &)), (CameraNPropertyComponent*)this, SLOT(changedEditTarget(const QString &)));
			}
		}
	}
}

void CameraNPropertyComponent::changedEditOrigin(const QString & newValue) {
	int dim = QObject::sender()->property("my_dimension").toInt();

	emit signalCameraOriginMovement(dimension, dim, newValue.toFloat());
}

void CameraNPropertyComponent::changedEditTarget(const QString & newValue) {
	int dim = QObject::sender()->property("my_dimension").toInt();

	emit signalCameraTargetMovement(dimension, dim, newValue.toFloat());
}

void CameraNPropertyComponent::changedFirstThreePositions(double ox, double oy, double oz, double tx, double ty, double tz) {
	firstThree[0]->setText(QString::number(ox));
	firstThree[1]->setText(QString::number(oy));
	firstThree[2]->setText(QString::number(oz));

	firstThree[3]->setText(QString::number(tx));
	firstThree[4]->setText(QString::number(ty));
	firstThree[5]->setText(QString::number(tz));
}

void CameraNPropertyComponent::changedCameraPerpective(int newState)  {
	int dim = QObject::sender()->property("my_dimension").toInt();

	emit signalCameraChangedPerspective(dimension, newState == Qt::Checked);
}

CameraNPropertyComponent::~CameraNPropertyComponent() {

}