#include "dear_imgui/imgui.h"
#include "dear_imgui/imgui_wrapper.h"
#include "gb.h"
#include "shared.h"
#include <SDL2/SDL.h>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>

extern std::atomic<bool> step_requested;

char stdout_buffer[4096]; ///< Buffer for stdout redirection
FILE *stdout_memstream;   ///< Space for stdout redirection

// IMGUI
bool gui_active = true;        ///< True if the GUI is active
bool gui_stack_active = false; ///< True if the stack window is active
bool gui_file_active = false;  ///< True if the file window is active
bool demo_window_active = false;
float fontScale = 1;                  ///< Global font scale
float border_size = 3.0;              ///< Border size between windows
ImVec2 window_menu_size(0.0f, 0.0f);  ///< Menu window size
ImVec2 window_stack_size(0.0f, 0.0f); ///< Stack window size
ImVec2 window_file_size(0.0f, 0.0f);  ///< File window size
ImVec2 window_shell_size(0.0f, 0.0f); ///< Shell window size
std::map<std::string, ImVec4>
    color_map = ///< String, ImVec4 map of term colours to ImGui colours
    {{MAG, ImVec4(0.698f, 0.569f, 0.847f, 1.0f)},
     {RESET, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)}};
ImVec4 colorLightestGreen = ImVec4(0.61f, 0.73f, 0.06f, 1.00f); ///> #9BBC0F
ImVec4 colorLightGreen = ImVec4(0.54f, 0.67f, 0.06f, 1.00f);    ///> #8BAC0F
ImVec4 colorDarkGreen = ImVec4(0.19f, 0.38f, 0.19f, 1.00f);     ///> #306230
ImVec4 colorDarkestGreen = ImVec4(0.06f, 0.22f, 0.06f, 1.00f);  ///> #0F380F
char last_key_pressed = '\0';
float rom_window_width = 0;

/// Initialise a GLFW window
///
/// Creates a GLFW window, makes it the current context, enabled v-sync
///
/// @return GLFW window
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
static const int updateInterval = 15;
static int width = 160;
static int height = 144;
static float scale = 2.0f;

