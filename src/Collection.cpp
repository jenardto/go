/* Collection.cpp */

#include "Collection.h"

Collection::Collection() {
  // Leave it empty.
}
/*
Collection::Collection(const Collection& collection) {
  for (vector<Vertex *>::const_iterator it = collection._vertices.begin(); it
	 != collection._vertices.end(); ++it)
    _vertices.push_back(new Vertex(**it));
}

Collection& Collection::operator =(const Collection& collection) {
  if (&collection != this) {
    for (vector<Vertex *>::iterator it = _vertices.begin(); it
	   != _vertices.end(); ++it)
      delete *it;
    _vertices.clear();

    for (vector<Vertex *>::const_iterator it = collection._vertices.begin(); it
	   != collection._vertices.end(); ++it)
      _vertices.push_back(new Vertex(**it));
  }
  return *this;
}

Collection::~Collection() {
  for (vector<Vertex *>::iterator it = _vertices.begin(); it
	 != _vertices.end(); it++)
    delete *it;
}
*/
Collection::Collection(vector<Polygon *> faces, vector<Vertex *> vertices) {
  _vertices = vertices;
  _faces = faces;
  _numOfVertices = vertices.size();
}

void Collection::draw() {
  if (4 > _faces.size()) {
    return;
  }
  for (int i = 0; i < _faces.size(); i++) {
    Polygon * currentFace = _faces[i];
    vector<Vertex> coordinates = currentFace->getCoordinates();
    vec3 color = currentFace->getColor();
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_POLYGON);
    for (int j = 0; j < coordinates.size(); j++) {
      Vertex currentCoord = coordinates[j];
      vec3 curPos = currentCoord.getPos();
      double x = curPos[0];
      double y = curPos[1];
      double z = curPos[2];
      glVertex3d(x, y, z);
    }
    glEnd();
  }
}

void Collection::addVertex(Vertex * v) {
  _vertices.push_back(v);
  _numOfVertices++;
}

void Collection::addFace(Polygon * f) {
  _faces.push_back(f);
}
