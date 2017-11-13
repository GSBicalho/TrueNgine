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


CutNPropertyComponent::CutNPropertyComponent(int N, double value) {
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

		QLineEdit* textEdit = new QLineEdit();
		innerLayout->addWidget(textEdit);

		textEdit->setText(QString::number(value));
		textEdit->setValidator(new QDoubleValidator(-1000000, 100000, 10, this));

		QObject::connect(textEdit, SIGNAL(textEdited(const QString &)), this, SLOT(changedEditValue(const QString &)));
	}
}

void CutNPropertyComponent::changedEditValue(const QString & newValue) {
	emit signalCutMovement(dimension, newValue.toFloat());
}

CutNPropertyComponent::~CutNPropertyComponent() {
}
