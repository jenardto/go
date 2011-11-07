/* Polygon.cpp */

#include "Polygon.h"

Polygon::Polygon() {
}
/*
Polygon::~Polygon() {
  for (map<string, Vertex *>::iterator it = _vertices.begin(); it != _vertices.end();
       it++) {
    delete (*it).second;
  }
  for (map<string, Face *>::iterator it = _faces.begin(); it != _faces.end(); 
       it++) {
    delete (*it).second;
  }
}
*/

Polygon::Polygon(string fileName) {
  vector<Vertex> tempVerts;
  ifstream file(fileName.c_str(), ifstream::in);
  if (!file) {
    std::cout << "Could not open given obj file " << fileName << std::endl;
  }
  while (file.good()) {
    string line;
    getline(file, line);
    if (!_parseLine(line, tempVerts)) {
      std::cout << "Failed to parse OBJ file." << std::endl;
      break;
    }
    if (_vertices.size() > 0) // take the first face in the file.                                 
      break;
  }
  file.close();
  std::cout << "Parsed an OBJ file with " << _vertices.size() << " vertices."
            << endl;
}


bool Polygon::_parseLine(string line, vector<Vertex> & temp) {
  string operand;
  bool success = true;
  if (line.empty())
    return true;
  stringstream ss(stringstream::in | stringstream::out);
  ss.str(line);
  ss >> operand;
  if (operand[0] == '#') {
    return true;
  } else if (operand == "v") {
    double x, y, z;
    ss >> x >> y >> z;
    temp.push_back(Vertex(x, y, z));
  } else if (operand == "f") {
    while (!ss.eof()) {
      int i;
      ss >> i;
      addVertex(&temp[i - 1]); // copy vertex in to polygon                                     
    }
  } else {
    cout << "Unknown operand in scene file, skipping line: " << operand
	 << endl;
  }
  if (ss.fail()) {
    std::cout
      << "The bad bit of the input file's line's stringstream is set! Couldn't parse:"
      << std::endl;
    std::cout << "  " << line << std::endl;
    success = false;
  }
  return success;
}


void Polygon::draw(GLenum mode) {
  if (1 > _vertices.size())
    return;
  glBegin(mode);
  for (vector<Vertex>::iterator it = _vertices.begin(); it
	 != _vertices.end(); it++) {
    glVertex3d(it->getPos()[0], it->getPos()[1], it->getPos()[2]);
  }
  glEnd();
}

void Polygon::addVertex(Vertex * v) {
  _vertices.push_back(*v);
}

/*
void Polygon::addVertex(string vName, Vertex * v) {
  _namedvertices[vName] = v;
}
*/
