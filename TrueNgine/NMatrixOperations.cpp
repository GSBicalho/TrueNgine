#include "NMatrixOperations.h"

#include <Eigen/Dense>
#include <stdexcept>
#include <assert.h>
#include <string.h>
#include <vector>
#include <QDebug>

#define print(something) {std::stringstream ss; ss << something; qDebug() << ss.str().c_str(); }

// TODO: Remake it using Eigen methods
Eigen::VectorXd getNormalVector(Eigen::MatrixXd vectors) {
	assert(vectors.rows() == vectors.cols() + 1); //Matrix size invalid. Must be (N, N-1)

	Eigen::MatrixXd pV = vectors.transpose();
	const unsigned int d = vectors.rows();

	//std::cout << "Normal -> pV:\n" << pV << "\n";

	Eigen::MatrixXd pM = Eigen::MatrixXd::Zero(d, d);
	Eigen::MatrixXd baseVectors = Eigen::MatrixXd::Zero(d, d);

	for (unsigned int i = 0; i < (d - 1); i++) {
		for (unsigned int j = 0; j < d; j++) {
			pM(i, j) = pV(i, j);
			baseVectors(i, j) = ((i == j) ? 1.f : 0.f);
		}
	}

	for (unsigned int j = 0; j < d; j++) {
		const unsigned int i = d - 1;
		baseVectors(i, j) = ((i == j) ? 1.f : 0.f);
	}

	//std::cout << "Normal -> pM:\n" << pM << "\n";

	Eigen::VectorXd rv = Eigen::VectorXd::Zero(d);

	for (unsigned int i = 0; i < d; i++) {
		Eigen::MatrixXd pS(d - 1, d - 1);

		for (unsigned int j = 0, r = 0; j < (d - 1); j++, r++) {
			for (unsigned int k = 0, c = 0; k < d; k++) {
				if (k == i) {
					continue;
				}

				pS(r, c) = pM(j, k);

				c++;
			}
		}

		if ((i % 2) == 0) {
			rv += baseVectors.row(i) * pS.determinant();
		}
		else {
			rv -= baseVectors.row(i) * pS.determinant();
		}

		//std::cout << "Normal -> pS " << i << ":\n" << pS << "\n";
		//std::cout << "Normal -> pS" << i << " deteminant:\n" << pS.determinant() << "\n";
		//std::cout << "Normal -> rV " << i << ":\n" << rv << "\n";
	}

	//std::cout << "Normal -> Result:\n" << rv << "\n";
	return rv;

	/*
	std::cout << "Getting Normal Vector...\n";

	std::cout << "Normal -> Vectors:\n" << vectors << "\n";

	const int N = vectors.rows();

	//Copy vectors to m and initialize its last column with zeroes
	Eigen::MatrixXd m(N, N);
	m.topLeftCorner(N, N - 1) = vectors;
	m.rightCols(1) = Eigen::VectorXd::Zero(N);
	std::cout << "Normal -> Starting Matrix:\n" << m << "\n";

	//Initialize baseVectors with the Identity matrix
	Eigen::MatrixXd baseVectors = Eigen::MatrixXd::Identity(N, N);

	Eigen::VectorXd returnVector = Eigen::VectorXd::Zero(N);
	for (int i = 0; i < N; i++) {
	Eigen::MatrixXd pS(N - 1, N - 1);

	for (unsigned int j = 0, r = 0; j < (N - 1); j++, r++) {
	for (unsigned int k = 0, c = 0; k < N; k++) {
	if (k == i) {
	continue;
	}

	pS(r, c) = m(j, k);

	c++;
	}
	}

	std::cout << "Normal -> pS" << i << " deteminant:\n" << pS.determinant() << "\n";

	if ((i % 2) == 0) {
	returnVector += baseVectors.col(i) * pS.determinant();
	} else {
	returnVector -= baseVectors.col(i) * pS.determinant();
	}

	//pS.leftCols(i) = m.topLeftCorner(N - 1, i);
	//pS.rightCols(N - i - 1) = m.topRightCorner(N - 1, N - i - 1);
	//returnVector += ((i % 2 ? -1 : 1) * pS.determinant()) * baseVectors.col(i);

	std::cout << "Normal -> pS " << i << ":\n" << pS << "\n";
	std::cout << "Normal -> returnVector " << i << ":\n" << returnVector << "\n";
	}

	std::cout << "Normal -> Result:\n" << returnVector << "\n";
	return returnVector;

	*/
}

