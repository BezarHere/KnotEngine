#pragma once
#include "Shader.hpp"
#include "utility/Blob.hpp"

namespace kt
{

  class ShaderProgram : public GraphicsResource
  {
  public:
    struct UniformInfo
    {
      uint32_t index;
      std::string name;
    };

    ShaderProgram(const Blob<const Shader *> &shaders);

    static void _attach_shader(GLID id, const Shader &shader);
  };

}
