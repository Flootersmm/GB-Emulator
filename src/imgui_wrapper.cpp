// imgui_wrapper.cpp
#include "dear_imgui/imgui_wrapper.h"
#include "dear_imgui/imgui.h"
#include "dear_imgui/imgui_impl_glfw.h"
#include "dear_imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

static GB *vm = nullptr;

void imgui_init(GLFWwindow *window, GB *vm) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");
}

void imgui_new_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void imgui_render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_cleanup() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

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

int calculateBytesPerLine(float windowWidth, float charWidth) {
  return static_cast<int>((windowWidth * 0.75f) / (charWidth * 3.5f));
}
