#include "CutNPropertyComponent.h"
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
#include <QSlider>


CutNPropertyComponent::CutNPropertyComponent(int N, double startingValue, double maxValue) {
	dimension = N;

	QVBoxLayout *outerLayout = new QVBoxLayout();
	this->setLayout(outerLayout);

	{
		std::stringstream ss;
		ss << "Dimension " << N << " Cut";
		QLabel* titleLabel = new QLabel(ss.str().c_str());
		outerLayout->addWidget(titleLabel);
	}

	{
		QHBoxLayout* innerLayout = new QHBoxLayout();
		outerLayout->addLayout(innerLayout);

		QLabel* titleLabel = new QLabel("Location: ");
		innerLayout->addWidget(titleLabel);

		textEdit = new QLineEdit();
		innerLayout->addWidget(textEdit);

		textEdit->setText(QString::number(startingValue));
		textEdit->setValidator(new QDoubleValidator(-1000000, 100000, 10, this));

		QObject::connect(textEdit, SIGNAL(textEdited(const QString &)), this, SLOT(changedEditValue(const QString &)));
	}

	slider = new QSlider(Qt::Horizontal);
	outerLayout->addWidget(slider);

	slider->setRange(-maxValue * 100, maxValue * 100);
	slider->setValue(startingValue);

	QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changedSliderValue(int)));
}

void CutNPropertyComponent::changedEditValue(const QString & newValue) {
	qDebug() << "CHANGED TEXT";

	slider->setValue((int)(newValue.toDouble() * 100));

	emit signalCutMovement(dimension, newValue.toDouble());
}

void CutNPropertyComponent::changedSliderValue(int newValue) {
	qDebug() << "CHANGED SLIDER";
	double aux = newValue;
	aux /= 100.0;

	textEdit->setText(QString::number(aux));

	emit signalCutMovement(dimension, aux);
}

CutNPropertyComponent::~CutNPropertyComponent() {
}
