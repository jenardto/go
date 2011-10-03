/* Polygon.h */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "global.h"
#include "Vertex.h"
#include "Face.h"
#include <map>

class Polygon {

 public:
  Polygon();
  //Polygon(const Polygon& poly);     //copy constructor
  //Polygon& operator =(const Polygon& poly);     //assignment of a Polygon
  Polygon(string name, map<string, Face *> faces, map<string, Vertex *> vertices);
  Polygon(string name);
  virtual ~Polygon();

  void draw();

  Vertex * transform(vec3 position, vec3 txform, vec3 rxform);

  /* Adds a vertex to the end of this polygon */
  void addVertex(string vName, Vertex * v);
  
  /* Adds a face to the end of this polygon */
  void addFace(string fName, Face * f);
  
  /* Get name of poly */
  string getName() { return _name; }

  /* Get vector of vertices */
  map<string, Vertex *> getVertices() { return _vertices; }

  /* Get vector of faces */
  map<string, Face *> getFaces() { return _faces; }

 private:
  map<string, Vertex *> _vertices;
  map<string, Face *> _faces;
  string _name;
};

#endif /* POLYGON_H_ */
