// imgui_wrapper.cpp
#include "dear_imgui/imgui_wrapper.h"
#include "dear_imgui/imgui.h"
#include "dear_imgui/imgui_impl_glfw.h"
#include "dear_imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

/// Initialise Dear ImGUI for OpenGL
void imgui_init(GLFWwindow *window, GB *vm) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");
}

/// Create new Dear ImGUI frame
void imgui_new_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

/// Render Dear ImGUI frame with OpenGL
void imgui_render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/// Cleanup Dear ImGUI context, and OpenGL and GLFW implementations
void imgui_cleanup() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

/// Create a toggle button
///
/// @param on "On" display string
/// @param off "Off" display string
/// @param value Boolean to control button state
void toggle_button(const char *on, const char *off, bool &value) {
  if (value) {
    if (ImGui::Button(on)) {
      value = false;
    }
  } else {
    if (ImGui::Button(off)) {
      value = true;
    }
  }
}

/// Calculate how many characters can fit in window width
///
/// @param windowWidth Width of window
/// @param charWidth Width of character, font dependent
int calculateBytesPerLine(float windowWidth, float charWidth) {
  return static_cast<int>((windowWidth * 0.75f) / (charWidth * 3.5f));
}