Eigen::MatrixXd translateMatrixN(int N, Eigen::VectorXd point) {
	assert(N > 0);

	Eigen::MatrixXd m = Eigen::MatrixXd::Identity(N + 1, N + 1);
	m.topRightCorner(N, 1) = point;
	return m;
}

// This was based on https://ef.gy/linear-algebra:perspective-projections
// However, he makes this in a different way than every other LookAt I could find
// Therefor it has been silightly adjusted to conform to the others
Eigen::MatrixXd lookAtMatrixN(const int N, Eigen::VectorXd from, Eigen::VectorXd to, Eigen::MatrixXd ups) {
	assert(N > 2);

	//print("LookAt Double Calculation:");
	//print("FROM:");
	//print(from.transpose());
	//print("TO:");
	//print(to.transpose());
	//print("UPS");
	//print(ups);

	Eigen::MatrixXd m(N, N);

	m.rightCols(1) = (to - from).normalized();

	int numLoops = 0;
	for (int currentColumn = N - 2; currentColumn > 0; currentColumn--) {
		Eigen::MatrixXd vectorsToCross(N, N - 1);
		int currentColumnOnVectorsToCross = 1;

		//First, cross product all ups, in order
		vectorsToCross.col(0) = ups.col(numLoops);
		for (int i = 1; i < currentColumn; i++) {
			vectorsToCross.col(currentColumnOnVectorsToCross) = ups.col(numLoops + i);
			currentColumnOnVectorsToCross++;
		}

		numLoops++;
		for (int i = 0; i < numLoops; i++) {
			vectorsToCross.col(currentColumnOnVectorsToCross) = m.col(currentColumn + i + 1);
			currentColumnOnVectorsToCross++;
		}

		//print("vectorsToCross\n" << vectorsToCross << "\n");

		auto normal = getNormalVector(vectorsToCross);
		//print("Normal:\n" << normal << "\n");
		//print("Normal Normalized:\n" << normal.normalized() << "\n");

		m.col(currentColumn) = normal.normalized();

		//print("Current Matrix:\n" << m << "\n");
	}

	m.col(0) = getNormalVector(m.rightCols(N - 1)).normalized();

	//print("m");
	//print(m);

	Eigen::MatrixXd temp = Eigen::MatrixXd::Identity(N + 1, N + 1);
	//m.leftCols(N - 1).rowwise().reverseInPlace();
	temp.topLeftCorner(N, N) = m;

	return temp.transpose();
}

Eigen::MatrixXd perspectiveMatrixN(const int N, double eye_radians_angle, double nearPlane, double farPlane, double aspectRatio) {
	assert(N > 2);

	if (N == 3) {
		Eigen::MatrixXd m = Eigen::MatrixXd::Zero(N + 1, N + 1);

		double f_tan = 1 / tan(eye_radians_angle / 2);
		m(0, 0) = f_tan / aspectRatio;
		m(1, 1) = f_tan;
		m(2, 2) = (nearPlane + farPlane) / (nearPlane - farPlane);
		m(2, 3) = -1.f;
		m(3, 2) = 2 * (nearPlane*farPlane) / (nearPlane - farPlane);

		return m.transpose();
	}
	else {
		Eigen::MatrixXd m = Eigen::MatrixXd::Identity(N + 1, N + 1);

		double f_tan = 1 / tan(eye_radians_angle / 2);
		m = m*f_tan;
		m(N - 1, N - 1) = 1;
		m(N, N) = 1;

		return m;
	}
}

// N > 3
Eigen::MatrixXd viewMatrixN(
	const int N,
	Eigen::VectorXd from,
	Eigen::VectorXd to,
	Eigen::MatrixXd ups,
	double eyeRadiansAngle,
	double nearPlane,
	double farPlane,
	double aspectRatio) {

	auto tr = translateMatrixN(N, -from);

	auto la = lookAtMatrixN(N, from, to, ups);

	auto pm = perspectiveMatrixN(N, eyeRadiansAngle, nearPlane, farPlane, aspectRatio);

	Eigen::MatrixXd result = pm * la * tr;
	
	//Axis direction correction
	Eigen::MatrixXd aux = Eigen::MatrixXd::Identity(N + 1, N + 1);
	//X correction
	aux(0, 0) = -1;
	//Z correction
	aux(2, 2) = N != 4 ? -1 : 1;
	result = aux * result;

	//Z correction
	
	return result;
}

