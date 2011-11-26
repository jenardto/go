/* Texture.h */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "global.h"

class Texture {
  
  GLuint tex; 

 public:
  Texture();
  
  bool polyloadTexture(string textureFile);

  string getName() { return _textureName; }
  void setName(string name) { _textureName = name; }

  GLuint getTex() { return tex; }
 private:
  string _textureName;

};

#endif /* TEXTURE_H_ */
