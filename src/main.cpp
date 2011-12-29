/* main.cpp */

#include "main.h"
#include <stack>
#include <limits>

using namespace std;
class Viewport {
public:
  Viewport(): mousePos(0.0,0.0) { orientation = identity3D(); };
  int w, h; // width and height                                                       
  vec2 mousePos;
  mat4 orientation;
};

//Global Variables
Viewport viewport;
Scene * scene;
Collection * collection;
Collection * prevCollection;
bool showPrevCollection;
int button, state, xOrigin, yOrigin;
UCB::ImageSaver * imgSaver;

void applyMat4(mat4 &m) {
  double glmat[16];
  int idx = 0;
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      glmat[idx++] = m[j][i];
    }
  }
  glMultMatrixd(glmat);
}

/*void setupView() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,-3);
  applyMat4(viewport.orientation);
}
*/


void RenderInstance(SceneInstance *n, vec3 color, string texture) {
  glPushMatrix();

  // color                                                                              
  float r, g, b;
  bool colorFlag = n->computeColor(color);
  if (colorFlag) {
    r = color[0];
    g = color[1];
    b = color[2];
  }
  
  // texture
  string texName;
  if (n->computeTexture(texture)) {
    texName = texture;
  }
  std::cout << "texture: " + texture << std::endl;
  bool textureAlreadyLoaded = false;
  map<string, Texture *> loadedTextures = collection->getLoadedTextures();
  for (map<string, Texture *>::iterator it = loadedTextures.begin(); 
       it != loadedTextures.end(); it++) {
    string key = (*it).first;
    if (texture == key) {
      textureAlreadyLoaded = true;
    }
  }
  if (!textureAlreadyLoaded) {
    Texture * newTexture = new Texture();
    newTexture->setName(texture);
    newTexture->polyloadTexture(texture);
    collection->addLoadedTexture(texture, newTexture);
  }

  // transformations                                                                    
  mat4 transformMatrix;
  GLdouble matrix[16];
  if (n->computeTransform(transformMatrix)) {
    int k = 0;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
	matrix[k] = transformMatrix[j][i];
	k++;
      }
    }
    glMultMatrixd(matrix);
  }
  GLdouble mat[16];
  mat4 tMat;
  glGetDoublev(GL_PROJECTION_MATRIX, mat);
  int k = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tMat[j][i] = mat[k];
      k++;
    }
  }

  if (n->getChild() != NULL) {
    SceneGroup * child = n->getChild();    
    if (child->getPolygon() != NULL) {      
      Face * newFace = new Face();

      newFace->setTexName(texture);
      newFace->setColor(color);
      
      vector<vec2> texCoords = child->getPolygon()->getTexCoordinates();
      double centroidTexCoordsX = 0;
      double centroidTexCoordsY = 0;
      for (int i = 0; i < texCoords.size(); i++) {
	centroidTexCoordsX = centroidTexCoordsX + texCoords[i][0];
	centroidTexCoordsY = centroidTexCoordsY + texCoords[i][1];
	newFace->addTexCoordinate(texCoords[i]);
      }
      centroidTexCoordsX = centroidTexCoordsX/texCoords.size();
      centroidTexCoordsY = centroidTexCoordsY/texCoords.size();
      vec2 centroidTex = vec2(centroidTexCoordsX, centroidTexCoordsY);

      vector<Vertex> tempVerts = child->getPolygon()->getCoordinates();
      double centroidX, centroidY, centroidZ;
      centroidX = 0;
      centroidY = 0;
      centroidZ = 0;
      int numVerts = tempVerts.size();
      for (int i = 0; i < numVerts; i++) {
	Vertex tempVert = tempVerts[i];

	vec3 tempVPos = tempVert.getPos();
	double x, y, z;
	x = tempVPos[0];
	y = tempVPos[1];
	z = tempVPos[2];
	
	vec4 transformVert = vec4(x, y, z, 1);
	transformVert = tMat * transformVert;
	double pX, pY, pZ;
	pX = transformVert[0];
	pY = transformVert[1];
	pZ = transformVert[2];

	centroidX = centroidX + pX;
	centroidY = centroidY + pY;
	centroidZ = centroidZ + pZ;

	Vertex * newVert = new Vertex(pX, pY, pZ);
	newFace->addVertex(newVert);
       
	vector<Vertex*> colCurVerts = collection->getVertices();
	bool addToCollection = true;
	for (int k = 0; k < colCurVerts.size(); k++) {
	  if (colCurVerts[k]->equals(tempVert)) {
	    addToCollection = false;
	  }
	}
	if (addToCollection) {
	  collection->addVertex(newVert);
	}
	/*
	bool addToCollection = true;
	for (int i = 0; i < collection->getVertices().size(); i++) {
	  Vertex cv = collection->getVertices()[i];
	  if (cv.equals(newVert)) {
	    addToCollection = false;
	  }
	}
	if (addToCollection) {
	  collection->addVertex(newVert);
	}
	*/
      }
      centroidX = centroidX/numVerts;
      centroidY = centroidY/numVerts;
      centroidZ = centroidZ/numVerts;
      Vertex * cent = new Vertex(centroidX, centroidY, centroidZ);
      cent->setTextureCoord(centroidTex);
      newFace->addCentroid(cent);
      collection->addFace(newFace);
    }
    
    // goes through all children                                                  
    for (int i = 0; i < child->getChildCount(); i++) {
      RenderInstance(child->getChild(i), color, texture);
    }
  }
  glPopMatrix();
}

