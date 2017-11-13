#pragma once

#include <Eigen/Dense>

const double CAMERA_N_EYE_ANGLE = 45.f;

class CameraND {
public:
	unsigned int N;

	//This is relative to the world
	Eigen::VectorXd Position;
	Eigen::VectorXd Target;
	Eigen::MatrixXd Ups; //for everything else

	double EyeAngle;

	bool usePerspective;

public:
	CameraND() {}

	CameraND(
		unsigned int n,
		Eigen::VectorXd position,
		Eigen::VectorXd target,
		bool usePerspective = true,
		double eyeAngle = CAMERA_N_EYE_ANGLE);

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	Eigen::MatrixXd GetViewProjectionModelMatrix();
};