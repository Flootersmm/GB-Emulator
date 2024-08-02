#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>

static GLuint shaderProgram;
static GLuint quadVAO, quadVBO;

GLuint compileShader(const char *source, GLenum type);
char *readFile(const char *filePath);
void setupQuad();

bool init_shaders(const char *vertexPath, const char *fragmentPath) {
  char *vertexSource = readFile(vertexPath);
  char *fragmentSource = readFile(fragmentPath);

  if (!vertexSource || !fragmentSource) {
    return false;
  }

  GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
  GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  GLint success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    return false;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  free(vertexSource);
  free(fragmentSource);

  setupQuad();

  return true;
}

void apply_shader(GLuint fbo, GLuint texture) {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(shaderProgram);

  glUniform1i(glGetUniformLocation(shaderProgram, "screenTexture"), 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);
  glUseProgram(0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void cleanup_shaders() {
  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);
  glDeleteProgram(shaderProgram);
}

GLuint compileShader(const char *source, GLenum type) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    return 0;
  }

  return shader;
}

char *readFile(const char *filePath) {
  std::ifstream file(filePath, std::ios::in | std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open file: " << filePath << std::endl;
    return nullptr;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  std::string str = buffer.str();
  char *result = (char *)malloc(str.size() + 1);
  strcpy(result, str.c_str());

  return result;
}

void setupQuad() {
  float quadVertices[] = {// positions     // texCoords
                          -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
                          1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
                          -1.0f, 1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f, 1.0f};

  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);

  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));

  glBindVertexArray(0);
}
