/* SceneData.h */

#ifndef SCENEDATA_H_
#define SCENEDATA_H_

#include "ParametricValue.h"
#include <vector>

class Transform {
  friend class SceneLoader;
 public:
  virtual mat4 getMatrix(int time) = 0; // pure virtual function
  virtual ~Transform() {}
};

class Translate : public Transform {
  ParametricValue *translate[3];
  friend class SceneLoader;

 public:
  mat4 getMatrix(int time) {
    vec3 tr;
    tr[0] = translate[0]->getValue();
    tr[1] = translate[1]->getValue();
    if (translate[2] == NULL)
      tr[2] = 0;
    else
      tr[2] = translate[2]->getValue();
    return translation3D(tr);
  }

  Translate() {
    for (int i = 0; i < 3; i++)
      translate[i] = NULL;
  }

  ~Translate() {
    for (int i = 0; i < 3; i++)
      delete translate[i];
  }
};

class Scale : public Transform {
  ParametricValue *scale[3];
  friend class SceneLoader;

 public:
  mat4 getMatrix(int time) {
    vec3 sc;
    sc[0] = scale[0]->getValue();
    sc[1] = scale[1]->getValue();
    if (scale[2] == NULL)
      sc[2] = 1;
    else
      sc[2] = scale[2]->getValue();
    return scaling3D(sc);
  }

  Scale() {
    for (int i = 0; i < 3; i++)
      scale[i] = NULL;
  }

  ~Scale() {
    for (int i = 0; i < 3; i++)
      delete scale[i];
  }
};

class Rotate : public Transform {
  ParametricValue *angle;
  ParametricValue *axis[3];
  friend class SceneLoader;

 public:
  mat4 getMatrix(int time) {
    vec3 ax;
    if (axis[2] == NULL) {
      ax = vec3(0,0,1);
    } else {
      ax = vec3(axis[0]->getValue(),
		axis[1]->getValue(),
		axis[2]->getValue());
    }
    return rotation3D(ax, angle->getValue());
  };

  Rotate() {
    angle = NULL;
    for (int i = 0; i < 3; i++)
      axis[i] = NULL;
  }

  ~Rotate() {
    delete angle;
    for (int i = 0; i < 3; i++)
      delete axis[i];
  }
};

class Color {
  ParametricValue *_color[3];
  friend class SceneLoader;

 public:
  vec3 getColor(int time) {
    return vec3(_color[0]->getValue(),
                _color[1]->getValue(),
		_color[2]->getValue());
  }

  Color() {
    for (int i = 0; i < 3; i++)
      _color[i] = NULL;
  }

  ~Color() {
    for (int i = 0; i < 3; i++)
      delete _color[i];
  }
};

#endif /* SCENEDATA_H_ */
