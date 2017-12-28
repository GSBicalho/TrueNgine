#include "ObjectNDManager.h"
#include "ObjectND.h"

#include <fstream>
#include <QDebug>
#include <vector>
#include <set>
#include <map>
#include <math.h>
#include <iomanip>
#include "CameraND.h"
#include "NMatrixOperations.h"

#define print(something) {std::stringstream ss; ss << std::setprecision(20); ss << something; qDebug() << ss.str().c_str(); }

void setTrueIfShared(bool* arrayToSet, Eigen::VectorXd *v1, Eigen::VectorXd *v2) {
	int count1 = 0, count2 = 0;

	while (count1 < v1->size() && count2 < v2->size()) {
		if ((*v1)(count1) >(*v2)(count2)) {
			count2++;
		}
		else if ((*v1)(count1) < (*v2)(count2)) {
			count1++;
		}
		else {
			arrayToSet[(int)(*v1)(count1)] = true;
			count1++;
			count2++;
		}
	}
}

Eigen::VectorXi getPolitopesKDThatComposePolytopeND(std::vector<std::vector<Eigen::VectorXd>*>* polytopeLists, int N, int K, int index) {
	assert(N > K);
	if (N == K + 1) {
		return polytopeLists->at(N)->at(index).cast<int>();
	}
	else {
		auto searchedPolytope = polytopeLists->at(N)->at(index);
		std::set<int> resultSet;
		for (int i = 0; i < searchedPolytope.size(); i++) {
			auto thisPolytopesComponents = getPolitopesKDThatComposePolytopeND(polytopeLists, N - 1, K, searchedPolytope(i));
			for (int j = 0; j < thisPolytopesComponents.size(); j++) {
				resultSet.insert(thisPolytopesComponents(j));
			}
		}
		Eigen::VectorXi result(resultSet.size());
		int currentIndex = 0;
		for (auto it = resultSet.begin(); it != resultSet.end(); it++) {
			result(currentIndex) = *it;
			currentIndex++;
		}
		return result;
	}
}

Eigen::VectorXi orderVerticesToRing(Eigen::VectorXi &vertices, std::vector<Eigen::VectorXd>* edgeList, Eigen::VectorXi &face) {
	return Eigen::VectorXi(0);
}

void triangulateFaces(std::vector<std::vector<Eigen::VectorXd>*>* currentComposingPolytopes) {
	std::vector<Eigen::VectorXd>* edgeList = NULL;
	std::vector<Eigen::VectorXd>* faceList = NULL;
	std::vector<Eigen::VectorXd>* spaceList = NULL;

	if (currentComposingPolytopes->size() >= 3) {
		edgeList = currentComposingPolytopes->at(1);
		faceList = currentComposingPolytopes->at(2);
	}
	else {
		return;
	}

	if (currentComposingPolytopes->size() >= 4) {
		spaceList = currentComposingPolytopes->at(3);
	}

	std::vector<Eigen::VectorXd> newFacesList;
	int firstFaceOfThisFace = 0;

	for (int currentFaceIndex = 0; currentFaceIndex < faceList->size(); currentFaceIndex++) {
		auto verticesOfFace = getPolitopesKDThatComposePolytopeND(currentComposingPolytopes, 2, 0, currentFaceIndex);
		auto currentFace = faceList->at(currentFaceIndex).cast<int>();
		
		
	}
}

ObjectNDManager::ObjectNDManager(int typeExtenson, std::ifstream &inStream, class RenderingWindow* parent) {
	this->parent = parent;
	this->renderingMode = OBJECTND_RENDERINGMODE_LINES;
	this->hasCalculatedCulling = false;
	this->useCulling = false;

	if (typeExtenson == OBJECTNDMANAGER_NDP_FORMAT) {
		print("READING NDP");
		readFromNDPFormatStream(inStream);
	} else if (typeExtenson == OBJECTNDMANAGER_POL_FORMAT) {
		print("READING POL");
		readFromPOLFormatStream(inStream);
	}

	//We triangulate all faces
	//triangulateFaces(&composingPolytopes);

	// We shift every point to the same random minimal direction
	if (composingPolytopes.at(0)->size() != 0) {
		Eigen::VectorXd aux(composingPolytopes.at(0)->at(0).size());
		for (int i = 0; i < composingPolytopes.at(0)->at(0).size(); i++) {
			aux(i) = sinf(i + 1) * 1e-20f;
		}
		for (int i = 0; i < composingPolytopes.at(0)->size(); i++) {
			composingPolytopes.at(0)->at(i) += aux;
		}
	}

}

