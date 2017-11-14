#pragma once
#include "ObjectND.h"
#include <map>

#define OBJECTNDMANAGER_POL_FORMAT 0
#define OBJECTNDMANAGER_NDP_FORMAT 1

class ObjectNDManager {
public:
	ObjectNDManager(int typeExtension, std::ifstream &inStream, class RenderingWindow* parent = 0);
	~ObjectNDManager();

	unsigned int numberOfDimensions;

	class RenderingWindow* parent;

	std::vector<std::vector<Eigen::VectorXd>*> composingPolytopes;

	std::vector<std::vector<Eigen::VectorXd>*>* currentComposingPolytopes;
	std::vector<class ObjectND*> currentObjects;

	void updateObjects(Eigen::VectorXd &cuts_location, std::vector<class CameraND> camerasND);
	void render();
	void setRenderingMode(int value);
	void setUsingCulling(bool value);

	void printStructure();

private:
	int renderingMode;
	bool useCulling;
	bool hasCalculatedCulling;

	void readFromPOLFormatStream(std::ifstream &inStream);
	void readFromNDPFormatStream(std::ifstream &inStream);
	void checkSharedEdgesAndFaces(std::vector<std::vector<Eigen::VectorXd>*>* currentComposingPolytopes, bool* edgeIsShared, bool* faceIsShared);
	void generateEdgeAndFaceLists(
		std::vector<std::vector<Eigen::VectorXd>*>* currentComposingPolytopes,
		Eigen::MatrixXi &edgeListWithoutCulling,
		Eigen::MatrixXi &edgeListWithCulling,
		Eigen::MatrixXi &faceListWithoutCulling,
		Eigen::MatrixXi &faceListWithCulling);
};