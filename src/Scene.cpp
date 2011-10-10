/* Scene.cpp */

#include "Scene.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265

Scene::Scene() {
}

Scene::Scene(string file) {
  //parser here
  ifstream inFile(file.c_str(), ifstream::in);
  if (!inFile) {
    std::cout << "Could not open given file " << file << std::endl;
  }
  while(inFile.good()) {
    string line;
    getline(inFile, line);
    std::cout << line << std::endl;
    if (!_parseLine(line)) {
      std::cout << "Failed to parse file." << std::endl;
      break;
    }
  }
  inFile.close();
}

Scene::~Scene() {
  for (map<string, Polygon *>::iterator it = _polygons.begin(); it != _polygons.end(); it++) {
    delete (*it).second;
  }
  for (map<string, Group *>::iterator it = _groups.begin(); it != _groups.end(); it++) {
    delete (*it).second;
  }
  delete _currentPoly;
  delete _currentGroup;
}

void Scene::draw() {
  /*//iterator through polygons of scene
  map<string, Polygon *>::iterator it;
  for (it = _polygons.begin(); it != _polygons.end(); it++) {
    _polygons[it->first]->draw();
  } */
  //iterator through groups of scene
  //std::cout << "drawing... " << std::endl;
  map<string, Group *>::iterator ig;
  for (ig = _groups.begin(); ig != _groups.end(); ig++) {
    //std::cout << ig->first << std::endl;
    _groups[ig->first]->draw();
  }
}

void Scene::addPolygon(string polyName, Polygon * poly) {
  _polygons[polyName] = poly;
}

void Scene::addGroup(string groupName, Group * group) {
  _groups[groupName] = group;
}

