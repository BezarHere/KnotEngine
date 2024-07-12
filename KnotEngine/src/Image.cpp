#include "pch.h"
#include "res/Image.hpp"

typedef kt::ImageFormat ImFormat;
using kt::real_t;

static constexpr real_t ByteNormalizeMul = 255;
static constexpr real_t ByteNormalizeDiv = 1 / ByteNormalizeMul;

static inline constexpr size_t GetFormatSize(ImFormat format);
static inline kt::Color ReadColor(const uint8_t *ptr, ImFormat format);
static inline void WriteColor(uint8_t *ptr, ImFormat format, const kt::Color &color);

namespace kt
{

  Image::Image(ImageFormat format, const Vec2u &size)
    : m_format{ format }, m_size{ size }, m_data{ GetFormatSize(format) * m_size.x * m_size.y } {
  }

  Color Image::get_pixel(uint32_t x, uint32_t y) const {
    if (x >= m_size.x || y >= m_size.y)
    {
      throw std::out_of_range("x || y");
    }

    const size_t position = GetFormatSize(m_format) * size_t(y * m_size.x + x);

    return ReadColor(m_data.data() + position, m_format);
  }

  void Image::set_pixel(uint32_t x, uint32_t y, const Color &color) {
    if (x >= m_size.x || y >= m_size.y)
    {
      throw std::out_of_range("x || y");
    }


    const size_t position = GetFormatSize(m_format) * size_t(y * m_size.x + x);

    return WriteColor(m_data.data() + position, m_format, color);
  }

}

inline constexpr size_t GetFormatSize(ImFormat format) {

  switch (format)
  {
  case ImFormat::RGB8:
    return 1ULL * 3;
  case ImFormat::RGB32:
    return 4ULL * 3;
  case ImFormat::RGBA8:
    return 1ULL * 4;
  case ImFormat::RGBA32:
    return 4ULL * 4;
  default:
    return 0;
  }
}

inline kt::Color ReadColor(const uint8_t *ptr, ImFormat format) {
  switch (format)
  {
  case ImFormat::RGB8:
    return kt::Color(ptr[0] * ByteNormalizeDiv, ptr[1] * ByteNormalizeDiv, ptr[2] * ByteNormalizeDiv);

  case ImFormat::RGB32:
    {
      const float *rgb32 = (const float *)ptr;
      return kt::Color(rgb32[0], rgb32[1], rgb32[2]);
    }

  case ImFormat::RGBA8:
    return kt::Color(ptr[0] * ByteNormalizeDiv, ptr[1] * ByteNormalizeDiv, ptr[2] * ByteNormalizeDiv, ptr[3] * ByteNormalizeDiv);

  case ImFormat::RGBA32:
    {
      const float *rgb32 = (const float *)ptr;
      return kt::Color(rgb32[0], rgb32[1], rgb32[2], rgb32[3]);
    }

  default:
    return kt::Color();
  }
}

inline void WriteColor(uint8_t *ptr, ImFormat format, const kt::Color &color) {
  switch (format)
  {
  case ImFormat::RGB8:
  case ImFormat::RGBA8:
    {
      ptr[0] = uint8_t(color.r * ByteNormalizeMul);
      ptr[1] = uint8_t(color.g * ByteNormalizeMul);
      ptr[2] = uint8_t(color.b * ByteNormalizeMul);
      if (format == ImFormat::RGBA8)
      {
        ptr[3] = uint8_t(color.a * ByteNormalizeMul);
      }

      return;
    }

  case ImFormat::RGB32:
  case ImFormat::RGBA32:
    {
      float *rgb32 = (float *)ptr;
      rgb32[0] = color.r;
      rgb32[1] = color.g;
      rgb32[2] = color.b;

      if (format == ImFormat::RGBA32)
      {
        rgb32[3] = color.a;
      }

      return;
    }

  default:
    return;
  }
}
