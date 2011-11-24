#include "SceneLoader.h"

SceneLoader::SceneLoader(Scene &scene, string file) {
    err = &cout;
    scene._root = new SceneInstance();
    root = scene._root;
    root->_name = "toplevel";
    buildScene(file);
}

SceneLoader::~SceneLoader() {
    for (map<string, SceneGroup*>::iterator it = groups.begin(); it != groups.end(); ++it) {
      delete it->second;
    }
    for (vector<SceneInstance*>::iterator it = instances.begin(); it != instances.end(); ++it) {
      delete *it;
    }
}

void SceneLoader::buildEndlineTable(string filename) {
  ifstream file(filename.c_str());
  string line;
  while (getline( file, line ))
    endlineTable.push_back(file.tellg());
  endlineTable.push_back(file.tellg());
}

void SceneLoader::curPos(ostream &out, int g) {
  int line = 1;
  int lastg = 0;
  for (vector<int>::iterator it = endlineTable.begin(); it != endlineTable.end(); ++it) {
    if (*it > g) {
      out << "line: " << line << " char: " << (g - lastg - 1);
      return;
    }
    lastg = *it;
    line ++;
  }
  out << "line: " << line << " char: " << (g - lastg);
}

string SceneLoader::getString(istream &str) {
  string ret;
  char temp;
  str >> temp; // get rid of white space, get to first char
  str.putback(temp);
  while (str.get(temp)) {
    if (temp >= 'a' && temp <= 'z'
	|| temp >= 'A' && temp <= 'Z'
	|| temp >= '0' && temp <= '9'
	|| temp == '_' )
      {
	ret += temp;
      }
    else if (temp == '#') {
      string no;
      getline(str, no);
      if (ret.empty())
	return getString(str);
      else
	return ret;
    } else {
      str.putback(temp);
      break;
    }
  }
  return ret;
}

string SceneLoader::getQuoted(istream &str) {
  string ret;
  char temp;
  str >> temp;
  if (temp != '"') {
    *err << "expected opening \" at ";
    curPos(*err, str.tellg());
    *err << endl;
  }
  getline(str, ret, '"');
  return ret;
}

bool SceneLoader::readCommand(istream &str, string &name) {
  name = getString(str);
  if (name.empty()) {
    *err << "error: expected command but did not find one at ";
    curPos(*err, str.tellg());
    *err << endl;
    return false;
  }
  return true;
}

bool SceneLoader::findOpenParen(istream &str) {
  char temp;
  string line;
  while (str >> temp) {
    if (temp == '(')
      return true;
    else if (temp == '#')
      getline(str,line);
    else {
      *err << "error: unexpected char " << temp << " at ";
      curPos(*err, str.tellg());
      *err << endl;
      return false;
    }
  }
  return false;
}

enum { OPEN, CLOSED, ERROR };  // possible types of paren, used by the findOpenOrClosedParen function below
int SceneLoader::findOpenOrClosedParen(istream &str) {
  char temp;
  string line;
  while (str >> temp) {
    if (temp == '(')
      return OPEN;
    else if (temp == '#')
      getline(str,line);
    else if (temp == ')') {
      str.putback(temp);
      return CLOSED;
    }
    else {
      *err << "error: unexpected char " << temp << " at ";
      curPos(*err, str.tellg());
      *err << endl;
      return false;
    }
  }
  return ERROR;
}

bool SceneLoader::findCloseParen(istream &str) {
  char temp;
  string line;
  int close = 0;
  while (str >> temp) {
    if (temp == '#')
      getline(str, line);
    if (temp == '(')
      close++;
    if (temp == ')')
      close--;
    if (close < 0)
      return true;
  }
  return false;
}


int SceneLoader::getValues(istream &str, vector<ParametricValue*> &vals)
{
    ParametricValue *val;
    while ( (val = getValue(str)) != NULL )
    {
        vals.push_back(val);
    }
    return int(vals.size());
}

ParametricValue* SceneLoader::getValue(istream &str)
{
    char temp;
    str >> temp;
    if (temp == ')')
    {
        str.putback(temp);
        return NULL;
    }
    else if (temp == '#')
    {
        string no;
        getline(str,no);
        return getValue(str);
    }
    else
    {
        str.putback(temp);

        double val;
        str.peek();
        str >> val;
        if (str.fail())
        {
            str.clear();
            *err << "Failed to extract a numeric value at ";
            curPos(*err, str.tellg());
            *err << endl;

            str.peek();
            return NULL;
        } else {
            return new ConstValue(val);
        }
    }
}


