#pragma once
#include <QWidget>
#include <Eigen/Dense>

class CutNPropertyComponent : public QWidget
{
	Q_OBJECT
public:
	CutNPropertyComponent(int N, double value);
	~CutNPropertyComponent();
private:
	int dimension;
public slots:
	void changedEditValue(const QString &);
signals:
	void signalCutMovement(int dimension, double value);
};

