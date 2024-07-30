#include "dear_imgui/imgui_wrapper.h"
#include "gb.h"
#include "shared.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

std::atomic<bool> running(true);
std::mutex emu_mutex;
std::atomic<bool> step_requested(true);

void emulator_thread(GB *vm) {
  while (running) {
    {
      std::lock_guard<std::mutex> lock(emu_mutex);
      if (vm->flag.halt) {
        running = false;
        break;
      }
      if (step_requested) {
        step(vm);
        step_requested = false;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
}
int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <rom_path>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char *rom_path = argv[1];

  GLFWwindow *window = window_init();
  if (!window) {
    fprintf(stderr, "Failed to initialize GLFW window\n");
    return -1;
  }

  GB *vm = gb_init(rom_path);
  if (!vm) {
    fprintf(stderr, "Failed to initialize GB\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  imgui_init(window, vm);
  texture_init();

  // Start the emulator thread
  std::thread emu_thread(emulator_thread, vm);

  // Run the ImGui main loop
  main_loop(window, vm);

  // Cleanup
  running = false;   // Signal the emulator thread to stop
  emu_thread.join(); // Wait for the emulator thread to finish
  texture_destroy();
  gb_destroy(vm);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
