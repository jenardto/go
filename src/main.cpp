/* main.cpp */

#include "main.h"

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

void display() {
  //Clear Buffers
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  glColor3f(1.0, 1.0, 1.0);
  applyMat4(viewport.orientation);
  //std::cout << "drawing polygons in scene" << std::endl;
  scene->draw();
  //std::cout << (scene->getPolys()).size() << std::endl;

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

  scene = new Scene(argv[1]);
  scene->setOpenParens();

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

  glutMainLoop();
}
