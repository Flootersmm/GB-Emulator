#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

bool init_shaders(const char *vertexPath, const char *fragmentPath);
void apply_shader(GLuint fbo, GLuint texture);
void cleanup_shaders();

#endif
