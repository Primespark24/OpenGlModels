#pragma once

#include <sb7.h>
#include <vmath.h>
#include <string>
#include <fstream>
#include <vector>

//help load obj into opengl forms

//File parsing helper
//pull first element of sub to delim
std::string parseAndClip(std::string& sub, std::string delim);

//filename -> blender obj
//vertices -> ordered vertices of object in triangles
//UVs -> Texture mapping coords
//normals
//number -> number of points in vertices
void load_obj(const char* filename, std::vector<vmath::vec4> &vertices, std::vector<vmath::vec2> &uvs, std::vector<vmath::vec4> &normals, GLuint &number);