#include "dear_imgui/imgui.h"
#include "dear_imgui/imgui_wrapper.h"
#include "gb.h"
#include <GLFW/glfw3.h>
#include <ctype.h>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <rom_path>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char *rom_path = argv[1];

  GLFWwindow *window = window_init();
  GB *vm = gb_init(rom_path);
  imgui_init(window, vm);
  texture_init();

  main_loop(window, vm);

  texture_destroy();
  gb_destroy(vm);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
