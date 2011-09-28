/* Polygon.h */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "global.h"
#include "Vertex.h"
#include "Face.h"

class Polygon {

 public:
  Polygon();
  Polygon(const Polygon& poly);     //copy constructor
  Polygon& operator =(const Polygon& poly);     //assignment of a Polygon
  Polygon(string name, vector<Face *> faces, vector<Vertex *> vertices);
  Polygon(string name);
  virtual ~Polygon();

  void draw();

  Vertex * transform(vec3 position, vec3 txform, vec3 rxform);

  /* Adds a vertex to the end of this polygon */
  void addVertex(Vertex * v);
  
  /* Adds a face to the end of this polygon */
  void addFace(Face * f);
  
  /* Get name of poly */
  string getName() { return _name; }

  /* Get vector of vertices */
  vector<Vertex *> getVertices() { return _vertices; }

  /* Get vector of faces */
  vector<Face *> getFaces() { return _faces; }

 private:
  vector<Vertex *> _vertices;
  vector<Face *> _faces;
  string _name;
};

#endif /* POLYGON_H_ */
