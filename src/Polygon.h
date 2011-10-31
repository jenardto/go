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
  //virtual ~Polygon();

  void draw(GLenum mode = GL_LINE_LOOP);

  void addVertex(Vertex * v);
  
  /* Adds a vertex to the end of this polygon */
  void addVertex(string vName, Vertex * v);
  
  /* Adds a face to the end of this polygon */
  //void addFace(string fName, Face * f);
  
  /* Get name of poly */
  string getName() { return _name; }

  /* Get vector of vertices */
  map<string, Vertex *> getVertices() { return _namedvertices; }

  /* Get vector of faces */
  //map<string, Face *> getFaces() { return _namedfaces; }

 private:
  bool _parseLine(string, vector<Vertex> &);
  vector<Vertex> _vertices;

  map<string, Vertex *> _namedvertices;
  //map<string, Face *> _namedfaces;
  string _name;
};

#endif /* POLYGON_H_ */