bool SceneLoader::doInclude(istream &str, string& name) {
  name = getString(str);
  if (name.empty()) {
    *err << "Couldn't read include name at "; errLine(str.tellg());
    return false;
  }

  if (groups[name] != NULL) {
    *err << "Illegal re-use of variable name \"" << name << "\" at ";
    curPos(*err,str.tellg());
    *err << endl;
    return false;
  }

  string file = getQuoted(str);
  SceneGroup *n = new SceneGroup();
  n->_poly = new Polygon(file);
  n->_name = name;
  groups[name] = n;
  return true;
}


bool SceneLoader::doSurface(istream &str, string& name) {
  name = getString(str);
  if (name.empty()) {
    *err << "Couldn't read surface name at "; errLine(str.tellg());
    return false;
  }

  if (_savedColors[name] != NULL) {
    *err << "Illegal re-use of surface name \"" << name << "\" at ";
    curPos(*err,str.tellg());
    *err << endl;
    return false;
  }

  do {
    int state = findOpenOrClosedParen(str);
    if (state == ERROR)
      return false;
    else if (state == CLOSED)
      return true;
    else if (state == OPEN) {
      string cmd;
      vector<ParametricValue*> values;
      if (readCommand(str, cmd)) {
	if (cmd == "rgb") {
	  int numv = getValues(str, values);
	  if (numv < 3) {
	    *err << "rgb with not enough args at "; errLine(str.tellg());
	  } else {
	    cleanAfter(values, 3);
	    Color *c = new Color();
	    for (int i = 0; i < 3; i++) {
	      c->_color[i] = values[i];
	    }
	    _savedColors[name] = c;
	  }
	} else if (cmd == "bitmap") {
	  // something with loading textures
	  string file = getQuoted(str);
	  std::cout << file << std::endl;
	  _savedTextures[name] = file;
	}
	findCloseParen(str);
      }
    }
  } while (true);
}

void SceneLoader::errLine(int at) {
  curPos(*err, at);
  *err << endl;
}

void SceneLoader::cleanAfter(vector<ParametricValue*> &vals, unsigned int i) {
  for (;i<vals.size(); i++) {
    delete vals[i];
  }
}


bool SceneLoader::doM(istream &str, string &name) {
  name = getString(str);
  if (name.empty())
    return false;

  if (groups[name] != NULL) {
    *err << "Illegal re-use of group name \"" << name << "\" at ";
    curPos(*err,str.tellg());
    *err << endl;
    return false;
  }

  SceneGroup *n = new SceneGroup();
  n->_poly = new Polygon();
  n->_name = name;
  groups[name] = n;

  vector<Vertex> temp;
  
  do {
    int state = findOpenOrClosedParen(str);
    if (state == ERROR)
      return false;
    else if (state == CLOSED)
      return true;
    else if (state == OPEN) {
      string cmd;
      vector<ParametricValue*> values;
      if (readCommand(str, cmd)) {
	if (cmd == "v") {
	  int numv = getValues(str, values);
          if (numv < 1) {
            *err << "v with no args at "; errLine(str.tellg());
          } else if (numv < 3) {
	    *err << "v with not enough args at "; errLine(str.tellg());
	  } else {
            cleanAfter(values, 3);
	    double x, y, z;
	    x = values[0]->getValue();
	    y = values[1]->getValue();
	    z = values[2]->getValue();
	    temp.push_back(Vertex(x, y, z));
          }
	} else if (cmd == "f") {
	  bool texFlag = false;
	  str.get(); // get rid of white space
	  char nextChar = str.peek();
	  if (!((nextChar >= '0') && (nextChar <= '9'))) {
	    string fcmd;
	    findOpenParen(str);
	    if (readCommand(str, fcmd)) {

	      if (fcmd == "surf") {
		//load the texture
		string textureName;
		textureName = getString(str);
		texFlag = true;
		std::cout << "texture accquired: " + _savedTextures[textureName] << std::endl;
		n->_poly->setTexName(_savedTextures[textureName]);
	      }
	    }
	  }
	  if (!texFlag) {
	    n->_poly->setTexName("noTexture");
	  }
	      string line;
	      getline(str, line);

	      stringstream ss(stringstream::in | stringstream::out);
	      ss.str(line);
	      while (!ss.eof()) {
		string elem;
		ss >> elem;
		if (elem == ")") {
		  continue;
		}

		string tempVertIndex = "";
		int elemIndex;
		for (elemIndex = 0; elemIndex < elem.size(); elemIndex++) {
		  if (elem[elemIndex] == '(') {
		    break;
		  } else {
		    tempVertIndex = tempVertIndex + elem[elemIndex];
		  }
		}

		size_t endIndex = elem.size() - elemIndex - 2;
		string texCoords = elem.substr(elemIndex + 1, endIndex);

		endIndex = texCoords.find(",");
		string sTempVal = "";
		string tTempVal = "";
		for (int i = 0; i < texCoords.size(); i++) {
		  if (texCoords[i] == ',' ) {
		    endIndex = texCoords.size() - i - 1;
		    tTempVal = texCoords.substr(i + 1, endIndex);
		    break;
		  }
		  sTempVal = sTempVal + texCoords[i];
		}

		char s[sizeof(sTempVal)];
		char t[sizeof(tTempVal)];
		for (int i = 0; i < sTempVal.size(); i++) {
		  s[i] = sTempVal[i];
		}
		for (int i = 0; i < tTempVal.size(); i++) {
		  t[i] = tTempVal[i];
		}

		double sVal = atof(s);
		double tVal = atof(t);

		vec2 textureCoord = vec2(sVal, tVal);

		char tVIndex[sizeof(tempVertIndex)];
		for (int i = 0; i < tempVertIndex.size(); i++) {
		  tVIndex[i] = tempVertIndex[i];
		}

		int vertIndex = atoi(tVIndex) - 1;

		std::cout << "textureCoord: " << textureCoord << std::endl;
		std::cout << "vertIndex: " << temp[vertIndex].getPos() << std::endl;
		n->_poly->addTexCoordinate(textureCoord);
		temp[vertIndex].setTextureCoord(textureCoord);
		n->_poly->addVertex(&temp[vertIndex]);
	      }
	      str.putback(')');
	}
	/*else {
	      *err << "invalid args for f"; errLine(str.tellg());
	    }
	  } else {   
	    int numv = getValues(str, values);
	    if (numv < 1) {
	      *err << "f with no args at "; errLine(str.tellg());
	    } else if (numv < 3) {
	      *err << "f with not enough args at "; errLine(str.tellg());
	    } else {
	      for (int i = 0; i < numv; i++) {
		int vertIndex = values[i]->getValue() - 1;
		std::cout << temp[vertIndex].getPos() << std::endl;
		n->_poly->addVertex(&temp[vertIndex]);
	      }
	    }
	  }
	*/
      
	findCloseParen(str);
      }
    }
  } while (true);
}

