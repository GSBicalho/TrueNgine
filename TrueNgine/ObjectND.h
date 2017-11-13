#pragma once
#include <Eigen/Dense>
#include <vector>

#define OBJECTND_RENDERINGMODE_LINES 0
#define OBJECTND_RENDERINGMODE_FACES 1

class ObjectND {
public:
	unsigned int N;

	Eigen::MatrixXd originalVertices;

	Eigen::MatrixXd currentVertices;
	Eigen::MatrixXi edgesList;
	Eigen::MatrixXi edgesWithCullingList;
	Eigen::MatrixXi facesList;
	Eigen::MatrixXi facesWithCullingList;

	class RenderingWindow* parent;
	
	ObjectND(unsigned int N, Eigen::MatrixXd &vertices, Eigen::MatrixXi &edges, Eigen::MatrixXi &edgesWithCulling, Eigen::MatrixXi &faces, Eigen::MatrixXi &facesWithCulling, std::vector<class CameraND> &camerasND, class RenderingWindow* parent = 0, int renderingMode = OBJECTND_RENDERINGMODE_LINES);
	
	~ObjectND();

	void render();
	void updateVertexMatrix(std::vector<class CameraND> camerasND, bool doUpdateVBO = true);

	void setRenderingMode(int newRenderingMode);
	int getRenderingMode();

	void setUseCulling(bool newUseCulling);
	bool getUseCulling();

	void updateEBO();
private:
	class QOpenGLVertexArrayObject* VAO = 0;
	class QOpenGLBuffer* VBO;
	class QOpenGLBuffer* EBO;

	bool requiresUpdate;
	bool* useCulling;
	int* renderingMode;

	void updateVBO();
	void initializeOpenGL();

};

