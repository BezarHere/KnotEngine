#pragma once
#include "Shader.hpp"
#include "utility/Blob.hpp"

namespace kt
{

  class ShaderProgram
  {
  public:
    struct UniformInfo
    {
      uint32_t index;
      std::string name;
    };

    ShaderProgram(const Blob<const Shader *> &shaders);

    inline GLID get_id() const { return m_id; }

    static void _attach_shader(GLID id, const Shader &shader);
  private:
    GLID m_id;
  };

}
