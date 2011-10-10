/* Scene.h */

#ifndef SCENE_H_
#define SCENE_H_

#include "global.h"
#include "Polygon.h"
#include "Group.h"
#include <map>

class Scene {
 public:
  Scene();
  Scene(string file);
  ~Scene();
  
  void draw();

  void addPolygon(string polyName, Polygon * poly);
  void addGroup(string groupName, Group * group);

  map<string, Polygon *> getPolys() { return _polygons; }
  map<string, Group *> getGroups() { return _groups; }
  
  bool _parseLine(string line);

  void setOpenParens() { _openParens = 0; }
 
 private:
  int _openParens;
  Polygon * _currentPoly;
  Group * _currentGroup;
  map<string, Polygon *> _polygons;
  map<string, Group *> _groups;
};

#endif /* SCENE_H_ */
