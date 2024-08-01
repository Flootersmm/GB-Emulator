#include "dear_imgui/imgui_wrapper.h"
#include "gb.h"
#include "shared.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

std::atomic<bool> running(true);
std::mutex emu_mutex;
std::atomic<bool> step_requested(true);
const double GAMEBOY_CLOCK_SPEED = 4194304.0;
const double FRAME_RATE = 59.73;
const double CYCLES_PER_FRAME = GAMEBOY_CLOCK_SPEED / FRAME_RATE;

/// Emulator thread handler for stepping and halting
void emulator_thread(GB *vm) {
  bool breakpoint_hit = false;

  while (running) {
    {
      std::lock_guard<std::mutex> lock(emu_mutex);

      if (vm->flag.halt) {
        running = false;
        break;
      }

      if (breakpoint_hit) {
        if (step_requested) {
          step(vm);
          step_requested = true;
        }
      } else {
        step(vm);

        if (vm->r.pc == 0xFFFFFF) { // Change for breakpoint
          breakpoint_hit = true;
          step_requested = false;
        }
      }
    }
    std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(
        (vm->timer_counter / GAMEBOY_CLOCK_SPEED) * 10000))); // 1000000
  }
}

/// Main function
///
/// Runs emulator in one thread, GUI in another
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

  std::thread emu_thread(emulator_thread, vm);

  main_loop(window, vm);

  running = false;
  emu_thread.join();
  texture_destroy();
  gb_destroy(vm);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