SceneInstance* SceneLoader::doI(istream &str, string &name) {
  name = getString(str);
  if (name.empty()) {
    *err << "Couldn't read instance name at "; errLine(str.tellg());
    return NULL;
  }

  string var = getString(str);
  if (groups[var] == NULL) {
    *err << "Instancing node " << var << " which doesn't exist yet at ";
    curPos(*err,str.tellg());
    *err << endl;
    return NULL;
  }

  SceneInstance *n = new SceneInstance();
  instances.push_back(n); //nodes[name] = n;
  n->_name = name;
  n->_child = groups[var];
  
  if (var == "square") {
    std::cout << "im a square: " + n->_child->getPolygon()->getTexName() << std::endl;
  }

  do {
    int state = findOpenOrClosedParen(str);
    if (state == ERROR)
      return NULL;
    else if (state == CLOSED)
      return n;
    else if (state == OPEN) {
      string cmd;
      vector<ParametricValue*> values;
      if (readCommand(str, cmd)) {
	if (cmd == "rot") {
	  int numv = getValues(str, values);
	  Rotate *r = NULL;
	  if (numv < 1) {
	    *err << "rot with no args at "; errLine(str.tellg());
	  } else if (numv < 3) {
	    cleanAfter(values, 1);
	    r = new Rotate();
	    r->angle = values[0];
	  } else {
	    cleanAfter(values, 4);
	    r = new Rotate();
	    r->angle = values[0];
	    for (int i = 0; i < 3; i++)
	      r->axis[i] = values[i+1];
	  }
	  if (r != NULL)
	    n->_transforms.push_back(r);
	}
	else if (cmd == "t") {
	  int numv = getValues(str, values);
	  Translate *t = NULL;
	  if (numv < 2) {
	    *err << "t with too few parameters at "; errLine(str.tellg());
	  } else if (numv == 2) {
	    t = new Translate();
	    for (int i = 0; i < 2; i++)
	      t->translate[i] = values[i];
	  } else {
	    cleanAfter(values, 3);
	    t = new Translate();
	    std::cout << "start: " << std::endl;
	    for (int i = 0; i < 3; i++) {
	      t->translate[i] = values[i];
	    }
	  }
	  if (t != NULL)
	    n->_transforms.push_back(t);
	}
	else if (cmd == "s") {
	  int numv = getValues(str, values);
	  Scale *s = NULL;
	  if (numv < 2) {
	    *err << "S with too few parameters at "; errLine(str.tellg());
	  } else if (numv == 2) {
	    s = new Scale();
	    for (int i = 0; i < 2; i++)
	      s->scale[i] = values[i];
	  } else {
	    s = new Scale();
	    for (int i = 0; i < 3; i++)
	      s->scale[i] = values[i];
	  }
	  if (s != NULL)
	    n->_transforms.push_back(s);
	}
	else if (cmd == "surf") {
	  string surfRef;
	  if (readCommand(str, surfRef)) {
	    n->_texName =_savedTextures[surfRef];
	    std::cout << "replaced texture with: " << n->_texName << std::endl;
	  }
	}
	else if (cmd == "color") {
	 
	  bool lookup = false;
	  Color *c = new Color();
	  str.get(); // get rid of white space
	  char nextChar = str.peek();
	  if (!((nextChar >= '0') && (nextChar <= '9'))) {
	    lookup = true;
	  }
	  if (lookup) {
	    string colorRef;
	    if (readCommand(str, colorRef)) {
	      std::cout << colorRef << std::endl;
	      Color *temp = _savedColors[colorRef];
	      for (int i = 0; i < 3; i++) {
		c->_color[i] = temp->_color[i];
	      }
	    }
	  } else {
	    int numv = getValues(str, values);
	    if (numv < 3) {
	      *err << "color with too few parameters at "; errLine(str.tellg());
	    } else {
	      cleanAfter(values, 3);
	      //c = new Color();
	      for (int i = 0; i < 3; i++)
		c->_color[i] = values[i];
	    }
	  }
	  if (c != NULL) {
	    n->_color = c;
	    n->_texName = "noTexture";
	    std::cout << "replaced with color, texture set to noTexture" << std::endl;
	  }
	}
	else {
	  *err << "Error: command " << cmd << " not recognized at ";
	  curPos(*err,str.tellg());
	  *err << endl;
	}
	findCloseParen(str);
      }
    }
  } while (true);
}

