#pragma once
#include <inttypes.h>
#include "../math/Color.hpp"
#include "../math/Vector.hpp"
#include "../utility/Span.hpp"

namespace kt
{
  enum class ImageFormat
  {
    RGB8,
    RGB32,

    RGBA8,
    RGBA32,
  };

  class Image
  {
  public:
    Image(ImageFormat format, const Vec2u &size);

    Color get_pixel(uint32_t x, uint32_t y) const;
    void set_pixel(uint32_t x, uint32_t y, const Color &color);


    inline ImageFormat format() const { return m_format; }
    inline const Vec2u &size() const { return m_size; }
    inline const Span<uint8_t> &data() const { return m_data; }

  private:
    ImageFormat m_format;
    Vec2u m_size;
    Span<uint8_t> m_data;
  };
}
