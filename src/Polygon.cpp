/* Polygon.cpp */

#include "Polygon.h"

Polygon::Polygon() {
}

Polygon::Polygon(string polyName, map<string, Face *> faces, map<string, Vertex *> vertices) {
  _name = polyName;
  _vertices = vertices;
  _faces = faces;
}

Polygon::Polygon(string polyName) {
  _name = polyName;
}

/*Polygon::Polygon(const Polygon& poly) {                                           
  for (map<string, Vertex *>::const_iterator it = poly._vertices.begin(); it != poly._vertices.end(); ++it) {
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
*/

Polygon::~Polygon() {
  for (map<string, Vertex *>::iterator it = _vertices.begin(); it != _vertices.end(); it++) {
    delete (*it).second;
  }
  for (map<string, Face *>::iterator it = _faces.begin(); it != _faces.end(); it++) {
    delete (*it).second;
  }
}

void Polygon::draw() {
  for (map<string, Face *>::iterator it = _faces.begin(); it != _faces.end(); it++) {
    Face * currentFace = (*it).second;
    vector<Vertex *> coordinates = currentFace->getCoordinates();
    glBegin(GL_POLYGON);
    for (map<string, Vertex *>::iterator iv = _vertices.begin(); iv != _vertices.end(); iv++) {
      Vertex * currentCoord = (*iv).second;
      vec3 curPos = currentCoord->getPos();
      double x = curPos[0];
      double y = curPos[1];
      double z = curPos[2];
      glVertex3f(x, y, z);
      //std::cout << x << " " << y << " " << z << std::endl;
    }
    glEnd();
  }
}

void Polygon::addVertex(string vName, Vertex * v) {
  _vertices[vName] = v;
}

void Polygon::addFace(string fName, Face * f) {
  _faces[fName] = f;
}