bool SceneLoader::doG(istream &str, string &name) {
  name = getString(str);
  if (name.empty())
    return false;

  if (groups[name] != NULL) {
    *err << "Illegal re-use of group name \"" << name << "\" at ";
    curPos(*err,str.tellg());
    *err << endl;
    return false;
  }
  
  SceneGroup *n = new SceneGroup();
  groups[name] = n;
  n->_name = name;

  do {
    int state = findOpenOrClosedParen(str);
    if (state == ERROR)
      return false;
    else if (state == CLOSED)
      return true;
    else if (state == OPEN) {
      string cmd;
      if (readCommand(str, cmd)) {
	if (cmd != "i") {
	  *err << "Command other than i from g at ";
	  curPos(*err,str.tellg());
	  *err << endl;
	}
	string iname;
	SceneInstance *newNode;
	if ((newNode = doI(str,iname)) != NULL) {
	  n->_children.push_back( newNode );
	}
	findCloseParen(str);
      }
    }
  } while (true);
}


bool SceneLoader::doRender(istream &str, string &name)
{
    name = getString(str);
    if (name.empty())
    {
        *err << "Trying to render group without specifying a name at ";
        curPos(*err,str.tellg());
        *err << endl;
        return false;
    }

    if (groups[name] == NULL)
    {
        *err << "Trying to render group not found \"" << name << "\" at ";
        curPos(*err,str.tellg());
        *err << endl;
        return false;
    }

    root->_child = groups[name];

    return true;
}

bool SceneLoader::buildScene(string filename)
{
  buildEndlineTable(filename);
  
  ifstream file(filename.c_str());
  string line;
  int lastPos = 0;
  while (findOpenParen(file)) {
    file.tellg();
    if (readCommand(file, line)) {
      if (line == "include") {
	string instName;
	if (doInclude(file, instName)) {
	
	} else {
	  cout << "mangled include at ";
	  curPos(cout, file.tellg());
	  cout << endl;
	}
      } else if (line == "surface") {
	string surfaceName;
	if (doSurface(file, surfaceName)) {
	  cout << "got surface named " << surfaceName << endl;
	}
      } else if (line == "m") {
	string iname;
	if (doM(file, iname)) {
	  cout << "got a mesh named " << iname << endl;
	}
      } else if (line == "i") {
	*err << "Error: Instance commands must belong to a group, but I found in global scope at "; errLine(file.tellg());
      } else if (line == "g") {
	string iname;
	if (doG(file, iname)) {
	  cout << "got a group named " << iname << endl;
	}
      } else if (line == "render") {
	string iname;
	if (doRender(file, iname)) {
	  cout << "did render " << iname << endl;
	}
      } else {
	*err << "command not recognized: " << line << endl;
      }
      findCloseParen(file);
    } else {

    }
    lastPos = file.tellg();
  }
  return true;
}
