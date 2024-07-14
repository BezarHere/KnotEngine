#include "pch.h"
#include "res/Shader.hpp"
#include "GLCommon.hpp"

static inline GLuint ShaderTypeToNative(kt::ShaderType type);
static GLID CreateShader(kt::ShaderType type, const char *source);

namespace kt
{

  Shader::Shader(ShaderType type, const char *source)
    : m_id{ CreateShader(type, source) }, m_type{ type } {

  }

  Shader::Shader(const Shader &copy) : m_id{}, m_type{ copy.m_type } {
    if (!copy.is_valid())
    {
      return;
    }

    auto source = copy.get_source();
    m_id = CreateShader(m_type, source.c_str());
  }

  Shader::Shader(Shader &&move) noexcept : m_id{ move.m_id }, m_type{ move.m_type } {
    move.m_id = NULL;
  }

  Shader &Shader::operator=(Shader &&move) noexcept {
    this->~Shader();

    m_id = move.m_id;
    m_type = move.m_type;

    move.m_id = NULL;

    return *this;
  }

  Shader::~Shader() noexcept {
    glDeleteShader(m_id);
  }

  std::string Shader::get_source() const {
    if (!is_valid())
    {
      return "";
    }


    GLint length = 0;
    glGetShaderiv(m_id, GL_SHADER_SOURCE_LENGTH, &length);

    if (length == 0)
    {
      return "";
    }

    std::string result;
    result.resize(length);

    GLsizei returned_length = 0;
    glGetShaderSource(m_id, length, &returned_length, result.data());

    if (returned_length != length)
    {
      Log::warning(EACCES, "Shader source length mismatch: expected %d, returned %d", length, returned_length);
    }

    result.shrink_to_fit();
    return result;
  }

}

inline GLuint ShaderTypeToNative(kt::ShaderType type) {
  switch (type)
  {
  case kt::ShaderType::Vertex:
    return GL_VERTEX_SHADER;
  case kt::ShaderType::Fragment:
    return GL_FRAGMENT_SHADER;
  case kt::ShaderType::Geometry:
    return GL_GEOMETRY_SHADER;
  case kt::ShaderType::Compute:
    return GL_COMPUTE_SHADER;
  default:
    return GL_VERTEX_SHADER;
  };
}

GLID CreateShader(kt::ShaderType type, const char *source) {
  const GLID id = glCreateShader(ShaderTypeToNative(type));

  glShaderSource(id, 1, &source, nullptr);

  if (CHECK_OPENGL_V("shader source set to %p, id=%u", source, id) != EOK)
  {
    glDeleteShader(id);
    return 0;
  }

  glCompileShader(id);

  if (CHECK_OPENGL_V("compiling shader id=%u", id) != EOK)
  {
    glDeleteShader(id);
    return 0;
  }

  GLint compile_successful = 0;
  glGetShaderiv(id, GL_COMPILE_STATUS, &compile_successful);

  if (compile_successful != GL_TRUE)
  {
    OpenGL::LogBuffer log_buffer{};
    glGetShaderInfoLog(id, OpenGL::LogBufferSz, NULL, log_buffer);
    kt::Log::error(EFAULT, "Shader compile error: %s", log_buffer);


    glDeleteShader(id);
    return 0;
  }

  return id;
}
