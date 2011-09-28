/* Face.cpp */

#include "Face.h"

Face::Face() {
}

Face::Face(vector<Vertex *> vertices) {
  _coordinates = vertices;
}

Face::~Face(void) {
}
