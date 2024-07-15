#include "pch.h"
#include "res/StorageBuffer.hpp"
#include "GLCommon.hpp"

using kt::StorageBufferType;

namespace kt
{
  template StorageBuffer<StorageBufferType::VertexBuffer>;
  template StorageBuffer<StorageBufferType::IndexBuffer>;

  template StorageBuffer<StorageBufferType::CopyReadBuffer>;
  template StorageBuffer<StorageBufferType::CopyWriteBuffer>;

  template StorageBuffer<StorageBufferType::DispatchIndirectBuffer>;
  template StorageBuffer<StorageBufferType::DrawIndirectBuffer>;

  template StorageBuffer<StorageBufferType::PixelPackBuffer>;
  template StorageBuffer<StorageBufferType::PixelUnPackBuffer>;

  template StorageBuffer<StorageBufferType::QueryBuffer>;

  template StorageBuffer<StorageBufferType::ShaderStorageBuffer>;
  template StorageBuffer<StorageBufferType::TexutreBuffer>;

  template StorageBuffer<StorageBufferType::TransformFeedbackBuffer>;
  template StorageBuffer<StorageBufferType::UniformBlockBuffer>;
}

static GLID CreateBuffer();
template <StorageBufferType type>
static void SetupBuffer(GLID id, kt::StorageBufferUsage usage, size_t size);

static inline constexpr GLuint BufferUsage2Native(kt::StorageBufferUsage usage);


namespace kt
{
  template<StorageBufferType Type>
  StorageBuffer<Type>::StorageBuffer()
    : GraphicsResource{}, m_usage{}, m_size{}, m_dirty{ true }, m_buf{} {
  }

  template<StorageBufferType Type>
  StorageBuffer<Type>::StorageBuffer(StorageBufferUsage usage, size_t size)
    : GraphicsResource{ CreateBuffer() }, m_usage{ usage }, m_size{ size }, m_dirty{ true }, m_buf{ size } {
    SetupBuffer<Type>(m_id, m_usage, m_size);
  }

  template<StorageBufferType Type>
  StorageBuffer<Type>::~StorageBuffer() {
    glDeleteBuffers(1, &m_id);
  }

  template<StorageBufferType Type>
  StorageBuffer<Type>::StorageBuffer(StorageBuffer &&move) noexcept
    : GraphicsResource{ move.m_id }, m_usage{ move.m_usage }, m_size{ move.m_size },
    m_dirty{ true }, m_buf{ move.m_buf } {
    move.m_id = NULL;
  }

  template<StorageBufferType Type>
  StorageBuffer<Type> &StorageBuffer<Type>::operator=(StorageBuffer &&move) noexcept {
    this->~StorageBuffer();

    this->m_id = move.m_id;
    this->m_usage = move.m_usage;
    this->m_size = move.m_size;

    this->m_dirty = true;
    this->m_buf = move.m_buf;

    move.m_id = NULL;
    return *this;
  }

  template<StorageBufferType Type>
  void StorageBuffer<Type>::update(const void *data, size_t size, size_t offset) {
    if (!is_valid())
    {
      return;
    }

    if (size + offset > m_size)
    {
      Log::error(ERANGE, "update will overflow, buffer size=%llu, update range=[%llu, %llu)", m_size, offset, offset + size);
      return;
    }

    m_dirty = true;
    m_buf.paste(data, size, offset);
  }

  template<StorageBufferType Type>
  void StorageBuffer<Type>::validate() {
    if (m_buf.size() != m_size)
    {
      Buffer new_buffer{ m_size };

      new_buffer.paste(m_buf.get(), std::min(m_buf.size(), m_size));

      m_buf = new_buffer;
    
      m_dirty = true;
    }
  }

  template<StorageBufferType Type>
  void StorageBuffer<Type>::__gl_upload() {
    glBindBuffer((int)Type, m_id);

    glBufferSubData((int)Type, 0, m_size, m_buf.get());

    glBindBuffer((int)Type, 0);
    m_dirty = false;
  }
}

GLID CreateBuffer() {
  GLID id;
  glCreateBuffers(1, &id);
  return id;
}

inline constexpr GLuint BufferUsage2Native(kt::StorageBufferUsage usage) {
  switch (usage)
  {
  case kt::StorageBufferUsage::StaticRead:
    return GL_STATIC_READ;
  case kt::StorageBufferUsage::StaticWrite:
    //case kt::StorageBufferUsage::StaticDraw:
    return GL_STATIC_DRAW;
  case kt::StorageBufferUsage::DynamicRead:
    return GL_DYNAMIC_READ;
  case kt::StorageBufferUsage::DynamicWrite:
    //case kt::StorageBufferUsage::DynamicDraw:
    return GL_DYNAMIC_DRAW;
  default:
    return GL_STATIC_DRAW;
  }
}

template<StorageBufferType type>
void SetupBuffer(GLID id, kt::StorageBufferUsage usage, size_t size) {
  glBindBuffer((int)type, id);

  glBufferData((int)type, size, nullptr, BufferUsage2Native(usage));
  CHECK_OPENGL_V("glBufferData(%X, %llu, %p, %d)", (int)type, size, nullptr, BufferUsage2Native(usage));

  glBindBuffer((int)type, 0);
}
