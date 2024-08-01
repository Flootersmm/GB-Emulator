#include "dear_imgui/imgui.h"
#include "dear_imgui/imgui_wrapper.h"
#include "gb.h"
#include "shared.h"
#include <SDL2/SDL.h>
#include <ctype.h>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>

extern std::atomic<bool> step_requested;

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

void update_texture(GB *vm) {
  u8 texture_data[144 * 160 * 3];
  for (int y = 0; y < 144; y++) {
    for (int x = 0; x < 160; x++) {
      int offset = (y * 160 + x) * 3;
      texture_data[offset] = vm->framebuffer[y][x][0];
      texture_data[offset + 1] = vm->framebuffer[y][x][1];
      texture_data[offset + 2] = vm->framebuffer[y][x][2];
    }
  }

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 160, 144, GL_RGB, GL_UNSIGNED_BYTE,
                  texture_data);
}

void handle_key_event(GB *vm, SDL_Event *event) {
  if (event->type == SDL_KEYDOWN) {
    switch (event->key.keysym.sym) {
    case SDLK_a:
      key_pressed(vm, 4);
      break;
    case SDLK_s:
      key_pressed(vm, 5);
      break;
    case SDLK_RETURN:
      key_pressed(vm, 7);
      break;
    case SDLK_SPACE:
      key_pressed(vm, 6);
      break;
    case SDLK_RIGHT:
      key_pressed(vm, 0);
      break;
    case SDLK_LEFT:
      key_pressed(vm, 1);
      break;
    case SDLK_UP:
      key_pressed(vm, 2);
      break;
    case SDLK_DOWN:
      key_pressed(vm, 3);
      break;
    }
  } else if (event->type == SDL_KEYUP) {
    switch (event->key.keysym.sym) {
    case SDLK_a:
      key_released(vm, 4);
      break;
    case SDLK_s:
      key_released(vm, 5);
      break;
    case SDLK_RETURN:
      key_released(vm, 7);
      break;
    case SDLK_SPACE:
      key_released(vm, 6);
      break;
    case SDLK_RIGHT:
      key_released(vm, 0);
      break;
    case SDLK_LEFT:
      key_released(vm, 1);
      break;
    case SDLK_UP:
      key_released(vm, 2);
      break;
    case SDLK_DOWN:
      key_released(vm, 3);
      break;
    }
  }
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
    ImGui::Text("Flags: Z:%d N:%d H:%d C:%d", (vm->r.f >> 7) & 1,
                (vm->r.f >> 6) & 1, (vm->r.f >> 5) & 1, (vm->r.f >> 4) & 1);
    u8 interrupt_flags = read_u8(vm, 0xFF0F);
    u8 interrupt_enable = read_u8(vm, 0xFFFF);
    ImGui::Separator();

    ImGui::Text("Interrupts Requested: V-Blank:%d LCD:%d Timer:%d Joypad:%d",
                (interrupt_flags >> 0) & 1,  // V-Blank
                (interrupt_flags >> 1) & 1,  // LCD
                (interrupt_flags >> 2) & 1,  // Timer
                (interrupt_flags >> 4) & 1); // Joypad

    ImGui::Text("Interrupts Enabled: V-Blank:%d LCD:%d Timer:%d Joypad:%d",
                (interrupt_enable >> 0) & 1,  // V-Blank
                (interrupt_enable >> 1) & 1,  // LCD
                (interrupt_enable >> 2) & 1,  // Timer
                (interrupt_enable >> 4) & 1); // Joypad
    ImGui::Separator();

    ImGui::Text("Timer Counter: %d", vm->timer_counter);
    ImGui::Text("Divider Counter: %d", vm->divider_counter);
    ImGui::Text("TMC: $%02X", vm->tmc);
    ImGui::Text("Divider Register: $%02X", vm->divider_register);
    ImGui::Text("TIMA: $%02X", vm->mem.data[0xFF05]);
    ImGui::Text("TMA: $%02X", vm->mem.data[0xFF06]);
    ImGui::Text("Cycles: %u", vm->mem.data[0xFF07]);
    ImGui::Text("Retrace_LY: %02X", vm->retrace_ly);

    ImGui::Separator();
    ImGui::Text("ScrollY (0xFF42): 0x%02X", vm->mem.data[0xFF42]);
    ImGui::Text("ScrollX (0xFF43): 0x%02X", vm->mem.data[0xFF43]);
    ImGui::Text("WindowY (0xFF4A): 0x%02X", vm->mem.data[0xFF4A]);
    ImGui::Text("WindowX (0xFF4B): 0x%02X", vm->mem.data[0xFF4B]);

    ImGui::Separator();
    u8 lcd_control = vm->mem.data[0xFF40];
    ImGui::Text("LCD Control Register: 0x%02X", lcd_control);

    ImGui::Separator();

    ImGui::Text("LCD Display Enable: %d", (lcd_control >> 7) & 0x01);
    ImGui::Text("Window Tile Map Display Select: %d",
                (lcd_control >> 6) & 0x01);
    ImGui::Text("Window Display Enable: %d", (lcd_control >> 5) & 0x01);
    ImGui::Text("BG & Window Tile Data Select: %d", (lcd_control >> 4) & 0x01);
    ImGui::Text("BG Tile Map Display Select: %d", (lcd_control >> 3) & 0x01);
    ImGui::Text("OBJ (Sprite) Size: %d", (lcd_control >> 2) & 0x01);
    ImGui::Text("OBJ (Sprite) Display Enable: %d", (lcd_control >> 1) & 0x01);
    ImGui::Text("BG Display: %d", lcd_control & 0x01);
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

  if (ImGui::TreeNode("Framebuffer Contents")) {
    ImGui::BeginChild("FramebufferView", ImVec2(0, 400), true);
    for (int y = 0; y < 144; y++) {
      for (int x = 0; x < 160; x++) {
        int red = vm->framebuffer[y][x][0];
        int green = vm->framebuffer[y][x][1];
        int blue = vm->framebuffer[y][x][2];
        ImGui::Text("Pixel [%d, %d]: R=%d, G=%d, B=%d", x, y, red, green, blue);
      }
      ImGui::Separator();
    }
    ImGui::EndChild();
    ImGui::TreePop();
  }

  if (ImGui::TreeNode("Current Instruction")) {
    const OPS *instr = &ops[vm->cart.data[vm->r.pc]];
    ImGui::Text("Opcode: %s", instr->debug_str);
    ImGui::Text("Length: %u", instr->length);

    ImGui::Text("Operands:");
    for (int i = 0; i < instr->length; i++) {
      ImGui::Text("0x%02X", vm->cart.data[vm->r.pc + i]);
    }

    ImGui::TreePop();
  }

  if (ImGui::Button("Step")) {
    {
      step_requested = true;
    }
  }
  static char pc_input[5] = "";

  ImGui::Text("Set Program Counter:");
  ImGui::InputText("##pc_input", pc_input, IM_ARRAYSIZE(pc_input),
                   ImGuiInputTextFlags_CharsHexadecimal);

  if (ImGui::Button("Update PC")) {
    unsigned int new_pc;
    if (sscanf(pc_input, "%x", &new_pc) == 1) {
      vm->r.pc = static_cast<u16>(new_pc);
    }
  }
  ImGui::SliderFloat("Font scale", &fontScale, 1.0f, 3.0f, "%.1f");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

  ImGui::SliderFloat("GB Scale", &scale, 1.0f, 6.0f, "%1.0f");
  ImGui::GetIO().FontGlobalScale = fontScale;

  ImGui::End();
}

