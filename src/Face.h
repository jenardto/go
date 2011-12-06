/* Face.h */

#ifndef FACE_H_
#define FACE_H_

#include "global.h"
#include "Vertex.h"

class Face {
 public:
  Face();
  //Face(string fileName);
  Face(vector<Vertex *> vertices);
  
  void addVertex(Vertex * v);
  vector<Vertex *> getCoordinates() { return _vertices; }

  bool onMyPoly(Vertex * vert);

  void addCentroid(Vertex * centroid) { _centroid = centroid; }
  Vertex* getCentroid() { return _centroid; }

  void addTexCoordinate(vec2 uv);
  vector<vec2> getTexCoordinates() { return _texCoordinates; }

  void draw(GLenum mode = GL_LINE_LOOP);

  string getName() { return _name; }

  void setColor(vec3 color) { _color = color; }
  vec3 getColor() { return _color; }

  void setTexName(string texName) { _textureName = texName; }
  string getTexName() { return _textureName; }

 private:
  //bool _parseLine(string, vector<Vertex*> &);
  vector<Vertex *> _vertices;
  vec3 _color;
  Vertex * _centroid;
  vector<vec2> _texCoordinates;
  string _textureName;
  string _name;
};

#endif /* FACE_H_ */
