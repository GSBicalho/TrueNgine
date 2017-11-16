#pragma once
#include <QWidget>
#include <Eigen/Dense>

class CutNPropertyComponent : public QWidget
{
	Q_OBJECT
public:
	CutNPropertyComponent(int N, double startingValue, double maxValue);
	~CutNPropertyComponent();
private:
	int dimension;
	class QLineEdit* textEdit;
	class QSlider* slider;
public slots:
	void changedEditValue(const QString &);
	void changedSliderValue(int newValue);
signals:
	void signalCutMovement(int dimension, double value);
};

