/* Scene.cpp */

#include "Scene.h"
#include "SceneLoader.h"

void tab(int level) {
    while (level-- > 0)
    {
        cout << "\t";
    }
}

void PrintNode(SceneInstance *n, int level = 0) {
    tab(level); cout << "Node " << n->getName() << " {" << endl;

    if (n->getChild()) {
        SceneGroup *below = n->getChild();
        for (int i = 0; i < below->getChildCount(); i++)
        {
            PrintNode(below->getChild(i), level+1);
        }
    }

    tab(level); cout << "}" << endl;
}

void Scene::PrintScene() {
    PrintNode(getRoot());
}

Scene::Scene(string filename) {
    _loader = new SceneLoader(*this, filename);
    cout << "Loaded scene: " << endl;
    PrintScene();
}

Scene::~Scene() {
    delete _loader;
}

//Scene::addTexture(string texName, Texture texture) {
//  _textures[texName] = texture;
//}
