/* Face.h */

#ifndef FACE_H_
#define FACE_H_

#include "global.h"
#include "Vertex.h"
#include <map>

class Face {
  
  // GLuint tex; // move texture loading to main
  
 public:
  Face();
  Face(string fileName);
  Face(vector<Vertex *> vertices);
  //virtual ~Face();
  
  /* from finalProj face class */
  /*****************************/
  vector<Vertex *> getCoordinates() { return _vertices; }
  bool onMyPoly(Vertex * vert);

  void addCentroid(Vertex * centroid) { _centroid = centroid; }
  Vertex* getCentroid() { return _centroid; }

  void addTexCoordinate(vec2 uv);
  vector<vec2> getTexCoordinates() { return _texCoordinates; }
  /*****************************/

  void draw(GLenum mode = GL_LINE_LOOP);

  void addVertex(Vertex * v);

  /* Get name of poly */
  string getName() { return _name; }

  void setColor(vec3 color) { _color = color; }
  vec3 getColor() { return _color; }
  /* Get vector of vertices */
  //map< string, Vertex *> getVertices() { return _namedvertices; }

  /* Get vector of faces */
  //map<string, Face *> getFaces() { return _namedfaces; }
  void setTexName(string texName) { _textureName = texName; }
  string getTexName() { return _textureName; }
  //bool polyLoadTexture(string textureFile);

 private:
  bool _parseLine(string, vector<Vertex*> &);
  vector<Vertex *> _vertices;

  vec3 _color;
  Vertex * _centroid;
  //vector<Vertex *> _coordinates;
  vector<vec2> _texCoordinates;
  string _textureName;
  string _name;
};

#endif /* FACE_H_ */
