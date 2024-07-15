#pragma once
#include "Resource.hpp"
#include <string>

namespace kt
{
  enum class ShaderType
  {
    Vertex,
    Fragment,
    Geometry,
    Compute
  };

  class ShaderProgram;
  class Shader : public GraphicsResource
  {
    friend ShaderProgram;
  public:

    Shader(ShaderType type, const char *source);

    Shader(const Shader &copy);

    Shader(Shader &&move) noexcept;
    Shader &operator=(Shader &&move) noexcept;

    ~Shader() noexcept;

    inline ShaderType get_type() const noexcept { return m_type; }

    std::string get_source() const;

  private:
    ShaderType m_type;
  };
}
