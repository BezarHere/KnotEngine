#include "pch.h"
#include "Renderer.hpp"
#include "GLCommon.hpp"

#include <memory>

typedef kt::RenderCommand::CmdType CommandType;

struct Debug
{
  std::unique_ptr<kt::ShaderProgram> program;
  kt::VertexInputDesc input = { std::_Noinit };
  kt::VertexBuffer vbuf;
  bool init = false;
} g_Debug;

struct BasicVertex
{
  kt::Vec2 pos;
  kt::Color color;
};

static void CreateDebugShader();

namespace kt
{

  Renderer::Renderer(Window &window)
    : m_window{ window } {
  }

  void Renderer::start() {
    glfwMakeContextCurrent((GLFWwindow *)m_window.m_handle);

    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_drawing = true;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    if (!g_Debug.init)
    {
      g_Debug.init = true;

      g_Debug.vbuf = VertexBuffer(StorageBufferUsage::DynamicDraw, sizeof(BasicVertex) * 6);

      g_Debug.input = {};
      auto &inputs = g_Debug.input.get_modifiable_attributes();
      inputs.clear();
      inputs.resize(2);

      inputs[0].type = VertexDataType::Float;
      inputs[0].size = VertexDataSize::Vec2;

      inputs[1].type = VertexDataType::Float;
      inputs[1].size = VertexDataSize::Vec4;

      //g_Debug.input._update();

      CreateDebugShader();
    }

    //glViewport(0, 0, 64, 64);


    //RenderCommand command{ CommandType::Rect };
    //command.rect.rect = { -0.2F, 0.4F, 0.8F, -0.9F };
    //command.rect.tint = { 1.F, 1.F, 0.5F };
    //do_command(command);

    //command.rect.rect = { 0.2F, 0.1F, 0.9F, 0.4F };
    //command.rect.tint = { 1.F, 0.5F, 1.F };
    //do_command(command);
  }

  void Renderer::flush() {
    m_drawing = false;
    glfwSwapBuffers((GLFWwindow *)m_window.m_handle);
    glfwMakeContextCurrent(nullptr);
  }

  void Renderer::do_command(const RenderCommand &command) {
    switch (command.type)
    {
    case CommandType::Transform:

    case CommandType::Rect:
      {
        const auto &rect = command.rect.rect;
        BasicVertex vertices[6]
        {
          { rect.position, command.rect.tint },
          { { rect.size.x + rect.position.x, rect.position.y} , command.rect.tint },
          { { rect.size.x + rect.position.x, rect.size.y + rect.position.y}, command.rect.tint },
          { rect.position, command.rect.tint },
          { { rect.size.x + rect.position.x, rect.size.y + rect.position.y}, command.rect.tint },
          { { rect.position.x, rect.size.y + rect.position.y}, command.rect.tint }
        };
        g_Debug.vbuf.update(
          vertices, sizeof(vertices)
        );
        g_Debug.vbuf.__gl_upload();

        glBindBuffer(g_Debug.vbuf.gl_type, g_Debug.vbuf.get_id());

        g_Debug.input.__gl_update();

        glUseProgram(g_Debug.program->get_id());

        glBindVertexArray(g_Debug.input.m_id);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(NULL);
        glBindBuffer(g_Debug.vbuf.gl_type, 0);
      }
      break;
    default:
      break;
    }
  }

  void Renderer::draw_rect(const Rect &rect, const Color &color) {

  }

}

void CreateDebugShader() {
  kt::Shader vertex{
    kt::ShaderType::Vertex,

    "#version 410\n"
    "layout (location = 0) in vec2 A_pos;"
    "layout (location = 1) in vec4 A_clr;"
    "out vec4 FragClr;"


    "void main() {"
    "gl_Position = vec4(A_pos.x, A_pos.y, 0.0, 1.0);"
    "FragClr = A_clr;"
    "}"
  };

  kt::Shader fragment{
    kt::ShaderType::Fragment,

    "#version 330\n"
    "in vec4 FragClr;"
    "layout (location = 0) out vec4 Color;"

    "void main() {"
    "Color = FragClr;"
    "}"
  };

  const kt::Shader *pointers[2] = { &vertex, &fragment };
  g_Debug.program.reset(new kt::ShaderProgram(Blob<const kt::Shader *>{ pointers, std::size(pointers) }));
}
