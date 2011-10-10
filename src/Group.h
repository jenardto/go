/* Group.h */

#ifndef GROUP_H_
#define GROUP_H_

#include "global.h"
#include "Polygon.h"
#include "Vertex.h"
#include "Face.h"
#include <map>

class Group {

 public:
  Group();
  Group(string name, map<string, Polygon *> polys);
  Group(string name);
  virtual ~Group();

  void draw();

  //Vertex * transform(vec3 position, vec3 txform, vec3 rxform);

  /* Sets map of Polygons */
  void setPolygons(map<string, Polygon *> polygons) { _polygons = polygons; }

  /* Adds a Polygon to this group*/
  void addPolygon(string polyName, Polygon * poly);
  
  /* Get name of group */
  string getName() { return _name; }

  /* Get map of polys */
  map<string, Polygon *> getPolygons() { return _polygons; }

 private:
  map<string, Polygon *> _polygons;
  string _name;
};

#endif /* GROUP_H_ */