void ObjectNDManager::readFromNDPFormatStream(std::ifstream &inStream) {
	int numDimensions, dimensioOfHighestStructure;
	inStream >> numDimensions >> dimensioOfHighestStructure;
	this->numberOfDimensions = numDimensions;

	//Initializing the vector of polytopes
	for (int i = 0; i < dimensioOfHighestStructure + 1; i++) {
		composingPolytopes.push_back(new std::vector<Eigen::VectorXd>());
	}

	for (int currentDimension = 0; currentDimension < dimensioOfHighestStructure + 1; currentDimension++) {
		int numberOfPolytopes;
		inStream >> numberOfPolytopes;
		inStream.get();
		std::string polytopeLine;
		for (int i = 0; i < numberOfPolytopes; i++) {
			std::getline(inStream, polytopeLine);
			std::stringstream ss;
			ss << polytopeLine;

			//Read polytope
			std::vector<double> facesOfPolytope(
				(std::istream_iterator<double>(ss)),
				(std::istream_iterator<double>()));

			//Transform it to its global indexes
			Eigen::VectorXd polytopeAfterMapping(facesOfPolytope.size());
			for (int j = 0; j < facesOfPolytope.size(); j++) {
				polytopeAfterMapping(j) = facesOfPolytope[j];
			}

			//Order it
			if (currentDimension != 0) {
				std::sort(polytopeAfterMapping.data(), polytopeAfterMapping.data() + polytopeAfterMapping.size(), [&](double lhs, double rhs) { return lhs < rhs; });
			}
			//Add it to polytope list
			composingPolytopes.at(currentDimension)->push_back(polytopeAfterMapping);
		}
	}


}

