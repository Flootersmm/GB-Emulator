#include "dear_imgui/imgui.h"
#include "dear_imgui/imgui_wrapper.h"
#include "gb.h"
#include <ctype.h>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>

char stdout_buffer[4096]; ///< Buffer for stdout redirection
FILE *stdout_memstream;   ///< Space for stdout redirection

// IJVM
char *binary_path = NULL; ///< Path to IJVM binary
bool program_started =
    false; ///< True if we've started stepping through IJVM binary
bool toggle_info = false;      ///< Show stack state
bool toggle_backtrace = false; ///< Show callstack backtrace
bool toggle_file = false;      ///< Show IJVM binary
int prev_bp = -1;              ///< Previous breakpoint

// IMGUI
bool gui_active = true;               ///< True if the GUI is active
bool gui_stack_active = false;        ///< True if the stack window is active
bool gui_file_active = false;         ///< True if the file window is active
float fontScale = 1;                  ///< Global font scale
float border_size = 3.0;              ///< Border size between windows
ImVec2 window_main_size(0.0f, 0.0f);  ///< Main window size
ImVec2 window_menu_size(0.0f, 0.0f);  ///< Menu window size
ImVec2 window_stack_size(0.0f, 0.0f); ///< Stack window size
ImVec2 window_file_size(0.0f, 0.0f);  ///< File window size
ImVec2 window_shell_size(0.0f, 0.0f); ///< Shell window size
std::map<std::string, ImVec4>
    color_map = ///< String, ImVec4 map of term colours to ImGui colours
    {{MAG, ImVec4(0.698f, 0.569f, 0.847f, 1.0f)},
     {RESET, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)}};

GLFWwindow *window_init() {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialise GLFW\n");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(1280, 720, "GB Emulator", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  return window;
}

static GLuint texture;
static int cycles = 0;
static const int updateInterval =
    15;                 // Update texture every 15 frames (1/4 second at 60 FPS)
static int width = 160; // Example width
static int height = 144; // Example height
static unsigned char *pixels = new unsigned char[width * height * 4]; // RGBA
static bool isBlack = true; // Toggle state for alternating colors
static float scale = 2.0f;

void update_texture() {
  // Set all pixels to white
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int offset = (y * width + x) * 4;
      pixels[offset] = 255;
      pixels[offset + 1] = 255;
      pixels[offset + 2] = 255;
      pixels[offset + 3] = 255; // Alpha channel
    }
  }

  // Draw the red border
  for (int x = 0; x < width; x++) {
    // Top border
    int offsetTop = x * 4;
    pixels[offsetTop] = 255;
    pixels[offsetTop + 1] = 0;
    pixels[offsetTop + 2] = 0;
    pixels[offsetTop + 3] = 255;

    // Bottom border
    int offsetBottom = ((height - 1) * width + x) * 4;
    pixels[offsetBottom] = 255;
    pixels[offsetBottom + 1] = 0;
    pixels[offsetBottom + 2] = 0;
    pixels[offsetBottom + 3] = 255;
  }

  for (int y = 0; y < height; y++) {
    // Left border
    int offsetLeft = (y * width) * 4;
    pixels[offsetLeft] = 255;
    pixels[offsetLeft + 1] = 0;
    pixels[offsetLeft + 2] = 0;
    pixels[offsetLeft + 3] = 255;

    // Right border
    int offsetRight = (y * width + (width - 1)) * 4;
    pixels[offsetRight] = 255;
    pixels[offsetRight + 1] = 0;
    pixels[offsetRight + 2] = 0;
    pixels[offsetRight + 3] = 255;
  }

  // Draw a checkerboard pattern
  for (int y = 1; y < height - 1; y++) {
    for (int x = 1; x < width - 1; x++) {
      int offset = (y * width + x) * 4;
      if ((x % 2) == (y % 2)) {
        // Black square
        pixels[offset] = 0;
        pixels[offset + 1] = 0;
        pixels[offset + 2] = 0;
        pixels[offset + 3] = 255;
      } else {
        // White square (unnecessary since we already set the background to
        // white, but added for clarity)
        pixels[offset] = 255;
        pixels[offset + 1] = 255;
        pixels[offset + 2] = 255;
        pixels[offset + 3] = 255;
      }
    }
  }

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA,
                  GL_UNSIGNED_BYTE, pixels);
}

