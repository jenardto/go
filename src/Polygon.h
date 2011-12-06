/* Polygon.h */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "global.h"
#include "Vertex.h"
#include <map>

class Polygon {
 public:
  Polygon();
  Polygon(string fileName);
  Polygon(vector<Vertex> vertices);

  //UNUSED!!!
  //bool onMyPoly(Vertex vert);
  //void addCentroid(Vertex centroid) { _centroid = centroid; }
  //Vertex getCentroid() { return _centroid; }
  void draw(GLenum mode = GL_LINE_LOOP);

  void addTexCoordinate(vec2 uv);
  vector<vec2> getTexCoordinates() { return _texCoordinates; }

  void addVertex(Vertex v);
  vector<Vertex> getCoordinates() { return _vertices; }

  string getName() { return _name; }

  void setColor(vec3 color) { _color = color; }
  vec3 getColor() { return _color; }

  void setTexName(string texName) { _textureName = texName; }
  string getTexName() { return _textureName; }

 private:
  bool _parseLine(string, vector<Vertex> &);
  vector<Vertex> _vertices;
  vec3 _color;
  Vertex _centroid;
  vector<vec2> _texCoordinates;
  string _textureName;
  string _name;
};

#endif /* POLYGON_H_ */
