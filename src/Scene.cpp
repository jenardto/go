/* Scene.cpp */

#include "Scene.h"
#include <stdio.h>
#include <string.h>

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
  delete _currentPoly;
}

void Scene::draw() {
  //iterator through polygons of scene
  map<string, Polygon *>::iterator it;
  for (it = _polygons.begin(); it != _polygons.end(); it++) {
    _polygons[it->first]->draw();
  }
}

void Scene::addPolygon(string polyName, Polygon * poly) {
  _polygons[polyName] = poly;
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
      if (_openParens == 1) {
	_currentPoly = new Polygon(polyName);
      }
      std::cout << "polygon" << std::endl;
      addPolygon(polyName, _currentPoly);
    } else if (element[1] == 'v') {
      double x, y, z;
      ss >> x >> y >> z;
      Vertex * vert = new Vertex(x, y, z);
      std::cout << "vertex" << std::endl;
      _currentPoly->addVertex(vert);
    } else if (element[1] == 'f') {
      vector<Vertex *> currentVerts = _currentPoly->getVertices();
      vector<Vertex *> addToFace;
      vector<int> tempVec;
      /*char str[element.length()];
      for (int i = 0; i < element.length(); i++) {
	str[i] = element[i];
      }
      char * tok = strtok(str, " ");
      while (tok != NULL) {
	tok = strtok(NULL, " ");
	std::cout << atoi(tok) << std::endl;
	tempVec.push_back(atoi(tok));
	}*/
      while (!ss.eof()) {     //split vertex numbers here
        int vertIndex;
        ss >> vertIndex;
        tempVec.push_back(vertIndex - 1);
      }
      for (int i = 0; i < tempVec.size(); i++) {
	addToFace.push_back(currentVerts[tempVec[i]]);
      }
      Face * newFace = new Face(addToFace);
      std::cout << "face" << std::endl;
      _currentPoly->addFace(newFace);
    } else if (element[1] == 'i') {
      string references;
      ss >> references;
      Polygon * tempPoly = _polygons[references];
      vector<Vertex *> tempVertices = tempPoly->getVertices();
      vector<Face *> tempFaces = tempPoly->getFaces();
      //int offset = (_currentPoly->getVertices()).size();
      for (int i = 0; i < tempVertices.size(); i++) {
	_currentPoly->addVertex(tempVertices[i]);
      }
      for (int i = 0; i < tempFaces.size(); i++) {
	_currentPoly->addFace(tempFaces[i]);
      }
    } else if (element[1] == 'g') {
      string groupName;
      ss >> groupName;
      if (_polygons.count(groupName) > 0) {
	_currentPoly = new Polygon(groupName);
	addPolygon(groupName, _currentPoly);
      }
    }

  } else if (element == ")") {
    _openParens = _openParens - 1;
    //std::cout << "closeParen" << std::endl;
  }
  
  return success;
}