bool Scene::_parseLine(string line) {
  bool success = true;
  string element;
  if (line.empty()) {
    return true;
  }
  stringstream ss(stringstream::in | stringstream::out);
  ss.str(line);
  
  ss >> element;

  if (element[0] == '(') {             //open parens must follow with operand: p, v, f, g, i
    _openParens = _openParens + 1;       //p, g, i: names follow operand
    if (element[1] == 'p') {
      string polyName;
      ss >> polyName;
      _currentPoly = new Polygon(polyName);
      addPolygon(polyName, _currentPoly);
    } else if (element[1] == 'v') {
      string vertName;
      ss >> vertName;
      double x, y, z;
      ss >> x >> y >> z;
      Vertex * vert = new Vertex(x, y, z);
      _currentPoly->addVertex(vertName, vert);
    } else if (element[1] == 'f') {
      map<string, Vertex *> currentVerts = _currentPoly->getVertices();
      vector<Vertex *> addToFace;
      vector<string> tempVec;
      string faceName;
      ss >> faceName;
      while (!ss.eof()) {     //split vertex numbers here
        string vertIdentifier;
        ss >> vertIdentifier;
        tempVec.push_back(vertIdentifier);
      }
      for (int i = 0; i < tempVec.size(); i++) {
	addToFace.push_back(currentVerts[tempVec[i]]);
      }
      Face * newFace = new Face(addToFace);
      _currentPoly->addFace(faceName, newFace);
    } else if (element[1] == 'i') {
      string instanceName;
      ss >> instanceName;
      _currentPoly = new Polygon(instanceName); 
      string references;
      ss >> references;
      Polygon * tempPoly = _polygons[references];
      map<string, Vertex *> tempVertices = tempPoly->getVertices();
      map<string, Face *> tempFaces = tempPoly->getFaces();
      map<string, Vertex *> newVertices;
      for (map<string, Vertex *>::iterator it = tempVertices.begin(); it != tempVertices.end(); it++) {
	string vertName = (*it).first;
	Vertex * tempVert = (*it).second;
	vec3 pos = tempVert->getPos();
	double x, y, z;
	x = pos[0];
	y = pos[1];
	z = pos[2];
	Vertex * addMe = new Vertex(x, y, z);
        newVertices[vertName] = addMe;
      }
      while (!ss.eof()) {
	string element;
	ss >> element;
	if (element[0] == '(') {
	  if (element[1] == 't') {
	    double x, y, z;
	    ss >> x >> y >> z;
	    for (map<string, Vertex *>::iterator it = newVertices.begin(); it != newVertices.end(); it++) {
	      string vertName = (*it).first;
	      Vertex * tempVert = (*it).second;
	      vec3 vertPos = tempVert->getPos();
	      double nX, nY, nZ;
	      nX = vertPos[0] + x;
	      nY = vertPos[1] + y;
	      nZ = vertPos[2] + z;
	      tempVert->setPos(nX, nY, nZ);
	    }
	  } else if (element.substr(1,2) == "rx") {
	    double angle;
	    ss >> angle;
	    angle = (angle*PI)/180;
	    vec3 v1 = vec3(1, 0, 0);
	    vec3 v2 = vec3(0, cos(angle), -sin(angle));
	    vec3 v3 = vec3(0, sin(angle), cos(angle));
	    mat3 rotX = mat3(v1, v2, v3);
	    for (map<string, Vertex *>::iterator it = newVertices.begin(); it != newVertices.end(); it++) {
	      string vertName = (*it).first;
	      Vertex * tempVert = (*it).second;
	      vec3 vertPos = tempVert->getPos();
	      vec3 newPos = rotX * vertPos;
	      double nX, nY, nZ;
	      nX = newPos[0];
	      nY = newPos[1];
	      nZ = newPos[2];
	      tempVert->setPos(nX, nY, nZ);
	    }
	  } else if (element.substr(1,2) == "ry") {
	    double angle;
            ss >> angle;
	    angle = (angle*PI)/180;
            vec3 v1 = vec3(cos(angle), 0, sin(angle));
            vec3 v2 = vec3(0, 1, 0);
            vec3 v3 = vec3(-sin(angle), 0, cos(angle));
            mat3 rotY = mat3(v1, v2, v3);
            for (map<string, Vertex *>::iterator it = newVertices.begin(); it != newVertices.end(); it++) {
              string vertName = (*it).first;
              Vertex * tempVert = (*it).second;
              vec3 vertPos = tempVert->getPos();
              vec3 newPos = rotY * vertPos;
              double nX, nY, nZ;
              nX = newPos[0];
              nY = newPos[1];
              nZ = newPos[2];
              tempVert->setPos(nX, nY, nZ);
	    }
	  } else if (element.substr(1,2) == "rz") {
	    double angle;
	    ss >> angle;
	    angle = (angle*PI)/180;
	    vec3 v1 = vec3(cos(angle), -sin(angle), 0);
	    vec3 v2 = vec3(sin(angle), cos(angle), 0);
	    vec3 v3 = vec3(0, 0, 1);
	    mat3 rotZ = mat3(v1, v2, v3);
	    for (map<string, Vertex *>::iterator it = newVertices.begin(); it != newVertices.end(); it++) {
	      string vertName = (*it).first;
	      Vertex * tempVert = (*it).second;
	      vec3 vertPos = tempVert->getPos();
	      vec3 newPos = rotZ * vertPos;
	      double nX, nY, nZ;
	      nX = newPos[0];
	      nY = newPos[1];
	      nZ = newPos[2];
	      tempVert->setPos(nX, nY, nZ);
	    }
	  }
	}
      }
      for (map<string, Vertex *>::iterator it = newVertices.begin(); it != newVertices.end(); it++) {
	_currentPoly->addVertex((*it).first, (*it).second);
      }
      for (map<string, Face *>::iterator it = tempFaces.begin(); it != tempFaces.end(); it++) {
	_currentPoly->addFace((*it).first, (*it).second);
      }
      _currentGroup->addPolygon(instanceName, _currentPoly);
    } else if (element[1] == 'g') {
      string groupName;
      ss >> groupName;
      _currentGroup = new Group(groupName);
      addGroup(groupName, _currentGroup);
    }
  } else if (element == ")") {
    //_openParens = _openParens - 1;    
  }
  
  return success;
}
