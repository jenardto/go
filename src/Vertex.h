/* Vertex.h */

#ifndef VERTEX_H_
#define VERTEX_H_

#include "global.h"

class Vertex
{
 public:
  Vertex();
  Vertex(vec3 p);
  Vertex(double x, double y, double z, int index);
  Vertex(double x, double y, double z);
  
  vec3 getPos();
  void setPos(vec3 p);
  void setPos(double x, double y, double z);

  int getIndex() { return _index; }
  
  void addNeighbor(Vertex * newNeighbor);
  vector<Vertex *> getNeighbors() { return _neighbors; }

  void addCentroid(Vertex * centroid);
  vector<Vertex *> getCentroids() { return _associatedCentroids; }

  bool equals (Vertex vert);

  void setTextureCoord(vec2 uv) { _texCoord = uv; }
  vec2 getTextureCoord() { return _texCoord; }

 private:
  vec3 _pos; 
  int _index;
  vector<Vertex *> _associatedCentroids;
  vector<Vertex *> _neighbors; // stores index of neighboring vertices
  vec2 _texCoord; // for use only by the temp centroids
};

#endif /* VERTEX_H_*/
