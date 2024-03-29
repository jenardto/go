/*
 * SceneLoader.h
 *
 *  Created on: Feb 8, 2009
 *      Author: jima
 */

#ifndef SCENE_LOADER_H_
#define SCENE_LOADER_H_

#include "Scene.h"
#include <map>
#include <vector>

using namespace std;

class SceneLoader { // this is basically like an elaborate constructor for Scene

public:
    SceneLoader(Scene &scene, string file);
    ~SceneLoader();

private:
    // make these private; they shouldn't be called
    SceneLoader(const SceneLoader &copy);
    SceneLoader& operator =(const SceneLoader& scene);

    // record locations of newline, to make it easy to convert a position in the file to a line number
    vector<int> endlineTable; 

    // map from group names to groups, to facilitate lookup when parsing
    map<string, SceneGroup*> groups;
    // all the instances in the file
    vector<SceneInstance*> instances;
    // map from color identifier to rgb values
    map<string, Color*> _savedColors;
    // map from surface name to texture file name used for loading
    map<string, string> _savedTextures;
    // a stream for error messages, usually cout
    ostream *err;

    // the top level
    SceneInstance *root;

    /* helper functions */
    void buildEndlineTable(string filename); // preprocess a file to get the endline table
    void curPos(ostream &out, int g); // convert position in file to line number
    void errLine(int at); // write the line number of position 'at' to the error stream
    void cleanAfter(vector<ParametricValue*> &vals, unsigned int i); // free memory from a parametric values vector (cleans up after getValues)

    /* functions to facilitated reading the file stream */
    string getString(istream &str); // extract the next string of characters (contiguous letters, numbers, and _s)
    string getQuoted(istream &str); // extract the next quoted string
    bool readCommand(istream &str, string &name); // extract the next command (ie string after an open paren)
    bool findOpenParen(istream &str); // extract characters until we find an open paren
    int findOpenOrClosedParen(istream &str); // extract until we find an opening or a closing paren
    bool findCloseParen(istream &str); // extract until we find one more closing paren than opening parens (eg current expression is finished)
    ParametricValue* getValue(istream &str); // extract one numeric value
    int getValues(istream &str, vector<ParametricValue*> &vals); // extract numeric values until we can't find any more, return amount found

    /* functions for processing each top-level command type */
    bool doInclude(istream &str, string& name); // process an include command
    bool doSurface(istream &str, string& name); // process a surface command
    bool doM(istream &str, string& name); // process a m (mesh) command
    SceneInstance* doI(istream &str, string &name); // process an i (instance) command
    bool doG(istream &str, string &name); // process a g (group) command
    bool doRender(istream &str, string &name); // process a render command

    /* the main loading function */
    bool buildScene(string filename); 
};

#endif /* SCENE_LOADER_H_ */
