/* Group.cpp */

#include "Group.h"

Group::Group() {
}

Group::Group(string name, map<string, Polygon *> polys) {
  _name = name;
  _polygons = polys;
}

Group::Group(string name) {
  _name = name;
}

Group::~Group() {
  for (map<string, Polygon *>::iterator ip = _polygons.begin(); ip != _polygons.end(); ip++) {
    map<string, Vertex *> vertices = (*ip).second->getVertices();
    map<string, Face *> faces = (*ip).second->getFaces();
    for (map<string, Face *>::iterator it = faces.begin(); it != faces.end(); it++) {
      delete (*it).second;
    }
    for (map<string, Vertex *>::iterator it = vertices.begin(); it != vertices.end(); it++) {
      delete (*it).second;
    }
  }
}

void Group::draw() {
  for (map<string, Polygon *>::iterator ip = _polygons.begin(); ip != _polygons.end(); ip++) {
    _polygons[(*ip).first]->draw();
    /*map<string, Vertex *> vertices = (*ip).second->getVertices();
    map<string, Face *> faces = (*ip).second->getFaces();
    for (map<string, Face *>::iterator it = faces.begin(); it != faces.end(); it++) {
      Face * currentFace = (*it).second;
      vector<Vertex *> coordinates = currentFace->getCoordinates();
      glBegin(GL_POLYGON);
      for (map<string, Vertex *>::iterator iv = vertices.begin(); iv != vertices.end(); iv++) {
	Vertex * currentCoord = (*iv).second;
	vec3 curPos = currentCoord->getPos();
	double x = curPos[0];
	double y = curPos[1];
	double z = curPos[2];
	glVertex3f(x, y, z);
	std::cout << x << " " << y << " " << z << std::endl;
      }
      glEnd();
      }*/
  }
}

void Group::addPolygon(string polyName, Polygon * poly) {
  _polygons[polyName] = poly;
}