void mergeVertices() {
  vector<Vertex*> mergedVertices;
  vector<Vertex*> allVertices = collection->getVertices();
  std::cout << "in merging: " << allVertices.size()  << std::endl;
  for (int i = 0; i < allVertices.size(); i++) {
    bool addToCollection = true;
    for (int j = 0; j < mergedVertices.size(); j++) {
      if (mergedVertices[j]->equals(allVertices[i])) {
	addToCollection = false;
      }
    }
    if (addToCollection) {
      mergedVertices.push_back(allVertices[i]);
      int newIndex = mergedVertices.size() - 1;
      mergedVertices[newIndex]->setIndex(newIndex);
    }
  }
  collection->setVertices(mergedVertices);
  std::cout << "numMergedVerts: " << mergedVertices.size() << std::endl;
  // make faces of each polygon in collection be pointers to mergedVertices
  vector<Face *> newFaces;
  vector<Face *> colCurFaces = collection->getFaces();
  for (int i = 0; i < colCurFaces.size(); i++) {
    Face * newFace = new Face();
    newFace->setTexName(colCurFaces[i]->getTexName());
    newFace->setColor(colCurFaces[i]->getColor());
    newFace->addCentroid(colCurFaces[i]->getCentroid());
    for (int j = 0; j < colCurFaces[i]->getTexCoordinates().size(); j++) {
      newFace->addTexCoordinate(colCurFaces[i]->getTexCoordinates()[j]);
    }
    vector<Vertex *> curFaceVerts = colCurFaces[i]->getCoordinates();
    for (int j = 0; j < curFaceVerts.size(); j++) {
      for (int k = 0; k < mergedVertices.size(); k++) {
	if (curFaceVerts[j]->equals(mergedVertices[k])) {
	  mergedVertices[k]->addCentroid(colCurFaces[i]->getCentroid());
	  newFace->addVertex(mergedVertices[k]);
	  std::cout << "added vertex to newFace" << std::endl;
	  break;
	}
      }
    }
    newFaces.push_back(newFace);
  }
  std::cout << "setting faces size of newFaces: " << newFaces.size()  << std::endl;
  collection->setFaces(newFaces);
  for (int i = 0; i < newFaces.size(); i ++) {
    std::cout << "face: " << std::endl;
    for (int j = 0; j < newFaces[i]->getCoordinates().size(); j++) {
      std::cout << newFaces[i]->getCoordinates()[j]->getPos() << std::endl;
    }
  }

  std::cout << "about to add vertex neighbors" << std::endl;
  // add vertex neighbors
  for (int i = 0; i < newFaces.size(); i++) {
    //std::cout << "int i loop" << std::endl;
    vector<Vertex *> curFaceVerts = newFaces[i]->getCoordinates();
    for (int j = 0; j < curFaceVerts.size() - 1; j++) {
      //std::cout << "int j loop" << std::endl;
      Vertex * curVert = curFaceVerts[j];
      if (j == 0) {
	// need todo: check to make sure that neighbors not already added
	//std::cout << "0 case" << std::endl;
	bool addNeighbor1 = true;
	bool addNeighbor2 = true;
	for (int k = 0; k < curVert->getNeighbors().size(); k++) {
	  if (curFaceVerts[1]->equals(curVert->getNeighbors()[k])) {
	    addNeighbor1 = false;
	  }
	  if (curFaceVerts[curFaceVerts.size() - 1]->equals(curVert->getNeighbors()[k])) {
	    addNeighbor2 = false;
	  }
	}
	if (addNeighbor1) {
	  curVert->addNeighbor(curFaceVerts[1]);
	}
	if (addNeighbor2) {
	  curVert->addNeighbor(curFaceVerts[curFaceVerts.size() - 1]);
	}
	bool addNeighbor3 = true;
	for (int k = 0; k < curFaceVerts[1]->getNeighbors().size(); k++) {
	  if (curVert->equals(curFaceVerts[1]->getNeighbors()[k])){
	    addNeighbor3 = false;
	  }
	}
	if (addNeighbor3) {
	  curFaceVerts[1]->addNeighbor(curVert);
	}
	bool addNeighbor4 = true;
	for (int k = 0; k < curFaceVerts[curFaceVerts.size() - 1]->getNeighbors().size(); k++) {
	  if (curVert->equals(curFaceVerts[curFaceVerts.size() - 1]->getNeighbors()[k])) {
	    addNeighbor4 = false;
	  }
	}
	if (addNeighbor4) {
	  curFaceVerts[curFaceVerts.size() - 1]->addNeighbor(curVert);
	}
      } else {
	bool addNeighbor1 = true;
	for (int k = 0; k < curVert->getNeighbors().size(); k++) {
	  if (curFaceVerts[j + 1]->equals(curVert->getNeighbors()[k])) {
	    addNeighbor1 = false;
	  }
	}
	if (addNeighbor1) {
	  curVert->addNeighbor(curFaceVerts[j + 1]);
	}
	bool addNeighbor2 = true;
	for (int k = 0; k < curFaceVerts[j + 1]->getNeighbors().size(); k++) {
	  if (curVert->equals(curFaceVerts[j + 1]->getNeighbors()[k])) {
	    addNeighbor2 = false;
	  }
	}
	if (addNeighbor2) {
	  curFaceVerts[j + 1]->addNeighbor(curVert);
	}
      }
    }
  }
  

}



