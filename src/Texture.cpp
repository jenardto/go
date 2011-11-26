/* Texture.cpp */

#include "Texture.h"
#include "LoadImage.h"

Texture::Texture() {
}

bool Texture::polyloadTexture(string textureFile) {
  bool texloaded = loadTexture(textureFile, tex);
  return texloaded;
}
