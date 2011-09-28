/* Face.h */

#ifndef FACE_H_
#define FACE_H_

#include "global.h"
#include "Vertex.h"

class Face
{
 public:
  Face();
  Face(vector<Vertex *> vertices);
  ~Face(void);

  vector<Vertex *> getCoordinates() { return _coordinates; }
 
 private:
  vector<Vertex *> _coordinates;
};

#endif /* FACE_H_ */