void display() {
  //Clear Buffers
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -3);
  applyMat4(viewport.orientation);
  /*
  Polygon * p = new Polygon();
  p->addVertex(Vertex(0.0, 0.0, 0.0));
  p->addVertex(Vertex(-0.5, 0.0, 0.0));
  p->addVertex(Vertex(-0.5, -0.5, 0.0));
  p->addVertex(Vertex(0.5, -0.5, 0.0));
  p->addVertex(Vertex(0.5, 0.5, 0.0));
  p->addVertex(Vertex(0.0, 0.5, 0.0));
  p->addTexCoordinate(vec2(GLdouble(0.5),GLdouble(0.25)));
  p->addTexCoordinate(vec2(GLdouble(0.25),GLdouble(0.25)));
  p->addTexCoordinate(vec2(GLdouble(0.25),GLdouble(0.5)));
  p->addTexCoordinate(vec2(GLdouble(0.75),GLdouble(0.5)));
  p->addTexCoordinate(vec2(GLdouble(0.75),GLdouble(0.0)));
  p->addTexCoordinate(vec2(GLdouble(0.5),GLdouble(0.0)));
  //p->setTexName("lizard.bmp");
  //p->polyLoadTexture("lizard.bmp");
  p->setColor(vec3(1.0, 0.0, 0.0));
  p->draw();
  */
  
  collection->draw();
  /*
  vector<Polygon *> faces = collection->getFaces();
  for (int i = 0; i < faces.size(); i++) {
    Polygon * currentFace = faces[i];

    if (currentFace->getTexName() != "noTexture" && currentFace->getTexName() != "") {
      string texName = currentFace->getTexName();
      currentFace->polyLoadTexture(texName);
    }
    currentFace->draw();
  }
  */
  glutSwapBuffers();
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);

  //set up the projection transformation stack to be a simple orthographic [-1, +1] projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, ((double)w / MAX(h, 1)), 1.0, 100.0);

  //set the modelview transformation stack to the identity
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void myKeyboardFunc(unsigned char key, int x, int y) {
  switch (key) {
  case 27:    //Escape key
    exit(0);
    break;
  }

  if (key == 'p') {
    if (showPrevCollection) {
      showPrevCollection = false;
      std::cout << "showPrevCollection off" << std::endl;
    } else {
      showPrevCollection = true;
      std::cout << "showPrevCollection on" << std::endl;
    }
  }
  
  if (key == 's') {
    prevCollection = new Collection(collection->getFaces(), collection->getVertices());
    collection->subDivide();
  }

}

