#include "CameraND.h"

#include <assert.h>
#include "NMatrixOperations.h"
#include <limits>

#include <QDebug>

Eigen::MatrixXd CameraND::GetViewProjectionModelMatrix() {

	return viewMatrixN(N, this->Position, this->Target, this->Ups, EyeAngle, 0, 0, 0);

}

CameraND::CameraND(
	unsigned int n,
	Eigen::VectorXd position,
	Eigen::VectorXd target,
	bool usePerspective,
	double eyeAngle) {
	assert(position.rows() == n);
	assert(n >= 4);

	this->N = n;

	this->Position = position;
	this->Target = target;
	this->EyeAngle = eyeAngle;

	this->Ups = Eigen::MatrixXd::Zero(N, N - 2);
	//This creates the Up vectors in an order that will not rotate the axes
	this->Ups.block(1, 0, N - 3, N - 3) = Eigen::MatrixXd::Identity(N - 3, N - 3).rowwise().reverse();
	this->Ups(N - 1, N - 3) = 1.f;

	//This is added so the user is able to look from the Up axes
	//It is a minimal perturbation, which does not affect the rendering
	for (int i = 0; i < this->Ups.cols(); i++) {
		this->Ups(0, i) += 1.e-20f;
	}

	this->usePerspective = usePerspective;
}