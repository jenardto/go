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
Vertex * tempVertex;
int button, state, xOrigin, yOrigin;
UCB::ImageSaver * imgSaver;

void applyMat4(mat4 &m) {
  double glmat[16];
  int idx = 0;
  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      glmat[idx++] = m[i][j];
  glMultMatrixd(glmat);
}

void setupView() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,-3);
  applyMat4(viewport.orientation);
}


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
      Polygon * newPolygon = new Polygon();

      newPolygon->setTexName(texture);
      newPolygon->setColor(color);
      
      vector<vec2> texCoords = child->getPolygon()->getTexCoordinates();
      for (int i = 0; i < texCoords.size(); i++) {
	newPolygon->addTexCoordinate(texCoords[i]);
      }

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
	newPolygon->addVertex(newVert);
	collection->addVertex(newVert);
      }
      centroidX = centroidX/numVerts;
      centroidY = centroidY/numVerts;
      centroidZ = centroidZ/numVerts;
      Vertex * cent = new Vertex(centroidX, centroidY, centroidZ);
      newPolygon->addCentroid(cent);
      collection->addFace(newPolygon);
    }
    
    // goes through all children                                                  
    for (int i = 0; i < child->getChildCount(); i++) {
      RenderInstance(child->getChild(i), color, texture);
    }
  }
  glPopMatrix();
}



void display() {
  //Clear Buffers
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  glColor3f(1.0, 1.0, 1.0);
  applyMat4(viewport.orientation);
  
  /*
  Polygon * p = new Polygon();
  p->addVertex(new Vertex(0.25, 0.25, 0.0));
  p->addVertex(new Vertex(0.25, -0.25, 0.0));
  p->addVertex(new Vertex(-0.25, -0.25, 0.0));
  p->addVertex(new Vertex(-0.25, 0.25, 0.0));
  p->addTexCoordinate(vec2(GLdouble(0.0),GLdouble(0.0)));
  p->addTexCoordinate(vec2(GLdouble(1.0),GLdouble(0.0)));
  p->addTexCoordinate(vec2(GLdouble(1.0),GLdouble(1.0)));
  p->addTexCoordinate(vec2(GLdouble(0.0),GLdouble(1.0)));
  p->setTexName("lizard.bmp");
  p->polyLoadTexture("lizard.bmp");
  //p->setColor(vec3(1.0, 0.0, 0.0));
  p->draw();
  */
  
  //collection->draw();

  vector<Polygon *> faces = collection->getFaces();
  for (int i = 0; i < faces.size(); i++) {
    Polygon * currentFace = faces[i];

    if (currentFace->getTexName() != "noTexture" && currentFace->getTexName() != "") {
      string texName = currentFace->getTexName();
      currentFace->polyLoadTexture(texName);
    }
    currentFace->draw();
  }

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
}

void myActiveMotionFunc(int x, int y) {
  vec2 newMouse = vec2((double)x / glutGet(GLUT_WINDOW_WIDTH),(double)y / glutGet(GLUT_WINDOW_HEIGHT));
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
    float pos[4] = { 0, 0, 1, 0 };                                                                
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

  glutMainLoop();
}
