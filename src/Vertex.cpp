/* Vertex.cpp */

#include "Vertex.h"

Vertex::Vertex(): _pos(0.0, 0.0, 0.0) {
}

Vertex::Vertex(vec3 p): _pos(p) {
}

Vertex::Vertex(double x, double y, double z, int index): _pos(x,y,z), _index(index) {
}

Vertex::Vertex(double x, double y, double z): _pos(x, y, z) {
}

void Vertex::addNeighbor(Vertex * newNeighbor) {
  _neighbors.push_back(newNeighbor);
}

void Vertex::addCentroid(Vertex * centroid) {
  _associatedCentroids.push_back(centroid);
}

vec3 Vertex::getPos() {
  return _pos;
}

void Vertex::setPos(vec3 p) {
  _pos = p;
}

void Vertex::setPos(double x, double y, double z) {
  _pos = vec3(x, y, z);
}

bool Vertex::equals(Vertex vert) {
  vec3 vertPos = vert.getPos();
  return (_pos[0] == vertPos[0] && _pos[1] == vertPos[1] && _pos[2] == vertPos[2]);
}
