#include "pch.h"
#include "res/ShaderProgram.hpp"
#include "utility/Bitfield.hpp"
#include "GLCommon.hpp"


static GLID CreateShaderProgram(const Blob<const kt::Shader *> &shaders);

namespace kt
{

  ShaderProgram::ShaderProgram(const Blob<const Shader *> &shaders)
    : m_id{ CreateShaderProgram(shaders) } {
  }

}

void kt::ShaderProgram::_attach_shader(GLID id, const Shader &shader) {
  glAttachShader(id, shader.m_id);
}

GLID CreateShaderProgram(const Blob<const kt::Shader *> &shaders) {

  GLID program = glCreateProgram();
  if (program == NULL)
  {
    CHECK_OPENGL("failed to create program");
    return NULL;
  }

  for (const kt::Shader *shader : shaders)
  {
    kt::ShaderProgram::_attach_shader(program, *shader);
    if (CHECK_OPENGL_V("attaching shader %p to %u failed", shader, program) != EOK)
    {
      continue;
    }
  }

  glLinkProgram(program);

  GLint linked_success = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &linked_success);

  if (linked_success != GL_TRUE)
  {
    GLsizei log_length = 0;
    OpenGL::LogBuffer message;
    glGetProgramInfoLog(program, OpenGL::LogBufferSz, &log_length, message);

    kt::Log::error(EFAULT, "failed to link shader program %d, msg=%s", program, message);

    glDeleteProgram(program);
    return NULL;
  }

  return program;
}
