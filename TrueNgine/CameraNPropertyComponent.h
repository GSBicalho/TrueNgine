#pragma once
#include <QWidget>
#include <Eigen/Dense>

class CameraNPropertyComponent : public QWidget
{
	Q_OBJECT

public:
	CameraNPropertyComponent(int N, Eigen::VectorXd *originPosition, Eigen::VectorXd *targetPosition, bool isEditable = true, QWidget *parent = 0);
	~CameraNPropertyComponent();

	class QLineEdit* firstThree[6];

	void changedFirstThreePositions(double ox, double oy, double oz, double tx, double ty, double tz);
public slots:
	void changedEditTarget(const QString &);
	void changedEditOrigin(const QString &);

	void changedCameraPerpective(int newState);

signals:
	void signalCameraOriginMovement(int currentCamera, int changedIndex, double newValue);
	void signalCameraTargetMovement(int currentCamera, int changedIndex, double newValue);

	void signalCameraChangedPerspective(int currentCamera, bool newValue);

private:
	int dimension;
};
