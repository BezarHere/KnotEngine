#pragma once
#include "Resource.hpp"

namespace kt
{
  enum class StorageBufferType
  {
    VertexBuffer = 0x8892,
    IndexBuffer = 0x8893,

    CopyReadBuffer = 0x8F36,
    CopyWriteBuffer = 0x8F37,

    DispatchIndirectBuffer = 0x90EE,
    DrawIndirectBuffer = 0x8F3F,

    PixelPackBuffer = 0x88EB,
    PixelUnPackBuffer = 0x88EC,

    QueryBuffer = 0x9192,

    ShaderStorageBuffer = 0x90D2,
    TexutreBuffer = 0x8C2A,

    TransformFeedbackBuffer = 0x8C8E,
    UniformBlockBuffer = 0x8A11
  };

  enum class StorageBufferUsage
  {
    StaticRead,
    StaticWrite,
    StaticDraw = StaticWrite,

    DynamicRead,
    DynamicWrite,
    DynamicDraw = DynamicWrite,
  };

  template <StorageBufferType Type>
  class StorageBuffer : public Resource
  {
  public:
    static constexpr StorageBufferType gl_type = Type;

    StorageBuffer(StorageBufferUsage usage, size_t size);
    ~StorageBuffer();

    void update(const void *data, size_t offset, size_t size);

  private:
    GLID m_id;
    StorageBufferUsage m_usage;
    size_t m_size;
  };

  
  typedef StorageBuffer<StorageBufferType::VertexBuffer> VertexBuffer;
  typedef StorageBuffer<StorageBufferType::IndexBuffer> IndexBuffer;
}