//This assumes that point is an N+1 dimensional vector and that point(N) == 1
Eigen::VectorXd projectPointLosingDimension(Eigen::VectorXd point, Eigen::MatrixXd m) {
	//std::cout << "point\n" << point << "\n";

	Eigen::VectorXd pointWith1(point.rows() + 1);
	pointWith1.topRightCorner(point.rows(), 1) = point;
	pointWith1(point.rows()) = 1;

	//std::cout << "pointWith1\n" << pointWith1 << "\n";

	Eigen::VectorXd v = m * pointWith1;
	//std::cout << "v\n" << v << "\n";
	v = v / v(v.rows() - 2, 0);
	//std::cout << "v\n" << v << "\n";
	auto result = v.topLeftCorner(v.rows() - 1, 1);
	//std::cout << "result\n" << result << "\n";

	return result;
}

//This assumes that each point is an N+1 dimensional column in a matrix and that point(N) == 1
Eigen::MatrixXd projectPointsLosingDimension(Eigen::MatrixXd points, Eigen::MatrixXd m, bool usePerspective) {
	//std::cout << "point\n" << points << "\n";

	Eigen::MatrixXd pointWith1(points.rows() + 1, points.cols());
	pointWith1.topRightCorner(points.rows(), points.cols()) = points;
	pointWith1.row(points.rows()) = Eigen::VectorXd::Ones(points.cols());

	//std::cout << "pointWith1\n" << pointWith1 << "\n";

	Eigen::MatrixXd v = m * pointWith1;
	//std::cout << "v\n" << v << "\n";

	if (usePerspective) {
		for (int i = 0; i < v.cols(); i++) {
			if (v.col(i)(v.rows() - 2, 0) == 0 || v.col(i)(v.rows() - 2, 0) == -0) {
				v.col(i) = v.col(i) / (0.000001);
			} else {
				v.col(i) = v.col(i) / v.col(i)(v.rows() - 2, 0);
			}
		}
	}else{
		for (int i = 0; i < v.cols(); i++) {
			if (v.col(i)(v.rows() - 1, 0) == 0 || v.col(i)(v.rows() - 1, 0) == -0) {
				v.col(i) = v.col(i) / (0.000001);
			}
			else {
				v.col(i) = v.col(i) / v.col(i)(v.rows() - 1, 0);
			}
		}
	}

	Eigen::MatrixXd result = v.topRows(v.rows() - 2);

	return result;
}

Eigen::MatrixXd rotateMatrixN(const unsigned int N, unsigned int axis1, unsigned int axis2, double radians_angle) {
	assert(axis1 != axis2);

	Eigen::MatrixXd rot_aa = Eigen::MatrixXd::Identity(N, N);

	rot_aa(axis1, axis1) = cos(radians_angle);
	rot_aa(axis1, axis2) = sin(radians_angle);
	rot_aa(axis2, axis1) = -sin(radians_angle);
	rot_aa(axis2, axis2) = cos(radians_angle);

	return rot_aa.transpose();
}

std::string generateNDimensionalShader(int n) {
	std::stringstream ss;
	ss << "#version 330 core\nlayout (location = 0) in double[" << n << "] position;\n";

	for (int i = n; i >= 3; i--) {
		ss << "uniform double m" << i << "[" << ((i + 1) * (i + 1)) << "];\n";
	}

	ss << "\nvoid main() {\n";

	ss << "	double[" << (n + 1) << "] positionWithOne;\n";
	ss << "	for (int i = 0; i < " << n << "; i++){\n";
	ss << "		positionWithOne[i] = position[i];\n";
	ss << "	}\n";
	ss << "	positionWithOne[" << n << "] = 1;\n\n";

	ss << "	double[" << (n + 1) << "] newPos" << n << ";\n";

	for (int i = n; i >= 3; i--) {
		ss << "	for (int i = 0; i <= " << i << "; i++) {\n";
		ss << "		double newVal = 0;\n";
		ss << "		for (int j = 0; j <= " << i << "; j++){\n";
		ss << "			newVal += m" << i << "[j * " << i << " + i] * positionWithOne[j];\n";
		ss << "		}\n";
		ss << "		newPos" << i << "[i] = newVal;\n";
		ss << "	}\n";
		ss << "	\n";
		if (i != 3) {
			ss << "	for (int i = 0; i < " << i << "; i++) {\n";
			ss << "		positionWithOne[i] = newPos" << i << "[i] / newPos" << i << "[" << (i - 1) << "];\n";
			ss << "	}\n";
			ss << "	\n";
			ss << "	double[" << i << "] newPos" << i - 1 << ";\n";
		}
	}

	ss << "	gl_Position.x = newPos3[0]/newPos3[3];\n";
	ss << "	gl_Position.y = newPos3[1]/newPos3[3];\n";
	ss << "	gl_Position.z = newPos3[2]/newPos3[3];\n";
	ss << "	gl_Position.w = newPos3[3]/newPos3[3];\n";
	ss << "}\n";

	std::string str = ss.str();
	return str;
}

