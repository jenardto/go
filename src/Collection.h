/* Collection.h */

#ifndef COLLECTION_H_
#define COLLECTION_H_

#include "global.h"
#include "Face.h"
#include "Texture.h"
#include <map>

class Collection {
 public:
  Collection();
  Collection(vector<Face *> faces, vector<Vertex *> vertices);

  void draw();
  void addVertex(Vertex * v);
  void addFace(Face * f);
 
  void subDivide();
  
  void setFaces(vector<Face *> faces) { _faces = faces; }
  void setVertices(vector<Vertex *> vertices) { _vertices = vertices; }
  vector<Vertex *> getVertices() { return _vertices; }
  vector<Face *> getFaces() { return _faces; }

  map<string, Texture *> getLoadedTextures() { return _loadedTextures; }
  void addLoadedTexture(string texName, Texture * texObj);
  
  int totalVerts() { return _numOfVertices; }

 private:
  int _numOfVertices;
  vector<Vertex *> _vertices;
  vector<Face *> _faces;
  map<string, Texture *> _loadedTextures;
};

#endif /* COLLECTION_H_ */