void ObjectNDManager::readFromPOLFormatStream(std::ifstream &inStream) {

	int startingDimension, finalDimension;
	inStream >> startingDimension >> finalDimension;
	print("startingDimension: " << startingDimension);
	print("finalDimension: " << finalDimension);

	this->numberOfDimensions = startingDimension;

	Eigen::VectorXi sizeGrid(startingDimension);
	//int* sizeGrid = new int[startingDimension];
	for (int i = 0; i < startingDimension; i++) {
		inStream >> sizeGrid(i);
	}
	print("sizeGrid: " << sizeGrid.transpose());

	std::vector<std::map<std::string, int>*> composingPolytopeToIndexMap;

	//Initializing the vectors
	for (int i = 0; i < startingDimension - finalDimension + 1; i++) {
		composingPolytopes.push_back(new std::vector<Eigen::VectorXd>());
		composingPolytopeToIndexMap.push_back(new std::map<std::string, int>());
	}

	int currentPolytope = 0;
	int currentPolytopeLabel;
	inStream >> currentPolytopeLabel;
	while (currentPolytopeLabel != -1) {
		print("Label " << currentPolytopeLabel);

		//ObjectND(startingDimension, finalDimension, inStream, camerasND, parent);
		Eigen::VectorXd currentCube(startingDimension);
		for (int i = 0; i < startingDimension; i++) {
			inStream >> currentCube(i);
		}
		print("Current Cube: " << currentCube.transpose());

		int numConvexComponents;
		inStream >> numConvexComponents;
		print("numConvexComponents " << numConvexComponents);

		for (int currentConvexComponent = 0; currentConvexComponent < numConvexComponents; currentConvexComponent++) {
			std::vector<Eigen::VectorXd>* vertexList = composingPolytopes.at(0);
			std::map<std::string, int>* vertexMap = composingPolytopeToIndexMap.at(0);

			// Getting the vertices that compose the Polytope
			int numVertexes;
			inStream >> numVertexes;
			print("numVertexes " << numVertexes);
			std::vector<int>* mappingOfCurrentPolytopesToGlobal = new std::vector<int>;
			for (int currentVertexIndex = 0; currentVertexIndex < numVertexes; currentVertexIndex++) {
				Eigen::VectorXd componentLabel(finalDimension + 1);
				for (int i = 0; i < finalDimension + 1; i++) {
					inStream >> componentLabel(i);
				}
				// We won't actually use the component labels for now
				// But maybe someday we will

				//Reads the vertex
				Eigen::VectorXd currentVertex(startingDimension);
				for (int i = 0; i < startingDimension; i++) {
					inStream >> currentVertex(i);
				}

				std::stringstream ss;
				ss << currentVertex;
				if (vertexMap->count(ss.str()) == 0) {
					//Adds it to the list of vertexes, if it isn't there already
					vertexList->push_back(currentVertex);
					(*vertexMap)[ss.str()] = vertexList->size() - 1;
					//vertexMap->at(ss.str()) = vertexList->size() - 1;
					//Maps it to the newest position
					mappingOfCurrentPolytopesToGlobal->push_back(vertexList->size() - 1);
				}
				else {
					//Maps it to found position
					mappingOfCurrentPolytopesToGlobal->push_back(vertexMap->at(ss.str()));
				}
			}

			//Reading edges and higher dimensional structures
			for (int currentDimensionOfComposingPolytopes = 1; currentDimensionOfComposingPolytopes < startingDimension - finalDimension + 1; currentDimensionOfComposingPolytopes++) {
				int cdocp = currentDimensionOfComposingPolytopes;

				std::vector<int>* mappingOfPreviousPolytopesToGlobal = mappingOfCurrentPolytopesToGlobal;
				mappingOfCurrentPolytopesToGlobal = new std::vector<int>();

				int numberOfPolytopesOfCurrentDimension;
				inStream >> numberOfPolytopesOfCurrentDimension;
				inStream.get();
				std::string polytopeLine;
				for (int i = 0; i < numberOfPolytopesOfCurrentDimension; i++) {
					std::getline(inStream, polytopeLine);
					std::stringstream ssA;
					ssA << polytopeLine;

					//Read polytope
					std::vector<int> facesOfPolytope(
						(std::istream_iterator<int>(ssA)),
						(std::istream_iterator<int>()));

					//Transform it to its global indexes
					Eigen::VectorXd polytopeAfterMapping(facesOfPolytope.size());
					for (int j = 0; j < facesOfPolytope.size(); j++) {
						polytopeAfterMapping(j) = mappingOfPreviousPolytopesToGlobal->at(facesOfPolytope[j] - 1);
					}

					if (currentDimensionOfComposingPolytopes == 1 && facesOfPolytope.size() != 2) {
						polytopeAfterMapping.conservativeResize(2);
					}

					//Order it
					std::sort(polytopeAfterMapping.data(), polytopeAfterMapping.data() + polytopeAfterMapping.size(), [&](int lhs, int rhs) { return lhs < rhs; });
					std::stringstream ss;
					ss << polytopeAfterMapping.transpose();

					if (!(composingPolytopeToIndexMap.at(cdocp)->count(ss.str()))) {
						composingPolytopes.at(cdocp)->push_back(polytopeAfterMapping);
						(*composingPolytopeToIndexMap.at(cdocp))[ss.str()] = composingPolytopes.at(cdocp)->size() - 1;
						//Maps it to the newest position
						mappingOfCurrentPolytopesToGlobal->push_back(composingPolytopes.at(cdocp)->size() - 1);
					}
					else {
						//Maps it to found position
						mappingOfCurrentPolytopesToGlobal->push_back(composingPolytopeToIndexMap.at(cdocp)->at(ss.str()));
					}
				}

				delete mappingOfPreviousPolytopesToGlobal;
			}

			// Starts next Polytope
			currentPolytope++;
		}
		inStream >> currentPolytopeLabel;
	}
}

ObjectNDManager::~ObjectNDManager() {
}

void ObjectNDManager::printStructure() {
	for (int i = 0; i < currentComposingPolytopes->size(); i++) {
		print("LAYER " << i);
		for (int j = 0; j < currentComposingPolytopes->at(i)->size(); j++) {
			print(j << ": " << currentComposingPolytopes->at(i)->at(j).transpose());
		}
	}
}

void ObjectNDManager::checkSharedEdgesAndFaces(std::vector<std::vector<Eigen::VectorXd>*>* currentComposingPolytopes, bool* edgeIsShared, bool* faceIsShared) {
	// Check to see which edges should be rendered
	if (currentComposingPolytopes->size() > 3) {
		//TODO: Check if there is currentComposingPolytopes->at(3);
		auto faceList = currentComposingPolytopes->at(2);
		auto spaceList = currentComposingPolytopes->at(3);

		//Compare each edge of each space against every edge of every other space
		for (int i = 0; i < spaceList->size(); i++) {
			auto facesOfSpaceA = spaceList->at(i);

			std::set<int> otherSpacesFaces;
			for (int j = i + 1; j < spaceList->size(); j++) {
				auto facesOfSpaceB = spaceList->at(j);
				for (int k = 0; k < facesOfSpaceB.size(); k++) {
					otherSpacesFaces.insert((int)facesOfSpaceB(k));
				}
			}

			for (int j = 0; j < facesOfSpaceA.size(); j++) {
				auto edgesA = faceList->at((int)facesOfSpaceA(j));
				for (int edgeB : otherSpacesFaces) {
					auto edgesB = faceList->at(edgeB);
					setTrueIfShared(edgeIsShared, &edgesA, &edgesB);
				}
			}
		}
	}

	// Check to see which faces should be rendered
	if (currentComposingPolytopes->size() > 3) {
		auto spaceList = currentComposingPolytopes->at(3);
		for (int i = 0; i < spaceList->size(); i++) {
			auto facesOfSpaceA = spaceList->at(i);
			for (int j = i + 1; j < spaceList->size(); j++) {
				auto facesOfSpaceB = spaceList->at(j);
				setTrueIfShared(faceIsShared, &facesOfSpaceA, &facesOfSpaceB);
			}
		}
	}
}