void draw_rom_viewer_window(GB *vm) {
  ImGui::Begin("ROM Viewer");

  // Calculate the number of rows
  int rows = vm->mem.size / 16;
  for (int row = 0; row < rows; row++) {
    // Print the address
    ImGui::Text("ROM:%04X ", row * 16);
    ImGui::SameLine();

    // Print the data
    for (int col = 0; col < 16; col++) {
      int index = row * 16 + col;
      if (vm->r.pc == index) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%02X ",
                           vm->mem.data[index]);
      } else {
        ImGui::Text("%02X ", vm->mem.data[index]);
      }
      if (col < 15) {
        ImGui::SameLine();
      }
    }
  }

  ImGui::End();
}

void draw_disassembly_window(GB *vm) {
  ImGui::Begin(
      "Disassembly Window", NULL,
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar |
          ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);

  // Display around the current PC
  u16 start_address = vm->r.pc > 0x10 ? vm->r.pc - 0x10 : 0;
  u16 end_address = start_address + 0x20;

  for (u16 address = start_address; address < end_address;) {
    u8 opcode = vm->mem.data[address];
    const char *op_str = ops[opcode].debug_str;
    int operand_length = ops[opcode].length;

    // Print address
    ImGui::Text("%04X: ", address);

    // Highlight the current PC
    if (address == vm->r.pc) {
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s ", op_str);
    } else {
      ImGui::SameLine();
      ImGui::Text("%s ", op_str);
    }

    // Print operands
    for (int i = 1; i <= operand_length; i++) {
      ImGui::SameLine();
      ImGui::Text("%02X ", vm->mem.data[address + i]);
    }

    address += 1 + operand_length;
  }

  ImGui::End();
}

void main_loop(GLFWwindow *window, GB *vm) {
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return;
  }

  while (!glfwWindowShouldClose(window) && running) {
    glfwPollEvents();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      handle_key_event(vm, &event);
    }

    imgui_new_frame();

    {
      std::lock_guard<std::mutex> lock(emu_mutex);
      draw_window_crude_debug(vm);
    }
    draw_window_texture();
    draw_rom_viewer_window(vm);
    draw_disassembly_window(vm);

    glClear(GL_COLOR_BUFFER_BIT);
    imgui_render();
    glfwSwapBuffers(window);

    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
      running = false;
      return;
    }

    cycles++;
    if (cycles % updateInterval == 0) {
      isBlack = !isBlack;
      update_texture(vm);
    }
  }

  SDL_Quit();
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
