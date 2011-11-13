/* Collection.h */

#ifndef COLLECTION_H_
#define COLLECTION_H_

#include "global.h"
#include "Polygon.h"

class Collection {
 public:
  Collection();
  //Collection(const Collection& collection);
  //Collection& operator =(const Collection& collection);
  Collection(vector<Polygon *> faces, vector<Vertex *> vertices);
  //virtual ~Collection();

  void draw();
  void addVertex(Vertex * v);
  void addFace(Polygon * f);
  
  vector<Vertex *> getVertices() { return _vertices; }
  vector<Polygon *> getFaces() { return _faces; }
  
  int totalVerts() { return _numOfVertices; }

 private:
  int _numOfVertices;
  vector<Vertex *> _vertices;
  vector<Polygon *> _faces;
};

#endif /* COLLECTION_H_ */
