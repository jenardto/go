/* Scene.h */

#ifndef SCENE_H_
#define SCENE_H_

#include "global.h"
#include "Polygon.h"
#include <map>

class Scene {
 public:
  Scene();
  Scene(string file);
  ~Scene();
  
  void draw();

  void addPolygon(string polyName, Polygon * poly);

  map<string, Polygon *> getPolys() { return _polygons; }
  
  bool _parseLine(string line);

  void setOpenParens() { _openParens = 0; }
 
 private:
  int _openParens;
  Polygon * _currentPoly;
  map<string, Polygon *> _polygons;
};

#endif /* SCENE_H_ */
