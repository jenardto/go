/* Polygon.cpp */

#include "Polygon.h"

Polygon::Polygon() {
}

Polygon::Polygon(string polyName, vector<Face *> faces, vector<Vertex *> vertices) {
  _name = polyName;
  _vertices = vertices;
  _faces = faces;
}

Polygon::Polygon(string polyName) {
  _name = polyName;
}

Polygon::Polygon(const Polygon& poly) {                                           
  for (vector<Vertex *>::const_iterator it = poly._vertices.begin(); it != poly._vertices.end(); ++it) {
    _vertices.push_back(new Vertex(**it));
  }
}

Polygon& Polygon::operator =(const Polygon& poly) {
  if (&poly != this) {
    for (vector<Vertex *>::iterator it = _vertices.begin(); it!= _vertices.end(); ++it) {
      delete *it;
    }
    _vertices.clear();

    for (vector<Vertex *>::const_iterator it = poly._vertices.begin(); it!= poly._vertices.end(); ++it) {
      _vertices.push_back(new Vertex(**it));
    }
  }

  return *this;
}

Polygon::~Polygon() {
  for (vector<Vertex *>::iterator it = _vertices.begin(); it != _vertices.end(); it++) {
    delete *it;
  }
}

void Polygon::draw() {
  for (unsigned int i = 0; i < _faces.size(); i++) {
    Face * currentFace = _faces[i];
    vector<Vertex *> coordinates = currentFace->getCoordinates();
    glBegin(GL_POLYGON);
    for (unsigned int j = 0; j < coordinates.size(); j++) {
      Vertex * currentCoord = coordinates[j];
      vec3 curPos = currentCoord->getPos();
      double x = curPos[0];
      double y = curPos[1];
      double z = curPos[2];
      glVertex3f(x, y, z);
    }
    glEnd();
  }
}

void Polygon::addVertex(Vertex * v) {
  _vertices.push_back(v);
}

void Polygon::addFace(Face * f) {
  _faces.push_back(f);
}
