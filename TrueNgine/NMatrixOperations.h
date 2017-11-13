#pragma once

#include <Eigen/Dense>
#include <string.h>
#include <vector>

// This is working
// And by working I mean I tested with an equivalent method in 4D and the same results came up
// TODO: Remake it using Eigen methods
Eigen::VectorXd getNormalVector(Eigen::MatrixXd vectors);

Eigen::MatrixXd translateMatrixN(int N, Eigen::VectorXd point);

// This was based on https://ef.gy/linear-algebra:perspective-projections
// However, he makes this in a different way than every other LookAt I could find
// Therefor it has been silightly adjusted to conform to the others
Eigen::MatrixXd lookAtMatrixN(const int N, Eigen::VectorXd from, Eigen::VectorXd to, Eigen::MatrixXd ups);

Eigen::MatrixXd perspectiveMatrixN(const int N, double eye_radians_angle, double nearPlane, double farPlane, double aspectRatio);

// N > 3
Eigen::MatrixXd viewMatrixN(
	const int N,
	Eigen::VectorXd from,
	Eigen::VectorXd to,
	Eigen::MatrixXd ups,
	double eyeRadiansAngle,
	double nearPlane,
	double farPlane,
	double aspectRatio);

//This assumes that point is an N+1 dimensional vector and that point(N) == 1
Eigen::VectorXd projectPointLosingDimension(Eigen::VectorXd point, Eigen::MatrixXd m);

//This assumes that each point is an N+1 dimensional column in a matrix and that point(N) == 1
Eigen::MatrixXd projectPointsLosingDimension(Eigen::MatrixXd points, Eigen::MatrixXd m, bool usePerspective = true);

Eigen::MatrixXd rotateMatrixN(const unsigned int N, unsigned int axis1, unsigned int axis2, double radians_angle);

//This assumes that each point is an N dimensional column vector in the matrix
//It also assumes that the shape given by the points CAN be reduced
//For example a 3D cube rotated in 4-Space
Eigen::MatrixXd rotateShapeToLowerDimension(Eigen::MatrixXd m);

//This assumes a square matrix
Eigen::MatrixXd makeMatrixHomogeneous(Eigen::MatrixXd matrix);

//This assumes that each point is an N dimensional column vector in the matrix
Eigen::MatrixXd makeVectorsHomogeneous(Eigen::MatrixXd points);

std::string generateNDimensionalShader(int n);