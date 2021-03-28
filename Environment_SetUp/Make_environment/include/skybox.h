/*
* Skybox Utility
* Comes From Scott Griffith's code small changes made by Brycen Martin
*
* Help load and set up skycube
*/

#pragma once  //use only once

#include <sb7.h>
#include <vector>
#include <string>
#include <vmath.h>

//fill soon to be vertex list to represent a cube
//centered on (0,0,0)
void createCube(std::vector<vmath::vec4> &vertices);

//fill vertices array with 0 1 2 3 to represent cube corners 0 1 2 3
void subPoints(int a, int b, int c, int d, std::vector<vmath::vec4> &vertices);

//Define points of cube
const vmath::vec4 CUBE_VERTICES[] = {
    vmath::vec4(-0.5, -0.5,  0.5, 1.0),
    vmath::vec4(-0.5,  0.5,  0.5, 1.0),
    vmath::vec4( 0.5,  0.5,  0.5, 1.0),
    vmath::vec4( 0.5, -0.5,  0.5, 1.0),
    vmath::vec4(-0.5, -0.5, -0.5, 1.0),
    vmath::vec4(-0.5,  0.5, -0.5, 1.0),
    vmath::vec4( 0.5,  0.5, -0.5, 1.0),
    vmath::vec4( 0.5, -0.5, -0.5, 1.0)
};

//Load textures
void loadCubeTextures(std::string directory, GLuint texture_ID);

//helps load textures and specific texture mapping
void loadCubeSide(GLint texture_ID, GLenum side, std::string file);

//convert char to unsigned int
unsigned int charToUInt(char * loc);