void myActiveMotionFunc(int x, int y) {
  vec2 newMouse = vec2((double)x / glutGet(GLUT_WINDOW_WIDTH),(double)y / glutGet(GLUT_WINDOW_HEIGHT));
  //vec2 newMouse = vec2(-(double)x / glutGet(GLUT_WINDOW_WIDTH),-(double)y / glutGet(GLUT_WINDOW_HEIGHT));
  vec2 diff = (newMouse - viewport.mousePos);
  double len = diff.length();
  if (len > .001) {
    vec3 axis = vec3(diff[1]/len, diff[0]/len, 0);
    viewport.orientation = viewport.orientation * rotation3D(axis, -180 * len);
  }
  //record mouse location 
  viewport.mousePos = newMouse;
  
  glutPostRedisplay();
}

void myPassiveMotionFunc(int x, int y) {
  viewport.mousePos = vec2((double)x / glutGet(GLUT_WINDOW_WIDTH),(double)y / glutGet(GLUT_WINDOW_HEIGHT));
  
  glutPostRedisplay();
}

//initialize the environment
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

  viewport.w = 600;
  viewport.h = 600;

  if (argc < 2) {
    cout << "USAGE: go poly.obj" << endl;
    exit(1);
  }

  imgSaver = new UCB::ImageSaver("./", "go");
  collection = new Collection();
  scene = new Scene(argv[1]);
  showPrevCollection = false;

  //Create OpenGL Window
  glutInitWindowSize(viewport.w,viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow("CS184 Framework");

  //Register event handlers with OpenGL
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(myKeyboardFunc);
  glutMotionFunc(myActiveMotionFunc);
  glutPassiveMotionFunc(myPassiveMotionFunc);


  {     
    float ambient[4] = { .1f, .1f, .1f, 1.f };                                                    
    float diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.f };
    float pos[4] = { 1, 0, 0, 0 };                                                                
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);                                                    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);                                                    
    glLightfv(GL_LIGHT0, GL_POSITION, pos);                                                       
    glEnable(GL_LIGHT0);                                                                          
  }                                                                                                
                                                                                                     
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);                                      
  glEnable(GL_COLOR_MATERIAL);                                                                     
  glEnable(GL_LIGHTING);                                                                       
  glEnable(GL_DEPTH_TEST);                                                                         
  glEnable(GL_TEXTURE_2D);                                                                         
                                                                                                     
  glewInit();                                                                                      
  if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)                                      
    printf("Ready for GLSL\n");                                                          
  else {                                                                                       
    printf("No GLSL support\n");                                                         
    exit(1);                                                                             
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                                                                              
  // flatten scene!
  RenderInstance(scene->getRoot(), vec3(1,1,1), "noTexture");
  mergeVertices();
  /*
  for (int i = 0; i < collection->getFaces().size(); i++) {
    std::cout << "face: " << std::endl;
    for (int j = 0; j < collection->getFaces()[i]->getCoordinates().size(); j++) {
      std::cout << collection->getFaces()[i]->getCoordinates()[j]->getPos() << std::endl;
    }
  }
  */
  for (int i = 0; i < collection->getVertices().size(); i++) {
    std::cout << collection->getVertices()[i]->getPos() << std::endl;
  }
  std::cout << "numOfVertices: " << collection->getVertices().size() << std::endl;
  std::cout << collection->getFaces().size() << std::endl;
  glutMainLoop();
}
