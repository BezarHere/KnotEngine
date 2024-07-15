#pragma once
#include "Resource.hpp"
#include "Image.hpp"

namespace kt
{
  enum class TextureFormat
  {
    Red8,
    RG8,

    RGB8,
    RGBA8,

    RGB16,
    RGBA16,

    RGB32F,
    RGBA32F,
  };

  class Texture : public GraphicsResource
  {
  public:
    struct LocalData
    {

    };

    Texture(TextureFormat format, const Image &img);

  };

}
