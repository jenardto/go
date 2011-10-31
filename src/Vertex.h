/* Vertex.h */

#ifndef VERTEX_H_
#define VERTEX_H_

#include "global.h"

class Vertex
{
 public:
  Vertex();
  Vertex(vec3 p);
  Vertex(double x, double y, double z);
  
  vec3 getPos();
  void setPos(vec3 p);
  void setPos(double x, double y, double z);

 private:
  vec3 _pos; 
};

#endif /* VERTEX_H_*/