void ObjectNDManager::generateEdgeAndFaceLists(
	std::vector<std::vector<Eigen::VectorXd>*>* currentComposingPolytopes, 
	Eigen::MatrixXi &edgeListWithoutCulling,
	Eigen::MatrixXi &edgeListWithCulling,
	Eigen::MatrixXi &faceListWithoutCulling,
	Eigen::MatrixXi &faceListWithCulling) {

	auto edgeList = currentComposingPolytopes->at(1);
	

	bool* edgeIsShared = new bool[currentComposingPolytopes->at(1)->size()];
	int sizeFaces = 0;
	if (currentComposingPolytopes->size() > 2) {
		sizeFaces = currentComposingPolytopes->at(2)->size();
	}
	bool* faceIsShared = new bool[sizeFaces];

	for (int i = 0; i < currentComposingPolytopes->at(1)->size(); i++) {
		edgeIsShared[i] = false;
	}
	for (int i = 0; i < sizeFaces; i++) {
		faceIsShared[i] = false;
	}

	hasCalculatedCulling = false;
	if (useCulling) {
		checkSharedEdgesAndFaces(currentComposingPolytopes, edgeIsShared, faceIsShared);
		hasCalculatedCulling = true;
	}

	// Get size of each list
	int sizeEdgeListWithCulling = 0;
	int sizeFaceListWithCulling = 0;
	int sizeFaceListWithoutCulling = 0;
	for (int i = 0; i < currentComposingPolytopes->at(1)->size(); i++) {
		sizeEdgeListWithCulling += !edgeIsShared[i] ? 1 : 0;
	}
	

	//Create lists. Here we create and mantain in memory the list of edges with culling and without,
	//Because it is faster than recalculating it every time we change rendering

	//First we do Edges
	int edgeCullCount = 0;
	edgeListWithoutCulling.conservativeResize(Eigen::NoChange, edgeList->size());
	edgeListWithCulling.conservativeResize(Eigen::NoChange, sizeEdgeListWithCulling);
	for (int i = 0; i < edgeList->size(); i++) {
		edgeListWithoutCulling.col(i) = edgeList->at(i).cast<int>();
		if (!edgeIsShared[i]) {
			edgeListWithCulling.col(edgeCullCount) = edgeList->at(i).cast<int>();
			edgeCullCount++;
		}
	}

	//Now we do Faces
	if (currentComposingPolytopes->size() > 2) {
		auto faceList = currentComposingPolytopes->at(2);

		for (int i = 0; i < currentComposingPolytopes->at(2)->size(); i++) {
			sizeFaceListWithoutCulling += currentComposingPolytopes->at(2)->at(i).size() - 2;
			sizeFaceListWithCulling += (!faceIsShared[i] ? 1 : 0) * (currentComposingPolytopes->at(2)->at(i).size() - 2);
		}

		int faceCullCount = 0;
		int faceNoCullCount = 0;

		faceListWithoutCulling.conservativeResize(3, sizeFaceListWithoutCulling);
		faceListWithCulling.conservativeResize(3, sizeFaceListWithCulling);
		for (int i = 0; i < faceList->size(); i++) {
			auto currentFace = faceList->at(i);

			int numberOfTrianglesInThisFace = currentFace.size() - 2;
			Eigen::MatrixXi trianglesList(3, numberOfTrianglesInThisFace);
			int currentLocationOnTrianglesList = 0;

			int baseVertex = (int)edgeList->at(currentFace(0))(0);

			for (int j = 1; j < currentFace.size() && currentLocationOnTrianglesList != numberOfTrianglesInThisFace; j++) {
				Eigen::VectorXi currentEdge = edgeList->at(currentFace(j)).cast<int>();
				if (!(currentEdge(0) == baseVertex || currentEdge(1) == baseVertex)) {
					Eigen::VectorXi auxVec(3);
					auxVec << baseVertex, currentEdge(0), currentEdge(1);
					trianglesList.col(currentLocationOnTrianglesList) = auxVec;
					currentLocationOnTrianglesList++;
				}
			}

			faceListWithoutCulling.middleCols(faceNoCullCount, numberOfTrianglesInThisFace) = trianglesList;
			faceNoCullCount += numberOfTrianglesInThisFace;
			if (!faceIsShared[i]) {
				faceListWithCulling.middleCols(faceCullCount, numberOfTrianglesInThisFace) = trianglesList;
				faceCullCount += numberOfTrianglesInThisFace;
			}
		}
	}
}

