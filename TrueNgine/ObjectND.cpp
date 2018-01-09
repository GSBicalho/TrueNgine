#include "ObjectND.h"

#include <vector>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QObject>
#include <fstream>

#include <sstream>
#include <QMessageBox>

#include <Eigen/Dense>

#include "NMatrixOperations.h"
#include "RenderingWindow.h"
#include "CameraND.h"
#include "Camera3D.h"

#define print(something) {std::stringstream ss; ss << something; qDebug() << ss.str().c_str(); }

//Each vertex location as a column in the vertices matrix
//The edges between the vertexes as their indexes, as columns in the matrix
ObjectND::ObjectND(unsigned int N, 
		Eigen::MatrixXd &vertices, 
		Eigen::MatrixXi &edges, 
		Eigen::MatrixXi &edgesWithCulling, 
		Eigen::MatrixXi &faces, 
		Eigen::MatrixXi &facesWithCulling, 
		std::vector<CameraND> &camerasND, 
		RenderingWindow* parent, 
		int renderingMode){

	this->N = N;
	this->originalVertices = vertices;

	this->edgesList = edges;
	this->edgesWithCullingList = edgesWithCulling;
	this->facesList = faces;
	this->facesWithCullingList = facesWithCulling;

	this->parent = parent;
	this->requiresUpdate = false;

	updateVertexMatrix(camerasND, false);

	initializeOpenGL();

	this->useCulling = new bool;
	*this->useCulling = false;

	this->renderingMode = new int;
	*this->renderingMode = -1;
	this->setRenderingMode(renderingMode);
}

ObjectND::~ObjectND(){
	VAO->destroy();
	VBO->destroy();
	EBO->destroy();
	delete useCulling;
	delete renderingMode;
}

void ObjectND::initializeOpenGL() {
	//Create VAO, VBO and EBO
	VAO = new QOpenGLVertexArrayObject(parent);
	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	EBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

	VAO->create();
	VBO->create();
	EBO->create();

	VAO->bind();

	VBO->bind();
	VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw); 
	Eigen::MatrixXf f = this->currentVertices.cast<float>();
	VBO->allocate(f.transpose().data(), this->currentVertices.size() * sizeof(float));

	EBO->bind();
	EBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
	EBO->allocate(this->edgesList.data(), this->edgesList.size() * sizeof(unsigned int));

	parent->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	parent->glEnableVertexAttribArray(0);

	VAO->release();
}

void ObjectND::setRenderingMode(int newRenderingMode) {
	if (*this->renderingMode == newRenderingMode) return;
	*this->renderingMode = newRenderingMode;

	updateEBO();
}

int ObjectND::getRenderingMode() {
	return *renderingMode;
}

void ObjectND::setUseCulling(bool newUseCulling) {
	if (*useCulling == newUseCulling) return;
	*useCulling = newUseCulling;

	updateEBO();
}

bool ObjectND::getUseCulling() {
	return *useCulling;
}

void ObjectND::updateEBO() {
	EBO->bind();

	switch (*renderingMode) {
	case OBJECTND_RENDERINGMODE_LINES: {
		if (*useCulling) {
			EBO->allocate(this->edgesWithCullingList.data(), this->edgesWithCullingList.size() * sizeof(unsigned int));
		} else {
			EBO->allocate(this->edgesList.data(), this->edgesList.size() * sizeof(unsigned int));
		}
		break;
	}
	case OBJECTND_RENDERINGMODE_FACES: {
		if (*useCulling) {
			EBO->allocate(this->facesWithCullingList.data(), this->facesWithCullingList.size() * sizeof(unsigned int));
		}
		else {
			EBO->allocate(this->facesList.data(), this->facesList.size() * sizeof(unsigned int));
		}
		break;
	}
	default:
		break;
	}

	EBO->release();
}

void ObjectND::render() {
	//qDebug() << "R " << renderingMode;
	switch (*renderingMode) {
	case OBJECTND_RENDERINGMODE_LINES:
		VAO->bind();
		//glDrawArrays(GL_LINES, 0, currentEdges.size());
		if (*useCulling) {
			glDrawElements(GL_LINES, edgesWithCullingList.size(), GL_UNSIGNED_INT, 0);
		} else {
			glDrawElements(GL_LINES, edgesList.size(), GL_UNSIGNED_INT, 0);
		}
		VAO->release();
		break;
	case OBJECTND_RENDERINGMODE_FACES:
		VAO->bind();
		if (*useCulling) {
			glDrawElements(GL_TRIANGLES, facesWithCullingList.size(), GL_UNSIGNED_INT, 0);
		} else {
			glDrawElements(GL_TRIANGLES, facesList.size(), GL_UNSIGNED_INT, 0);
		}
		VAO->release();
	default:
		break;
	}
}

void ObjectND::updateVertexMatrix(std::vector<CameraND> camerasND, bool doUpdateVBO) {
	currentVertices = originalVertices;

	for (CameraND c : camerasND) {
		currentVertices = projectPointsLosingDimension(currentVertices, c.GetViewProjectionModelMatrix(), c.usePerspective);
	}

	if (doUpdateVBO) {
		updateVBO();
	}

	//print(currentVertices);
}

void ObjectND::updateVBO() {
	VBO->bind();
	Eigen::MatrixXf f = this->currentVertices.cast<float>();
	VBO->allocate(f.transpose().data(), currentVertices.size() * sizeof(float));
	VBO->release();
}