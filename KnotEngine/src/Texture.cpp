#include "pch.h"
#include "res/Texture.hpp"
#include "GLCommon.hpp"

#include <mutex>


typedef kt::TextureFormat TexFormat;

static inline GLID _CreateTexture() {
  GLID id = 0;
  glGenTextures(1, &id);
  return id;
}

static inline constexpr GLuint GetNativeInternalFormat(TexFormat format);
static inline constexpr GLuint GetNativeInputFormat(kt::ImageFormat format);
static inline constexpr GLuint GetNativeInputType(kt::ImageFormat format);

static void SetupTexture(GLID id);
static void LoadTextureData(GLID id, TexFormat format, const kt::Image &img);



static std::mutex g_texture_bind_mutex;
struct TextureBindLock
{
public:
  inline TextureBindLock(GLuint type, GLID id)
    : m_type{ type }, m_guard{ g_texture_bind_mutex } {
    glBindTexture(m_type, id);
  }

  inline ~TextureBindLock() {
    glBindTexture(m_type, 0);
  }


private:
  const GLuint m_type;
  std::lock_guard<std::mutex> m_guard;
};

namespace kt
{

  Texture::Texture(TextureFormat format, const Image &img)
    : m_id{ _CreateTexture() } {
    SetupTexture(m_id);
    LoadTextureData(m_id, format, img);
  }

}

inline constexpr GLuint GetNativeInternalFormat(TexFormat format) {
  constexpr GLuint lookup_table[] =
  {
    GL_R8,
    GL_RG8,

    GL_RGB8,
    GL_RGBA8,

    GL_RGB16,
    GL_RGBA16,

    GL_RGB32F,
    GL_RGBA32F,
  };

  return lookup_table[(int)format];
}

inline constexpr GLuint GetNativeInputFormat(kt::ImageFormat format) {
  switch (format)
  {
  case kt::ImageFormat::RGB8:
  case kt::ImageFormat::RGB32:
    return GL_RGB;
  case kt::ImageFormat::RGBA8:
  case kt::ImageFormat::RGBA32:
    return GL_RGBA;
  default:
    return GL_RGB;
  }
}

inline constexpr GLuint GetNativeInputType(kt::ImageFormat format) {
  switch (format)
  {
  case kt::ImageFormat::RGB8:
  case kt::ImageFormat::RGBA8:
    return GL_UNSIGNED_BYTE;
  case kt::ImageFormat::RGB32:
  case kt::ImageFormat::RGBA32:
    return GL_FLOAT;
  default:
    return GL_FLOAT;
  }
}

void SetupTexture(GLID id) {
  std::scoped_lock<decltype(g_texture_bind_mutex)> lock{ g_texture_bind_mutex };
  TextureBindLock bind_lock = { GL_TEXTURE_2D, id };

  // TODO: make those configurable
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

void LoadTextureData(GLID id, TexFormat format, const kt::Image &img) {
  std::scoped_lock<decltype(g_texture_bind_mutex)> lock{ g_texture_bind_mutex };
  TextureBindLock bind_lock = { GL_TEXTURE_2D, id };

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GetNativeInternalFormat(format),
    img.size().x,
    img.size().y,
    0,
    GetNativeInputFormat(img.format()),
    GetNativeInputType(img.format()),
    img.data().begin()
  );
}