/// Update the Gameboy display texture
///
/// When not in V-blank, we grab the framebuffer and unwrap it into a 1d array
/// for our texture
///
/// @param vm GB vm
void update_texture(GB *vm) {
  if (vm->mem.data[LY] >= 144) {
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
}

/// Handle ImGuiIO keypresses
///
/// @param vm GB *vm
/// @param last_key_pressed Last key pressed
void handle_key_event(GB *vm, char &last_key_pressed) {
  ImGuiIO &io = ImGui::GetIO();

  if (io.KeysDown[ImGuiKey_A]) {
    key_pressed(vm, 4);
    last_key_pressed = 'A';
  }
  if (io.KeysDown[ImGuiKey_S]) {
    key_pressed(vm, 5);
    last_key_pressed = 'S';
  }
  if (io.KeysDown[ImGuiKey_Enter]) {
    key_pressed(vm, 7);
    last_key_pressed = 'E';
  }
  if (io.KeysDown[ImGuiKey_Space]) {
    key_pressed(vm, 6);
    last_key_pressed = ' ';
  }
  if (io.KeysDown[ImGuiKey_RightArrow]) {
    key_pressed(vm, 0);
    last_key_pressed = '>';
  }
  if (io.KeysDown[ImGuiKey_LeftArrow]) {
    key_pressed(vm, 1);
    last_key_pressed = '<';
  }
  if (io.KeysDown[ImGuiKey_UpArrow]) {
    key_pressed(vm, 2);
    last_key_pressed = '^';
  }
  if (io.KeysDown[ImGuiKey_DownArrow]) {
    key_pressed(vm, 3);
    last_key_pressed = 'v';
  }

  if (!io.KeysDown[ImGuiKey_A]) {
    key_released(vm, 4);
  }
  if (!io.KeysDown[ImGuiKey_S]) {
    key_released(vm, 5);
  }
  if (!io.KeysDown[ImGuiKey_Enter]) {
    key_released(vm, 7);
  }
  if (!io.KeysDown[ImGuiKey_Space]) {
    key_released(vm, 6);
  }
  if (!io.KeysDown[ImGuiKey_RightArrow]) {
    key_released(vm, 0);
  }
  if (!io.KeysDown[ImGuiKey_LeftArrow]) {
    key_released(vm, 1);
  }
  if (!io.KeysDown[ImGuiKey_UpArrow]) {
    key_released(vm, 2);
  }
  if (!io.KeysDown[ImGuiKey_DownArrow]) {
    key_released(vm, 3);
  }
}

/// Draw the Dear ImGUI window for the Gameboy display texture
///
/// Draws a square window with a 5% margin, and displays texture as an image
void draw_window_texture() {
  ImVec2 texture_size = ImVec2((float)width * scale, (float)height * scale);
  ImVec2 margin = ImVec2(texture_size.x * 0.05f, texture_size.y * 0.05f);
  float title_bar_height = ImGui::GetFrameHeightWithSpacing();
  ImVec2 window_size = ImVec2(texture_size.x + margin.x * 2,
                              texture_size.y + margin.y * 2 + title_bar_height);

  ImGui::SetNextWindowPos(ImVec2(487 + rom_window_width, 360));
  ImGui::SetNextWindowSize(window_size);
  ImGui::Begin("Texture Window", NULL,
               ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoScrollWithMouse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  ImGui::SetCursorPos(ImVec2(margin.x, margin.y + title_bar_height));

  ImGui::Image((void *)(intptr_t)texture, texture_size);

  ImGui::End();
}

/// Draw a crude debug window
///
/// Registers, interrupts, flags, ROM info
void draw_window_crude_debug(GB *vm) {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(487.0f, 720.0f));
  ImGui::Begin("GB Emulator", &gui_active,
               ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoResize);

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open..", "Ctrl+O")) {
        // TODO
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) {
        // TODO
      }
      if (ImGui::MenuItem("Demo", "Show demo window")) {
        demo_window_active == true ? demo_window_active = false
                                   : demo_window_active = true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  if (ImGui::Button("Step")) {
    {
      step_requested = true;
    }
  }
  static int n = 1;
  ImGui::InputInt("Number of steps", &n);

  if (ImGui::Button("Steps")) {
    for (int i = 0; i < n; i++) {
      step(vm);
    }
  }
  ImGui::SameLine();
  if (ImGui::Button("Pause")) {
    {
      vm->paused == true ? vm->paused = false : vm->paused = true;
    }
  }
  ImGui::SameLine();
  ImGui::Text("Paused: %d", vm->paused);

  if (ImGui::CollapsingHeader("ROM Info")) {

    ImGui::Text("Path: %s", vm->rom_path);
    ImGui::Text("Size: %u bytes", vm->cart.size);
    ImGui::Text("Title:");
    for (int i = 0; i < 16; i++) {
      char display_char = vm->cart.title[i] == 0 ? '_' : vm->cart.title[i];
      ImGui::SameLine();
      ImGui::Text("%c", display_char);
    }
    ImGui::Separator();
    ImGui::Text("Manufacturer Code: %02X %02X %02X",
                vm->cart.manufacturer_code[0], vm->cart.manufacturer_code[1],
                vm->cart.manufacturer_code[2]);

    ImGui::Text("CGB Flag: %s", vm->flag.cgb == NON_CGB        ? "NON_CGB"
                                : vm->flag.cgb == CGB_ENHANCED ? "CGB_ENHANCED"
                                                               : "CGB_ONLY");

    ImGui::Text("Licensee: %s", licensee_get_name(vm));
    ImGui::Text("SGB Flag: %s", vm->cart.sgb_flag ? "true" : "false");
    ImGui::Text("Cartridge Type: %s",
                cart_get_type_str(vm->cart.cartridge_enum));
    ImGui::Text("ROM Size: %s", rom_get_size_str(vm));
    ImGui::Text("RAM Size: %s", ram_get_size_str(vm));
    ImGui::Text("Destination: %s", destination_code_get_str(vm));
    ImGui::Text("Mask ROM Version: %02X", vm->cart.mask_rom_ver_number);

    ImGui::Separator();
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
        ImGui::TextWrapped(" ");
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
    ImGui::Separator();
  }

  if (ImGui::CollapsingHeader("Registers")) {
    ImGui::Columns(2, "RegistersColumns");

    ImGui::Text("A:");
    ImGui::SameLine(40);
    ImGui::Text("$%02X", vm->r.a);
    ImGui::Text("B:");
    ImGui::SameLine(40);
    ImGui::Text("$%02X", vm->r.b);
    ImGui::Text("C:");
    ImGui::SameLine(40);
    ImGui::Text("$%02X", vm->r.c);
    ImGui::Text("D:");
    ImGui::SameLine(40);
    ImGui::Text("$%02X", vm->r.d);
    ImGui::Text("E:");
    ImGui::SameLine(40);
    ImGui::Text("$%02X", vm->r.e);
    ImGui::Text("F:");
    ImGui::SameLine(40);
    ImGui::Text("$%02X", vm->r.f);
    ImGui::Text("H:");
    ImGui::SameLine(40);
    ImGui::Text("$%02X", vm->r.h);
    ImGui::Text("L:");
    ImGui::SameLine(40);
    ImGui::Text("$%02X", vm->r.l);

    ImGui::NextColumn();

    ImGui::Text("AF:");
    ImGui::SameLine(40);
    ImGui::Text("$%04X", vm->r.af);
    ImGui::Text("BC:");
    ImGui::SameLine(40);
    ImGui::Text("$%04X", vm->r.bc);
    ImGui::Text("DE:");
    ImGui::SameLine(40);
    ImGui::Text("$%04X", vm->r.de);
    ImGui::Text("HL:");
    ImGui::SameLine(40);
    ImGui::Text("$%04X", vm->r.hl);
    ImGui::Text("PC:");
    ImGui::SameLine(40);
    ImGui::Text("$%04X", vm->r.pc);
    ImGui::Text("SP:");
    ImGui::SameLine(40);
    ImGui::Text("$%04X", vm->r.sp);
    ImGui::Text("STAT:");
    ImGui::SameLine(40);
    ImGui::Text("$%04X", vm->mem.data[0xFF41]);
    ImGui::Text("LY:");
    ImGui::SameLine(40);
    ImGui::Text("$%04X", vm->mem.data[LY]);

    ImGui::Columns(1);

    ImGui::Separator();
    ImGui::Text("Flags: Z:%d N:%d H:%d C:%d", (vm->r.f >> 7) & 1,
                (vm->r.f >> 6) & 1, (vm->r.f >> 5) & 1, (vm->r.f >> 4) & 1);

    u8 interrupt_flags = read_u8(vm, 0xFF0F);
    u8 interrupt_enable = read_u8(vm, 0xFFFF);
    ImGui::Separator();

    ImGui::Text(
        "Interrupts Requested: V-Blank: %d LCD: %d Timer: %d Joypad: %d",
        (interrupt_flags >> 0) & 1,  // V-Blank
        (interrupt_flags >> 1) & 1,  // LCD
        (interrupt_flags >> 2) & 1,  // Timer
        (interrupt_flags >> 4) & 1); // Joypad

    ImGui::Text(
        "Interrupts Enabled:   V-Blank: %d LCD: %d Timer: %d Joypad: %d",
        (interrupt_enable >> 0) & 1,  // V-Blank
        (interrupt_enable >> 1) & 1,  // LCD
        (interrupt_enable >> 2) & 1,  // Timer
        (interrupt_enable >> 4) & 1); // Joypad

    ImGui::Separator();
    ImGui::Text("Timer Counter: %d", vm->timer_counter);
    ImGui::Text("Divider Counter: %d", vm->divider_counter);
    ImGui::Text("Divider Register: $%02X", vm->mem.data[0xFF04]);
    ImGui::Text("TIMA: $%02X", vm->mem.data[0xFF05]);
    ImGui::Text("TMA: $%02X", vm->mem.data[0xFF06]);
    ImGui::Text("Cycles: %u", vm->mem.data[0xFF07]);

    ImGui::Separator();
    ImGui::Text("ScrollY (0xFF42): 0x%02X", vm->mem.data[0xFF42]);
    ImGui::Text("ScrollX (0xFF43): 0x%02X", vm->mem.data[0xFF43]);
    ImGui::Text("WindowY (0xFF4A): 0x%02X", vm->mem.data[0xFF4A]);
    ImGui::Text("WindowX (0xFF4B): 0x%02X", vm->mem.data[0xFF4B]);

    ImGui::Separator();
    ImGui::Text("Joypad: %02X", vm->mem.data[0xFF00]);
    ImGui::Text("Last Key Pressed: %c",
                last_key_pressed ? last_key_pressed : ' ');

    ImGui::Separator();
    u8 lcd_control = vm->mem.data[0xFF40];
    ImGui::Text("LCD Control Register: 0x%02X", lcd_control);
    ImGui::Text("Scanline counter: %04x", vm->scanline_counter);

    ImGui::Separator();

    ImGui::Text("LCD Display Enable:             %d",
                (lcd_control >> 7) & 0x01);
    ImGui::Text("Window Tile Map Display Select: %d",
                (lcd_control >> 6) & 0x01);
    ImGui::Text("Window Display Enable:          %d",
                (lcd_control >> 5) & 0x01);
    ImGui::Text("BG & Window Tile Data Select:   %d",
                (lcd_control >> 4) & 0x01);
    ImGui::Text("BG Tile Map Display Select:     %d",
                (lcd_control >> 3) & 0x01);
    ImGui::Text("OBJ (Sprite) Size:              %d",
                (lcd_control >> 2) & 0x01);
    ImGui::Text("OBJ (Sprite) Display Enable:    %d",
                (lcd_control >> 1) & 0x01);
    ImGui::Text("BG Display:                     %d", lcd_control & 0x01);

    ImGui::Separator();
  }

  if (ImGui::CollapsingHeader("Memory Locations")) {
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
    const int numColumns = 4;
    const int numRows = (numAddresses + numColumns - 1) / numColumns;
    const float rowHeight = ImGui::GetTextLineHeightWithSpacing();
    const float windowHeight = rowHeight * (numRows + 1);

    ImGui::BeginChild("MemoryGrid", ImVec2(0, windowHeight), true);

    for (int i = 0; i < numAddresses; i += numColumns) {
      for (int j = 0; j < numColumns; ++j) {
        if (i + j < numAddresses) {
          ImGui::Text("%s: $%02X", addresses[i + j],
                      vm->mem.data[locations[i + j]]);
          if (j < numColumns - 1)
            ImGui::SameLine();
        }
      }
    }

    ImGui::EndChild();
  }

  if (ImGui::CollapsingHeader("Framebuffer")) {
    ImGui::BeginChild("FramebufferView", ImVec2(0, 400), true);
    for (int y = 0; y < 144; y++) {
      for (int x = 0; x < 160; x++) {
        int red = vm->framebuffer[y][x][0];
        int green = vm->framebuffer[y][x][1];
        int blue = vm->framebuffer[y][x][2];
        ImGui::Text("[%03d, %03d]: %03d%03d%03d", x, y, red, green, blue);
        ImGui::SameLine();
      }
      ImGui::Separator();
    }
    ImGui::EndChild();
  }

  if (ImGui::CollapsingHeader("Current Instruction")) {
    const OPS *instr = &ops[vm->mem.data[vm->r.pc]];
    ImGui::Text("Opcode: %s", instr->debug_str);
    ImGui::Text("Length: %u", instr->length);

    ImGui::Text("Operands:");
    ImGui::SameLine();
    for (int i = 0; i < instr->length; i++) {
      ImGui::Text("0x%02X", vm->cart.data[vm->r.pc + i]);
      ImGui::SameLine();
    }
  }

  ImGui::NewLine();
  ImGui::Separator();
  ImGui::NewLine();

  ImGui::SliderFloat("Font scale", &fontScale, 1.0f, 3.0f, "%.1f");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

  ImGui::SliderFloat("GB Scale", &scale, 1.0f, 6.0f, "%1.0f");
  ImGui::GetIO().FontGlobalScale = fontScale;

  ImGui::End();
}

/// Draws a ROM viewer window
///
/// Displays the entirety of the ROM with some colour coding for VRAM
void draw_rom_viewer_window(GB *vm) {
  rom_window_width =
      ImGui::CalcTextSize("ROM:0000 ").x + 18 * ImGui::CalcTextSize("FF ").x;

  ImGui::SetNextWindowPos(ImVec2(487, 0));
  ImGui::SetNextWindowSize(ImVec2(rom_window_width, 720.0f));
  ImGui::Begin("ROM Viewer", NULL,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

  int rows = vm->mem.size / 16;
  for (int row = 0; row < rows; row++) {
    ImGui::Text("ROM:%04X", row * 16);
    ImGui::SameLine();

    for (int col = 0; col < 16; col++) {
      int index = row * 16 + col;
      if (vm->r.pc == index) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%02X",
                           vm->mem.data[index]);
      } else if (index >= 0x8000 && index <= 0x87FF) {
        ImGui::TextColored(ImVec4(0.7f, 0.5f, 0.9f, 1.0f), "%02X",
                           vm->mem.data[index]);
      } else {
        ImGui::Text("%02X", vm->mem.data[index]);
      }
      if (col < 15) {
        ImGui::SameLine();
      }
    }
  }

  ImGui::End();
}

