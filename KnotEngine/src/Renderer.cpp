#include "pch.h"
#include "Renderer.hpp"

typedef kt::RenderCommand::CmdType CommandType;

namespace kt
{

  Renderer::Renderer(Window &window)
    : m_window{ window } {
  }

  void Renderer::clear() {
    glfwMakeContextCurrent((GLFWwindow *)m_window.m_handle);

    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glfwMakeContextCurrent(nullptr);
  }

  void Renderer::flush() {
    glfwSwapBuffers((GLFWwindow *)m_window.m_handle);
  }

  void Renderer::do_command(const RenderCommand &command) {
    switch (command.type)
    {
    case CommandType::Transform:

    default:
      break;
    }
  }

  void Renderer::draw_rect(const Rect &rect, const Color &color) {

  }

}
