#pragma once
#include "Resource.hpp"
#include "utility/Buffer.hpp"

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
  class StorageBuffer : public GraphicsResource
  {
  public:
    static constexpr uint32_t gl_type = uint32_t(Type);

    StorageBuffer();

    StorageBuffer(StorageBufferUsage usage, size_t size);
    ~StorageBuffer();

    StorageBuffer(StorageBuffer &&move) noexcept;
    StorageBuffer &operator=(StorageBuffer &&move) noexcept;

    inline void mark_dirty() { m_dirty = true; }
    inline bool is_dirty() const { return m_dirty; }

    inline size_t size() const { return m_size; }

    void update(const void *data, size_t size, size_t offset);

    void validate();

    // uploads the data to opengl's buffer, not be called by the user!
    void __gl_upload();
  private:
    StorageBufferUsage m_usage;
    size_t m_size;

    bool m_dirty;
    Buffer m_buf;
  };


  typedef StorageBuffer<StorageBufferType::VertexBuffer> VertexBuffer;
  typedef StorageBuffer<StorageBufferType::IndexBuffer> IndexBuffer;
}
