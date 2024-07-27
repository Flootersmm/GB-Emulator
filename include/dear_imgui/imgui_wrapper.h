// imgui_wrapper.h
#ifndef IMGUI_WRAPPER_H
#define IMGUI_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gb.h"
#include <GLFW/glfw3.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

void imgui_init(GLFWwindow *window, GB *vm);
void imgui_new_frame();
void imgui_render();
void imgui_cleanup();
void toggle_button(const char *on, const char *off, bool &value);
int calculateBytesPerLine(float windowWidth, float charWidth);

#ifdef __cplusplus
}
#endif

#endif
