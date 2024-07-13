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
static void SetupBuffer(GLID id, kt::StorageBufferUsage usage);

static inline constexpr GLuint BufferUsage2Native(kt::StorageBufferUsage usage);


namespace kt
{
  template<StorageBufferType Type>
  StorageBuffer<Type>::StorageBuffer(StorageBufferUsage usage, size_t size)
    : m_id{ CreateBuffer() }, m_usage{ usage }, m_size{ size } {
    //glGenBuffers()
  }

  template<StorageBufferType Type>
  StorageBuffer<Type>::~StorageBuffer() {
    glDeleteBuffers(1, &m_id);
  }
  
  template<StorageBufferType Type>
  void StorageBuffer<Type>::update(const void *data, size_t offset, size_t size) {
    glBufferSubData((int)Type, offset, size, data);
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
void SetupBuffer(GLID id, kt::StorageBufferUsage usage) {
  glBindBuffer((int)type, id);

  glBufferData((int)type, 0, nullptr, BufferUsage2Native(usage));

  glBindBuffer((int)type, 0);
}
