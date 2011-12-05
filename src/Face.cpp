/* Face.cpp */

#include "Face.h"
#include "LoadImage.h"

Face::Face() {
}

Face::Face(vector<Vertex *> vertices) {
  for (int i = 0; i < vertices.size(); i++) {
    _vertices.push_back(vertices[i]);
  }
}

Face::Face(string fileName) {
  vector<Vertex*> tempVerts;
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

bool Face::onMyPoly(Vertex * vert) {
  for (int i = 0; i < _vertices.size(); i++) {
    if (_vertices[i]->equals(vert)) {
      return true;
    }
  }
  return false;
}

void Face::addTexCoordinate(vec2 uv) {
  _texCoordinates.push_back(uv);
}

bool Face::_parseLine(string line, vector<Vertex*> & temp) {
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
    temp.push_back(new Vertex(x, y, z));
  } else if (operand == "f") {
    while (!ss.eof()) {
      int i;
      ss >> i;
      addVertex(temp[i - 1]); // copy vertex in to polygon                                     
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


void Face::draw(GLenum mode) {
  if (1 > _vertices.size())
    return;
  if (_textureName == "noTexture" || _textureName == "") {
    glDisable(GL_TEXTURE_2D);
    glColor3f(_color[0], _color[1], _color[2]);
    glBegin(GL_POLYGON);
    for (int i = 0; i < _texCoordinates.size(); i++) {                                           
      vec2 texPos = _texCoordinates[i];                                                         
      glVertex3d(_vertices[i]->getPos()[0], _vertices[i]->getPos()[1], _vertices[i]->getPos()[2]);   
    } 
    glEnd();
  } else {
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    //glActiveTexture(GL_TEXTURE0);                                                
    //glBindTexture(GL_TEXTURE_2D, tex);

    glBegin(GL_POLYGON);
    for (int i = 0; i < _texCoordinates.size(); i++) {
      vec2 texPos = _texCoordinates[i];
      glTexCoord2d(GLdouble(texPos[0]), GLdouble(texPos[1]));
      glVertex3d(_vertices[i]->getPos()[0], _vertices[i]->getPos()[1], _vertices[i]->getPos()[2]);
    }
    glEnd();
  }
}

void Face::addVertex(Vertex * v) {
  _vertices.push_back(v);
}
/*
bool Face::polyLoadTexture(string textureFile) {
  bool texloaded = loadTexture(textureFile, tex);
  return texloaded;
}
*/
