#include "ijvm.h"
#include "imgui.h"
#include "imgui_wrapper.h"
#include "util.h"
#include <GLFW/glfw3.h>
#include <ctype.h>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>

// TODO:
// - limit dimensions to not exceed main window dims
// - dynamic shell buffer size because otherwise it stops printing
// - fix it so we can also separately make the cli ver
// - move like variables into structs
// - move shell to the top right, move show file and show stack next to menu
// - prettier printing for file
// - additional memory window for inspecting registers etc, printed like vscode
// - debug window that shows all variables and states
//
// - long term expand to just jvm?

#define BUFFER_SIZE 16096

char stdout_buffer[BUFFER_SIZE]; ///< Buffer for stdout redirection
FILE *stdout_memstream;          ///< Space for stdout redirection

// IJVM
char *binary_path = NULL; ///< Path to IJVM binary
bool program_started =
    false; ///< True if we've started stepping through IJVM binary
bool toggle_info = false;      ///< Show stack state
bool toggle_backtrace = false; ///< Show callstack backtrace
bool toggle_file = false;      ///< Show IJVM binary
int prev_bp = -1;              ///< Previous breakpoint
ijvm *m = NULL;                ///< IJVM virtual machine instance

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

/// Redirect stdout
///
/// Create memory region stdout_memstream 'as file', with immediate capture, and
/// set stdout to it
void setup_stdout_capture() {
  stdout_memstream = fmemopen(stdout_buffer, BUFFER_SIZE, "w");
  if (stdout_memstream == NULL) {
    perror("fmemopen");
    exit(EXIT_FAILURE);
  }
  setvbuf(stdout_memstream, NULL, _IONBF, 0);
  fflush(stdout);
  stdout = stdout_memstream;
}

/// Close stdout_memstream
void cleanup_stdout_capture() { fclose(stdout_memstream); }

/// Transform term colour codes to ImGui::TextColored calls
///
/// Uses '0\' and '\033' delimiters to find term colour codes, then uses a map
/// of term colour codes to ImVec4 colour codes to set the text colour
///
/// @param text Raw text including term colour codes
void RenderColoredText(const char *text) {
  const char *start = text;
  const char *end = text;

  ImVec4 current_color = color_map[RESET];

  while (*end != '\0') {
    if (*end == '\033') {
      if (start != end) {
        ImGui::TextColored(current_color, "%.*s", (int)(end - start), start);
      }

      for (const auto &pair : color_map) {
        size_t len = pair.first.length();
        if (strncmp(end, pair.first.c_str(), len) == 0) {
          current_color = pair.second;
          end += len;
          break;
        }
      }

      start = end;
    } else {
      ++end;
    }
  }

  if (start != end) {
    ImGui::TextColored(current_color, "%.*s", (int)(end - start), start);
  }
}

/// Draws Shell Output window that prints redirected stdout
///
/// Draws a window with:
/// - Title = Shell Output
/// - NoResize
/// - Size.x = char.x * 102
/// - Size.y = window_main_size.y * 0.5
/// - Pos.x = next to window_menu + border
/// - Pos.y = 0
/// - TextWrapPos = char.x * 102
void ShowCapturedStdout() {
  ImGui::SetNextWindowSize(
      ImVec2((ImGui::CalcTextSize("F").x * 102), // Size of our Mandelbrot test
             window_main_size.y * 0.5f));
  ImGui::SetNextWindowPos(ImVec2(window_menu_size.x + border_size, 0));

  ImGui::Begin("Shell Output", nullptr, ImGuiWindowFlags_NoResize);
  window_shell_size = ImGui::GetWindowSize();
  ImGui::PushTextWrapPos(ImGui::CalcTextSize("F").x * 102);

  RenderColoredText(stdout_buffer);

  ImGui::PopTextWrapPos();

  ImGui::End();
}

/// Print hardcoded raw string of ascii art
void print_welcome_message() {
  printf(
      "\n"
      "\n"
      "Welcome to...\n"
      "\n" MAG
      "   :::::::::::      :::::::::::    :::     :::         :::   :::\n"
      "         :+:              :+:        :+:     :+:        :+:+: :+:+:\n"
      "        +:+              +:+        +:+     +:+       +:+ +:+:+ +:+\n"
      "       +#+              +#+        +#+     +:+       +#+  +:+  +#+\n"
      "      +#+              +#+         +#+   +#+        +#+       +#+\n"
      "     #+#          #+# #+#          #+#+#+#         #+#       #+#\n"
      "###########       #####             ###           ###       ###\n" RESET
      "\n"
      "Type 'help' for help, and 'quit' to quit!\n"
      "\n");
}

/// Print help string before window opens
static void print_help(void) { printf("Usage: ./ijvm binary \n"); }

/// Initialize GLFW and create a window
///
/// @return Pointer to the created GLFW window
GLFWwindow *initialize_window() {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    exit(EXIT_FAILURE);
  }

  GLFWwindow *window =
      glfwCreateWindow(1280, 720, "IJVM Interpreter", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Vsync

  return window;
}