void ObjectNDManager::setUsingCulling(bool value) {
	useCulling = value;
	if (!hasCalculatedCulling && useCulling) {
		Eigen::MatrixXi edgeListWithoutCulling(2, 0);
		Eigen::MatrixXi edgeListWithCulling(2, 0);
		Eigen::MatrixXi faceListWithoutCulling(3, 0);
		Eigen::MatrixXi faceListWithCulling(3, 0);

		//after cuts
		generateEdgeAndFaceLists(currentComposingPolytopes, edgeListWithoutCulling, edgeListWithCulling, faceListWithoutCulling, faceListWithCulling);

		//Create object and add it to render list
		currentObjects.at(0)->edgesWithCullingList = edgeListWithCulling;
		currentObjects.at(0)->facesWithCullingList = faceListWithCulling;

		hasCalculatedCulling = true;
	}

	for (ObjectND* obj : currentObjects) {
		obj->setUseCulling(value);
	}
}

void ObjectNDManager::updateObjects(Eigen::VectorXd &cutsLocation, std::vector<class CameraND> camerasND) {
	currentObjects.clear();

	int maxDimension = composingPolytopes.size() - 1;
	int resultingDimension = camerasND.size() + 3;

	std::vector<std::vector<Eigen::VectorXd>*>* currentComposingPolytopes = &composingPolytopes;

	//calculate cuts
	for (int currentCutIndex = 0; currentCutIndex < cutsLocation.size(); currentCutIndex++) {
		auto vertexList = currentComposingPolytopes->at(0);
		auto edgeList = currentComposingPolytopes->at(1);

		if (vertexList->size() == 0) {
			break;
		}
		
		std::vector<Eigen::VectorXd>* newPoints = new std::vector<Eigen::VectorXd>();
		int* edgeToPointsMapping = new int[edgeList->size()];
		double currentCutLocation = cutsLocation(currentCutIndex);
		//We calculate the cut over every edge
		for (int i = 0; i < edgeList->size(); i++) {
			edgeToPointsMapping[i] = -1;

			int pointAIndex = (int)edgeList->at(i)(0);
			int pointBIndex = (int)edgeList->at(i)(1);

			auto startVertex = vertexList->at(pointAIndex);
			auto endVertex = vertexList->at(pointBIndex);

			auto lastCoordStart = startVertex(startVertex.size() - 1);
			auto lastCoordEnd = endVertex(endVertex.size() - 1);

			Eigen::VectorXd maxLastCoord;
			Eigen::VectorXd minLastCoord;

			if (lastCoordStart > lastCoordEnd) {
				maxLastCoord = startVertex;
				minLastCoord = endVertex;
			} else {
				maxLastCoord = endVertex;
				minLastCoord = startVertex;
			}

			bool crossesCut = minLastCoord(minLastCoord.size() - 1) < currentCutLocation && currentCutLocation < maxLastCoord(maxLastCoord.size() - 1);

			if (crossesCut) {
				Eigen::VectorXd newPoint = startVertex - endVertex;
				newPoint /= newPoint(newPoint.size() - 1);
				newPoint *= currentCutLocation - minLastCoord(minLastCoord.size() - 1);
				newPoint += minLastCoord;
				newPoint.conservativeResize(newPoint.size() - 1);

				edgeToPointsMapping[i] = newPoints->size();
				newPoints->push_back(newPoint);
			}
		}

		//Now we have every single point that our collection of polytopes contains
		//Every edge has generated either 0 or 1 point

		//We create the structure that will hold our new polytopes
		std::vector<std::vector<Eigen::VectorXd>*>* newComposingPolytopes = new std::vector<std::vector<Eigen::VectorXd>*>;
		//The first line shall be our newly found points
		newComposingPolytopes->push_back(newPoints);
		//The rest are initialized empty, and will be built in the next step
		for (int i = 1; i < currentComposingPolytopes->size() - 1; i++) {
			newComposingPolytopes->push_back(new std::vector<Eigen::VectorXd>());
		}

		int newDimension = vertexList->at(0).size() - 1;

		//We start building the structure from the bottom up
		int* previousPolytopeLayerMapping = edgeToPointsMapping;
		int* currentPolytopeLayerMapping;
		edgeToPointsMapping = nullptr;
		for (int currentPolitopeDimension = 1; currentPolitopeDimension < newComposingPolytopes->size(); currentPolitopeDimension++) {
			auto previousPolytopeList = currentComposingPolytopes->at(currentPolitopeDimension);
			auto currentPolytopeList = currentComposingPolytopes->at(currentPolitopeDimension + 1);

			currentPolytopeLayerMapping = new int[currentPolytopeList->size()];

			for (int currentPolytopeIndex = 0; currentPolytopeIndex < currentPolytopeList->size(); currentPolytopeIndex++) {
				auto listOfLowerDimensionPolitopesFound = new std::vector<int>();
				auto currentPolytope = currentPolytopeList->at(currentPolytopeIndex);

				currentPolytopeLayerMapping[currentPolytopeIndex] = -1;
				for (int i = 0; i < currentPolytope.size(); i++) {
					int aux = previousPolytopeLayerMapping[(int)currentPolytope(i)];
					if (previousPolytopeLayerMapping[(int)currentPolytope(i)] != -1) {
						listOfLowerDimensionPolitopesFound->push_back(previousPolytopeLayerMapping[(int)currentPolytope(i)]);
					}
				}

				if (listOfLowerDimensionPolitopesFound->size() != 0 && listOfLowerDimensionPolitopesFound->size() > currentPolitopeDimension) {
					if (currentPolitopeDimension == 1 && listOfLowerDimensionPolitopesFound->size() > 2) {
						while (listOfLowerDimensionPolitopesFound->size() > 2) {
							listOfLowerDimensionPolitopesFound->erase(listOfLowerDimensionPolitopesFound->begin());
						}
					}

					Eigen::VectorXd aux(listOfLowerDimensionPolitopesFound->size());
					for (int i = 0; i < listOfLowerDimensionPolitopesFound->size(); i++) {
						aux(i) = listOfLowerDimensionPolitopesFound->at(i);
					}
					currentPolytopeLayerMapping[currentPolytopeIndex] = newComposingPolytopes->at(currentPolitopeDimension)->size();
					newComposingPolytopes->at(currentPolitopeDimension)->push_back(aux);
				}
				
			}

			delete previousPolytopeLayerMapping;
			previousPolytopeLayerMapping = currentPolytopeLayerMapping;
		}

		currentComposingPolytopes = newComposingPolytopes;
	}
	
	Eigen::MatrixXi edgeListWithoutCulling(2, 0);
	Eigen::MatrixXi edgeListWithCulling(2, 0);
	Eigen::MatrixXi faceListWithoutCulling(3, 0);
	Eigen::MatrixXi faceListWithCulling(3, 0);

	//after cuts
	hasCalculatedCulling = false;
	generateEdgeAndFaceLists(currentComposingPolytopes, edgeListWithoutCulling, edgeListWithCulling, faceListWithoutCulling, faceListWithCulling);

	//Create Vertex Matrix
	Eigen::MatrixXd vertMatrix(resultingDimension, currentComposingPolytopes->at(0)->size());
	for (int i = 0; i < currentComposingPolytopes->at(0)->size(); i++) {
		auto aux = currentComposingPolytopes->at(0)->at(i);
		vertMatrix.col(i) = aux;
	}

	this->currentComposingPolytopes = currentComposingPolytopes;

	//Create object and add it to render list
	ObjectND* newObj = new ObjectND(this->numberOfDimensions, vertMatrix, edgeListWithoutCulling, edgeListWithCulling, faceListWithoutCulling, faceListWithCulling, camerasND, this->parent, this->renderingMode);
	currentObjects.push_back(newObj);
}

void ObjectNDManager::render() {
	for (auto o : currentObjects) {
		o->render();
	}
}

void ObjectNDManager::setRenderingMode(int value) {
	renderingMode = value;
	for (auto o : currentObjects) {
		o->setRenderingMode(value);
	}
}