void draw_window_texture() {
  // Calculate the size of the texture and the margin
  ImVec2 texture_size = ImVec2((float)width * scale, (float)height * scale);
  ImVec2 margin = ImVec2(texture_size.x * 0.05f, texture_size.y * 0.05f);
  float title_bar_height = ImGui::GetFrameHeightWithSpacing();
  ImVec2 window_size = ImVec2(texture_size.x + margin.x * 2,
                              texture_size.y + margin.y * 2 + title_bar_height);

  // Set the window size to include the margin
  ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);

  // Begin a new window with the specified size
  ImGui::Begin("Texture Window", NULL,
               ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoScrollWithMouse |
                   ImGuiWindowFlags_NoResize);

  // Remove padding and add margin
  ImGui::SetCursorPos(ImVec2(margin.x, margin.y + title_bar_height));

  // Draw the texture scaled to the window size
  ImGui::Image((void *)(intptr_t)texture, texture_size);

  ImGui::End();
}

void draw_window_crude_debug(GB *vm) {
  ImVec2 window_main_size = ImGui::GetWindowSize();

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(500.0f, window_main_size.y * 1.5f));
  ImGui::Begin("GB Emulator", &gui_active, ImGuiWindowFlags_MenuBar);

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open..", "Ctrl+O")) {
        // TODO
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) {
        // TODO
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  if (ImGui::TreeNode("ROM Info")) {

    ImGui::Text("rom_path: %s", vm->rom_path);
    ImGui::Text("mem.size: %u", vm->mem.size);
    ImGui::Text("Cart Size: %u bytes", vm->cart.size);

    ImGui::Text("Title:");
    ImGui::BeginGroup();
    for (int i = 0; i < 16; i++) {
      char display_char = vm->cart.title[i] == 0 ? '_' : vm->cart.title[i];
      ImGui::SameLine();
      ImGui::Text(&display_char);
    }
    ImGui::EndGroup();

    ImGui::Text("Manufacturer Code: %02X %02X %02X",
                vm->cart.manufacturer_code[0], vm->cart.manufacturer_code[1],
                vm->cart.manufacturer_code[2]);

    switch (vm->flag.cgb) {
    case NON_CGB:
      ImGui::Text("CGB Flag: NON_CGB");
      break;
    case CGB_ENHANCED:
      ImGui::Text("CGB Flag: CGB_ENHANCED");
      break;
    case CGB_ONLY:
      ImGui::Text("CGB Flag: CGB_ONLY");
      break;
    default:
      ImGui::Text("CGB Flag: Unknown CGB flag");
      break;
    }

    ImGui::Text("Licensee: %s", licensee_get_name(vm));

    ImGui::Text("SGB Flag: %s", vm->cart.sgb_flag ? "true" : "false");

    ImGui::Text("Cartridge Type: %s",
                cart_get_type_str(vm->cart.cartridge_enum));

    ImGui::Text("Cart ROM Size: %s", rom_get_size_str(vm));

    ImGui::Text("Cart RAM Size: %s", ram_get_size_str(vm));

    ImGui::Text("Destination: %s", destination_code_get_str(vm));

    ImGui::Text("Mask ROM Version Number: %02X", vm->cart.mask_rom_ver_number);

    ImGui::Text("Header Checksum: %02X", vm->cart.header_checksum);
    ImGui::SameLine();
    if ((!cart_header_checksum_calc(vm)) == (vm->cart.header_checksum)) {
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Failed");
    } else {
      ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Passed");
    }

    ImGui::Text("Global Checksum: %02X %02X", vm->cart.global_checksum[0],
                vm->cart.global_checksum[1]);
    ImGui::SameLine();
    if ((!cart_global_checksum_calc(vm)) ==
        ((vm->cart.global_checksum[0] << 8) | vm->cart.global_checksum[1])) {
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Failed");
    } else {
      ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Passed");
    }

    ImGui::Text("Cartridge Logo:");
    ImGui::NewLine();
    ImGui::BeginGroup();
    for (u32 i = 0x0104; i < 0x0134; i++) {
      if (i != 0x0104 && (i - 0x0104) % 16 == 0) {
        ImGui::TextWrapped("");
      }
      ImGui::SameLine();
      ImGui::Text("%02X", vm->cart.data[i]);
    }
    ImGui::SameLine();
    if (!vm->flag.logo_match) {
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Failed");
    } else {
      ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Passed");
    }

    ImGui::EndGroup();

    ImGui::Text("vram.item: %p", vm->vram.item);
    ImGui::Text("vram.size: %u", vm->vram.size);

    ImGui::TreePop();
  }

  if (ImGui::TreeNode("Registers")) {
    ImGui::Text("A: $%02X", vm->r.a);
    ImGui::Text("B: $%02X", vm->r.b);
    ImGui::Text("C: $%02X", vm->r.c);
    ImGui::Text("D: $%02X", vm->r.d);
    ImGui::Text("E: $%02X", vm->r.e);
    ImGui::Text("F: $%02X", vm->r.f);
    ImGui::Text("H: $%02X", vm->r.h);
    ImGui::Text("AF: $%04X", vm->r.af);
    ImGui::Text("BC: $%04X", vm->r.bc);
    ImGui::Text("DE: $%04X", vm->r.de);
    ImGui::Text("HL: $%04X", vm->r.hl);
    ImGui::Text("PC: $%04X", vm->r.pc);
    ImGui::Text("SP: $%04X", vm->r.sp);

    ImGui::TreePop();
  }

  if (ImGui::TreeNode("Initial Memory Locs")) {
    const char *addresses[] = {
        "[$FF05]", "[$FF06]", "[$FF07]", "[$FF10]", "[$FF11]", "[$FF12]",
        "[$FF14]", "[$FF16]", "[$FF17]", "[$FF19]", "[$FF1A]", "[$FF1B]",
        "[$FF1C]", "[$FF1E]", "[$FF20]", "[$FF21]", "[$FF22]", "[$FF23]",
        "[$FF24]", "[$FF25]", "[$FF26]", "[$FF40]", "[$FF42]", "[$FF43]",
        "[$FF45]", "[$FF47]", "[$FF48]", "[$FF49]", "[$FF4A]", "[$FF4B]",
        "[$FFFF]"};

    const uint16_t locations[] = {
        0xFF05, 0xFF06, 0xFF07, 0xFF10, 0xFF11, 0xFF12, 0xFF14, 0xFF16,
        0xFF17, 0xFF19, 0xFF1A, 0xFF1B, 0xFF1C, 0xFF1E, 0xFF20, 0xFF21,
        0xFF22, 0xFF23, 0xFF24, 0xFF25, 0xFF26, 0xFF40, 0xFF42, 0xFF43,
        0xFF45, 0xFF47, 0xFF48, 0xFF49, 0xFF4A, 0xFF4B, 0xFFFF};

    const int numAddresses = sizeof(addresses) / sizeof(addresses[0]);
    const int numColumns =
        4; // Adjust this value to change the number of columns in the grid
    const int numRows = (numAddresses + numColumns - 1) / numColumns;
    const float rowHeight = ImGui::GetTextLineHeightWithSpacing();
    const float windowHeight = rowHeight * (numRows + 1);

    ImGui::BeginChild("MemoryGrid", ImVec2(0, windowHeight), true);

    for (int i = 0; i < numAddresses; i += numColumns) {
      for (int j = 0; j < numColumns; ++j) {
        if (i + j < numAddresses) {
          ImGui::Text("%s: $%02X", addresses[i + j],
                      vm->mem.io_registers[locations[i + j] - 0xFF00]);
          if (j < numColumns - 1)
            ImGui::SameLine();
        }
      }
    }

    ImGui::EndChild();
    ImGui::TreePop();
  }
  ImGui::SetCursorPosY((ImGui::GetWindowSize().y) - ImGui::GetFontSize() * 4);
  ImGui::SliderFloat("Font scale", &fontScale, 1.0f, 3.0f, "%.1f");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

  ImGui::SliderFloat("GB Scale", &scale, 1.0f, 6.0f, "%1.0f");
  ImGui::GetIO().FontGlobalScale = fontScale;

  ImGui::End();
}

void main_loop(GLFWwindow *window, GB *vm) {
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  while (!glfwWindowShouldClose(window)) {
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
      imgui_cleanup();
      gb_destroy(vm);
      glfwDestroyWindow(window);
      glfwTerminate();
      return;
    }

    glfwPollEvents();
    imgui_new_frame();

    draw_window_crude_debug(vm);
    draw_window_texture();

    glClear(GL_COLOR_BUFFER_BIT);
    imgui_render();
    glfwSwapBuffers(window);

    cycles++;
    if (cycles % updateInterval == 0) {
      isBlack = !isBlack;
      update_texture();
    }
  }
}

int texture_init() {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);

  return 0;
}

int texture_destroy() {
  delete[] pixels;
  glDeleteTextures(1, &texture);

  return 0;
}