Eigen::MatrixXd makeVectorsHomogeneous(Eigen::MatrixXd points) {
	points.conservativeResize(points.rows() + 1, Eigen::NoChange);
	points.row(points.rows() - 1) = Eigen::VectorXd::Ones(points.cols());

	return points;
}

Eigen::MatrixXd makeMatrixHomogeneous(Eigen::MatrixXd matrix) {
	matrix.conservativeResize(matrix.rows() + 1, matrix.cols() + 1);
	matrix.row(matrix.rows() - 1) = Eigen::VectorXd::Zero(matrix.cols());
	matrix.col(matrix.cols() - 1) = Eigen::VectorXd::Zero(matrix.rows());
	matrix(matrix.rows() - 1, matrix.cols() - 1) = 1;

	return matrix;
}

#define print(something) {std::stringstream ss; ss << something; qDebug() << ss.str().c_str(); }

Eigen::MatrixXd rotateShapeToLowerDimension(Eigen::MatrixXd m) {
	const int D = m.rows();

	// First we move the first vertex of the shape to the origin
	m = translateMatrixN(D, -m.col(0).cast<double>()).cast<double>() * makeVectorsHomogeneous(m);
	m.conservativeResize(m.rows() - 1, Eigen::NoChange);

	//Then, we find a new base created from the points of the polytope
	//And also a vector that is simply the last dimension
	Eigen::MatrixXd newBase = Eigen::MatrixXd::Zero(D, D);
	int currentSizeBase = 1;;
	{
		Eigen::VectorXd aux = Eigen::VectorXd::Zero(D);
		aux(D - 1) = 1;
		newBase.col(0) = aux;

		for (int i = 0; i < m.cols() && currentSizeBase < 2; i++) {
			for (int j = i + 1; j < m.cols() && currentSizeBase < 2; j++) {
				Eigen::VectorXd newVectorOfBase = m.col(j) - m.col(i);
				newVectorOfBase.normalize();
				if (newVectorOfBase(0) < 0) {
					newVectorOfBase = newVectorOfBase * -1;
				}

				if (newVectorOfBase != newBase.col(0)) {
					newBase.col(currentSizeBase) = newVectorOfBase;
					currentSizeBase++;
				}

			}
		}
	}

	for (int i = 0; i < m.cols() && currentSizeBase < D; i++) {
		for (int j = i + 1; j < m.cols() && currentSizeBase < D; j++) {
			Eigen::VectorXd newVectorOfBase = m.col(j) - m.col(i);
			newVectorOfBase.normalize();
			if (newVectorOfBase(0) < 0) {
				newVectorOfBase = newVectorOfBase * -1;
			}

			Eigen::VectorXd x = newBase.colPivHouseholderQr().solve(newVectorOfBase);

			Eigen::VectorXd check = (newBase * x) - newVectorOfBase;
			double error = 0;
			for (int k = 0; k < x.size(); k++) {
				error += check(k) * check(k);
			}

			if (error > 1e-5f) {
				newBase.col(currentSizeBase) = newVectorOfBase;
				currentSizeBase++;
			}
		}
	}
	
	if (currentSizeBase != newBase.cols()) {
		for (int i = 0; i < newBase.rows(); i++) {
			if (newBase.row(i).isZero(1e-10)) {
				m.row(i) = m.row(m.rows() - 1);
				return m.topRows(D - 1);
			}
		}
	} else {
		//We set the coordinate we want to exclude as the last one
		for (int i = 0; i < D - 1; i++) {
			Eigen::VectorXd aux = newBase.col(i);
			newBase.col(i) = newBase.col(i + 1);
			newBase.col(i + 1) = aux;
		}

		//We change the base
		Eigen::MatrixXd transformationMatrix = newBase.inverse();
		Eigen::MatrixXd newVertices = transformationMatrix * m;

		//Return only the top rows, excluding the last one
		return newVertices.topRows(D - 1);
	}

	return Eigen::MatrixXd(0, 0);
}