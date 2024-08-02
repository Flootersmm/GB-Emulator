#ifndef SHARED_H
#define SHARED_H

#include <GL/gl.h>
#include <atomic>
#include <mutex>
#include <thread>

extern std::atomic<bool> running;
extern std::mutex emu_mutex;
extern GLuint fbo;
extern GLuint fboTexture;

#endif
