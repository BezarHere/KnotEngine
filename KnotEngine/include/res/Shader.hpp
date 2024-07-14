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
  class Shader : public Resource
  {
    friend ShaderProgram;
  public:

    Shader(ShaderType type, const char *source);

    Shader(const Shader &copy);

    Shader(Shader &&move) noexcept;
    Shader &operator=(Shader &&move) noexcept;

    ~Shader() noexcept;

    inline bool is_valid() const noexcept { return m_id != 0; }
    inline ShaderType get_type() const noexcept { return m_type; }

    std::string get_source() const;

  private:
    GLID m_id;
    ShaderType m_type;
  };
}