/// Main GUI loop
///
/// @param window Pointer to the GLFW window
void main_loop(GLFWwindow *window) {
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  while (!glfwWindowShouldClose(window)) {
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
      cleanup_imgui();
      destroy_ijvm(m);
      glfwDestroyWindow(window);
      glfwTerminate();
      return;
    }

    glfwPollEvents();
    new_frame_imgui();

    ImVec2 window_main_size = ImGui::GetWindowSize();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("IJVM Interpreter", &gui_active, ImGuiWindowFlags_MenuBar);

    window_menu_size = ImGui::GetWindowSize();

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open..", "Ctrl+O")) {
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
    ImGui::Text("PC: %d", m->r.pc);
    ImGui::SameLine();
    ImGui::Text("SP: %d", m->stack.sp);
    ImGui::SameLine();
    ImGui::Text("LV: %d", m->r.lv);
    if (ImGui::Button("Step")) {
      step(m);
    }
    if (ImGui::Button("Run full program")) {
      while (!m->halting) {
        step(m);
      }
    }
    toggle_button("Close stack", "Open stack", gui_stack_active);
    toggle_button("Hide file", "Show file", gui_file_active);
    ShowCapturedStdout();
    if (ImGui::Button("Print test character")) {
      printf("X\n");
    }

    ImGui::SetCursorPosY((ImGui::GetWindowSize().y) - ImGui::GetFontSize() * 4);
    ImGui::SliderFloat("Font scale", &fontScale, 1.0f, 3.0f, "%.1f");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);

    ImGui::End();

    if (gui_stack_active) {
      ImGui::Begin("Stack", &gui_stack_active, ImGuiWindowFlags_NoResize);
      ImGui::Text("Stack test");
      ImGui::End();
    }

    if (gui_file_active) {
      float charWidth = ImGui::CalcTextSize("F").x;

      ImGui::SetNextWindowPos(ImVec2(window_shell_size.x + window_menu_size.x +
                                         border_size + border_size,
                                     0),
                              ImGuiCond_Always);
      ImGui::SetNextWindowSize(ImVec2(charWidth * 66, window_main_size.y),
                               ImGuiCond_Always);

      ImGui::Begin("File", &gui_file_active, ImGuiWindowFlags_NoResize);

      window_file_size = ImGui::GetWindowSize();
      if (ImGui::CollapsingHeader("Constant Pool")) {
        ImVec2 headerSize = ImGui::GetItemRectSize();
        if (m->const_pool_size > 0) {
          ImGui::BeginChild(
              "Constant Pool",
              ImVec2(0,
                     (ImGui::GetContentRegionAvail().y * 0.5f) - headerSize.y),
              true);
          for (word_t i = 0; i < m->const_pool_size; i += 16) {
            ImGui::Text("%08X ", i);
            ImGui::SameLine();
            for (int j = 0; j < 16; ++j) {
              if (i + j < m->const_pool_size) {
                ImGui::Text("%02X", m->const_pool_data[i + j]);
              } else {
                ImGui::Text("");
              }
              if (j < 15)
                ImGui::SameLine();
            }
          }
          ImGui::EndChild();
        }
      }

      static word_t previous_pc = 0;

      int bytesPerLineText =
          calculateBytesPerLine(window_file_size.x, charWidth);
      if (ImGui::CollapsingHeader("Text Section",
                                  ImGuiTreeNodeFlags_DefaultOpen)) {
        ImVec2 headerSize = ImGui::GetItemRectSize();

        ImGui::BeginChild(
            "Text Section",
            ImVec2(0, (ImGui::GetContentRegionAvail().y) - headerSize.y), true);

        for (word_t i = 0; i < m->text_size; i += 16) {
          ImGui::Text("%08X ", i);
          ImGui::SameLine();
          for (int j = 0; j < 16; ++j) {
            if (i + j < m->text_size) {
              if (i + j == m->r.pc) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%02X",
                                   m->text_data[i + j]);
                if (m->r.pc != previous_pc) {
                  ImGui::SetScrollHereY();
                }
              } else {
                ImGui::Text("%02X", m->text_data[i + j]);
              }
            } else {
              ImGui::Text("");
            }
            if (j < 15)
              ImGui::SameLine();
          }
        }

        byte_t instruction = get_instruction(m);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                           "PC: %4d, Instruction: 0x%02X", m->r.pc,
                           instruction);

        ImGui::EndChild();
      }

      previous_pc = m->r.pc; // Update the previous PC value

      ImGui::End();
    }
    io.FontGlobalScale = fontScale;

    glClear(GL_COLOR_BUFFER_BIT);
    render_imgui();
    glfwSwapBuffers(window);
  }
}

/// Initialize the IJVM virtual machine
///
/// @param binary_path Path to the IJVM binary
/// @return Pointer to the initialized IJVM virtual machine
ijvm *initialize_ijvm(char *binary_path) {
  ijvm *m = init_ijvm_std(binary_path);
  if (m == NULL) {
    fprintf(stderr, "Couldn't load binary %s\n", binary_path);
    exit(EXIT_FAILURE);
  }
  return m;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    print_help();
    return 1;
  }

  GLFWwindow *window = initialize_window();
  setup_stdout_capture();

  m = initialize_ijvm(argv[1]);
  init_imgui(window, m);
  print_welcome_message();

  main_loop(window);

  cleanup_imgui();
  destroy_ijvm(m);
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