void draw_disassembly_window(GB *vm) {
  ImGui::SetNextWindowPos(ImVec2(487 + rom_window_width, 0));
  ImGui::SetNextWindowSize(ImVec2(1280 - (480 + rom_window_width), 360.0f));
  ImGui::Begin("Disassembly Window", NULL,
               ImGuiWindowFlags_AlwaysAutoResize |
                   ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoScrollWithMouse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  u16 start_address = vm->r.pc > 0x10 ? vm->r.pc - 0x10 : 0;
  u16 end_address = start_address + 0x20;

  for (u16 address = start_address; address < end_address;) {
    u8 opcode = vm->mem.data[address];
    const char *op_str = ops[opcode].debug_str;
    int operand_length = ops[opcode].length;

    ImGui::Text("%04X: ", address);

    if (address == vm->r.pc) {
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s ", op_str);
    } else {
      ImGui::SameLine();
      ImGui::Text("%s ", op_str);
    }
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

    imgui_new_frame();

    {
      std::lock_guard<std::mutex> lock(emu_mutex);
      draw_window_crude_debug(vm);
      draw_window_texture();
      draw_rom_viewer_window(vm);
      draw_disassembly_window(vm);
    }

    if (demo_window_active) {
      ImGui::ShowDemoWindow();
    }

    glClear(GL_COLOR_BUFFER_BIT);
    imgui_render();
    glfwSwapBuffers(window);
    handle_key_event(vm, last_key_pressed);

    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
      running = false;
      return;
    }

    update_texture(vm);
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
  glDeleteTextures(1, &texture);

  return 0;
}
