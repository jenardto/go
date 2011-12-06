/* Collection.cpp */

#include "Collection.h"

Collection::Collection() {
}

Collection::Collection(vector<Face *> faces, vector<Vertex *> vertices) {
  _vertices = vertices;
  _faces = faces;
  _numOfVertices = vertices.size();
}

void Collection::draw() {
  for (int i = 0; i < _faces.size(); i++) {
    Face * currentFace = _faces[i];
    if (currentFace->getTexName() != "noTexture" && currentFace->getTexName() != "") {
      string texName = currentFace->getTexName();
      Texture * currentTexture = _loadedTextures[texName];
      glBindTexture(GL_TEXTURE_2D, currentTexture->getTex());
    }
    currentFace->draw();
  }
}

void Collection::addVertex(Vertex * v) {
  _vertices.push_back(v);
  _numOfVertices++;
}

void Collection::addFace(Face * f) {
  _faces.push_back(f);
}

void Collection::addLoadedTexture(string texName, Texture * texObj) {
  _loadedTextures[texName] = texObj;
}

void Collection::subDivide() {
  std::cout << "in subdivide" << std::endl;
  std::cout << "numVertices: " << _vertices.size() << std::endl;
  std::cout << "numFaces: " << _faces.size() << std::endl;
  vector<Face *> newFaces;
  vector<Vertex *> newVertices;
  vector<Vertex *> midpointVerts;
  int indexNewVertices = 0;
  int initSizeOfFaces = _faces.size();
  for (int i = 0; i < initSizeOfFaces; i ++) {
    Face * curFace = _faces[i];
    vector<vec2> curFaceTexCoords = curFace->getTexCoordinates();                        
    int initSizeOfVerticesOnFace = curFace->getCoordinates().size();                     
    vec3 curFaceCentroid = curFace->getCentroid()->getPos();                             
    vec2 curFaceCentroidTexCoord = curFace->getCentroid()->getTextureCoord();            
    double centX = curFaceCentroid[0];                                                   
    double centY = curFaceCentroid[1];                                                   
    double centZ = curFaceCentroid[2];                                                   
                                                                                              
    double centTexU = curFaceCentroidTexCoord[0];                                        
    double centTexV = curFaceCentroidTexCoord[1];                                     
                                                                                          
    Vertex * centroid = new Vertex(centX, centY, centZ, indexNewVertices);
    indexNewVertices++;                                                                  
    newVertices.push_back(centroid);

    for (int k = 0; k < initSizeOfVerticesOnFace; k++) { //add new vertices   
      vector<vec2> newFaceTexCoords;                                               
      Vertex * curVert = curFace->getCoordinates()[k];                             
      vec2 curVertTextureCoord = curFaceTexCoords[k];                              
      vec3 curVertPos = curVert->getPos();                                         
      bool addNewCurVert = true;                                                   
      int indexOfNewCurVert = 0;   //disregard if newCurVert already appears in newVertices    
      for (int w = 0; w < newVertices.size(); w++) {                               
	Vertex * temp = newVertices[w];                                      
	if (temp->equals(curVert)) {
	  indexOfNewCurVert = w;                                       
	  addNewCurVert = false;                                       
	  break;                                                       
	}                                                    
      }
      Vertex * newCurVert;                                                         
      if (addNewCurVert) {                                                         
	newCurVert = new Vertex(curVertPos[0], curVertPos[1], curVertPos[2], indexNewVertices);
	newFaceTexCoords.push_back(curFaceTexCoords[k]);                     
	indexNewVertices++;
	newVertices.push_back(newCurVert);                                   
      } else {                                                                     
	newCurVert = newVertices[indexOfNewCurVert];                         
	newFaceTexCoords.push_back(curFaceTexCoords[k]);                     
      }                                                                            
      vector<Vertex *> neighbors = curVert->getNeighbors();                        
      vector<Vertex *> vectorForNewFace;                                           
      vectorForNewFace.push_back(newCurVert);
      
      bool addCentroid = true;
      
      for (int l = 0; l < neighbors.size(); l++) {
	bool vertExists = false;
	int indexOfExistingVert;
	if (curFace->onMyPoly(neighbors[l])) {
	  vec2 neighborVertTexCoord;
	  for (int u = 0; u < curFace->getCoordinates().size(); u++) {
	    if (neighbors[l]->equals(curFace->getCoordinates()[u])) {
	      neighborVertTexCoord = curFaceTexCoords[u];
	      break;
	    }
	  }

	  vec3 neighborVertPos = neighbors[l]->getPos();
	  double newVertX = curVertPos[0] + neighborVertPos[0];
	  double newVertY = curVertPos[1] + neighborVertPos[1];
	  double newVertZ = curVertPos[2] + neighborVertPos[2];

	  double newVertTexU = curVertTextureCoord[0] + neighborVertTexCoord[0];
	  double newVertTexV = curVertTextureCoord[1] + neighborVertTexCoord[1];

	  for (int m = 0; m < newVertices.size(); m++) {
	    vec3 tempPos = newVertices[m]->getPos();             
	    double tempX = tempPos[0];                           
	    double tempY = tempPos[1];                           
	    double tempZ = tempPos[2];                           
	    if (tempX == newVertX/2 && tempY == newVertY/2 && tempZ == newVertZ/2) {
	      vertExists = true;                           
	      indexOfExistingVert = m;                     
	    }
	  }

	  Vertex * newVert;
	  if (!vertExists) {
	    newVert = new Vertex(newVertX/2, newVertY/2, newVertZ/2, indexNewVertices);
	    newFaceTexCoords.push_back(vec2(GLdouble(newVertTexU/2), GLdouble(newVertTexV/2)));
	    indexNewVertices++;
	    newVertices.push_back(newVert);
	    midpointVerts.push_back(newVert);
	  } else {
	    newVert = newVertices[indexOfExistingVert];
	    newFaceTexCoords.push_back(vec2(GLdouble(newVertTexU/2), GLdouble(newVertTexV/2)));
	  }
	  vectorForNewFace.push_back(newVert);
	  if (addCentroid) {
	    vectorForNewFace.push_back(centroid);
	    newFaceTexCoords.push_back(vec2(GLdouble(centTexU), GLdouble(centTexV)));
	    addCentroid = false;
	  }
	}
      }
      
      Face * newFace = new Face(vectorForNewFace);
      newFace->setTexName(curFace->getTexName());
      newFace->setColor(curFace->getColor());
      for (int t = 0; t < newFaceTexCoords.size(); t++) {
	newFace->addTexCoordinate(newFaceTexCoords[t]);
      }
      newFaces.push_back(newFace);                  
      for (int n = 0; n < vectorForNewFace.size(); n++) {
	Vertex * myNext = newVertices[vectorForNewFace[(n+1)%vectorForNewFace.size()]->getIndex()];
	Vertex * myPrev = newVertices[vectorForNewFace[(n-1)%vectorForNewFace.size()]->getIndex()];
	bool nextOK = true;
	bool prevOK = true;
	for (int p = 0; p < newVertices[vectorForNewFace[n]->getIndex()]->getNeighbors().size(); p++) {
	  if (newVertices[vectorForNewFace[n]->getIndex()]->getNeighbors()[p]->equals(myNext))   
	    nextOK = false;                                                                     
	  else if (newVertices[vectorForNewFace[n]->getIndex()]->getNeighbors()[p]->equals(myPrev))
	    prevOK = false;                                                                
	}
	if (nextOK)
	  newVertices[vectorForNewFace[n]->getIndex()]->addNeighbor(myNext);
	if (prevOK)
	  newVertices[vectorForNewFace[n]->getIndex()]->addNeighbor(myPrev);
      }
    }
  }
  vector<int> idxOfMidVerts;
  vector<vec3> newValsForMidVerts;
  for (int i = 0; i < midpointVerts.size(); i++) {
    Vertex * curMidVert = midpointVerts[i];
    idxOfMidVerts.push_back(curMidVert->getIndex());
    vector<Vertex *> midNeighbors = curMidVert->getNeighbors();
    int numMidNeighbors = midNeighbors.size();
    vec3 firstMidNeighbor = midNeighbors[0]->getPos();
    double midX = firstMidNeighbor[0];
    double midY = firstMidNeighbor[1];
    double midZ = firstMidNeighbor[2];
    for (int j = 1; j < numMidNeighbors; j++) {
      vec3 nextMidNeighbor = midNeighbors[j]->getPos();
      midX += nextMidNeighbor[0];
      midY += nextMidNeighbor[1];
      midZ += nextMidNeighbor[2];
    }
    midX = midX/numMidNeighbors;
    midY = midY/numMidNeighbors;
    midZ = midZ/numMidNeighbors;
    newValsForMidVerts.push_back(vec3(midX, midY, midZ));
  }
  for (int i = 0; i < _vertices.size(); i++) {
    Vertex * oVert = _vertices[i];
    Vertex * nVert;
    for (int k = 0; k < newVertices.size(); k++) {
      nVert = newVertices[k];
      if (nVert->equals(oVert)) {
	break;
      }
    }
    vector<Vertex *> oVertCentroids = oVert->getCentroids();
    int numCentroids = oVertCentroids.size();                    
    vec3 initCent = oVertCentroids[0]->getPos();
    double xFaceAvg = initCent[0];
    double yFaceAvg = initCent[1];
    double zFaceAvg = initCent[2];
    for (int j = 1; j < oVertCentroids.size(); j++) {
      vec3 nextCent = oVertCentroids[j]->getPos();
      xFaceAvg += nextCent[0];
      yFaceAvg += nextCent[1];
      zFaceAvg += nextCent[2];
    }
    xFaceAvg = xFaceAvg/numCentroids;
    yFaceAvg = yFaceAvg/numCentroids;
    zFaceAvg = zFaceAvg/numCentroids;            
    vector<Vertex *> nNeighbors = nVert->getNeighbors();
    vec3 firstNeighbor = nNeighbors[0]->getPos();
    int numNeighbors = nNeighbors.size();
    double nX = firstNeighbor[0];
    double nY = firstNeighbor[1];
    double nZ = firstNeighbor[2];
    for (int j = 1; j < nNeighbors.size(); j++) {
      vec3 nextNeighbor = nNeighbors[j]->getPos();
      nX += nextNeighbor[0];
      nY += nextNeighbor[1];
      nZ += nextNeighbor[2];
    }
    nX = nX/numNeighbors;
    nY = nY/numNeighbors;
    nZ = nZ/numNeighbors;            
    vec3 nVertPos = nVert->getPos();
    double newX = (xFaceAvg + (2*nX) + ((numNeighbors - 3)*nVertPos[0]))/numNeighbors;
    double newY = (yFaceAvg + (2*nY) + ((numNeighbors - 3)*nVertPos[1]))/numNeighbors;
    double newZ = (zFaceAvg + (2*nZ) + ((numNeighbors - 3)*nVertPos[2]))/numNeighbors;
    nVert->setPos(newX, newY, newZ);
  }
  for (int i = 0; i < midpointVerts.size(); i++) {
    Vertex * curMidVert = midpointVerts[i];
    int index = 0;
    for (int j = 0; j < idxOfMidVerts.size(); j++) {
      if (curMidVert->getIndex() == idxOfMidVerts[j]) {
	index = j;
	break;
      }
    }
    vec3 newValues = newValsForMidVerts[index];
    curMidVert->setPos(newValues[0], newValues[1], newValues[2]);
  }
  for (int i = 0; i < newFaces.size(); i++) {
    Face * nFace = newFaces[i];
    int numVertsInFace = nFace->getCoordinates().size();
    vector<vec2> nFaceTexCoords = nFace->getTexCoordinates();
    vec2 firstVertTexCoord = nFaceTexCoords[0];
    vec3 firstVert = nFace->getCoordinates()[0]->getPos();
    double newCentroidX = firstVert[0];
    double newCentroidY = firstVert[1];
    double newCentroidZ = firstVert[2];
    double newCentroidTexU = firstVertTexCoord[0];
    double newCentroidTexV = firstVertTexCoord[1];    
    for (int z = 1; z < numVertsInFace; z++) {
      vec2 nextVertTexCoord = nFaceTexCoords[z];
      vec3 nextVert = nFace->getCoordinates()[z]->getPos();
      newCentroidX += nextVert[0];
      newCentroidY += nextVert[1];
      newCentroidZ += nextVert[2];
      newCentroidTexU += nextVertTexCoord[0];
      newCentroidTexV += nextVertTexCoord[1];
    }
    Vertex * nFCentroid = new Vertex(newCentroidX/numVertsInFace, newCentroidY/numVertsInFace, newCentroidZ/numVertsInFace);
    nFCentroid->setTextureCoord(vec2(GLdouble(newCentroidTexU/numVertsInFace), GLdouble(newCentroidTexV/numVertsInFace)));
    nFace->addCentroid(nFCentroid);
    for (int z = 0; z < numVertsInFace; z++) {
      nFace->getCoordinates()[z]->addCentroid(nFCentroid);
    }
  }                                   
  _faces.clear();
  for (int n = 0; n < newFaces.size(); n++) {
    for (int m = 0; m < newFaces[n]->getTexCoordinates().size(); m++) {
    }
    _faces.push_back(newFaces[n]);
  }
  _vertices.clear();
  _numOfVertices = 0;
  for (int n = 0; n < newVertices.size(); n++) {
    _vertices.push_back(newVertices[n]);
  }

}
