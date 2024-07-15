#pragma once
#include "Resource.hpp"
#include "utility/Blob.hpp"
#include "utility/ArrayList.hpp"

namespace kt
{
  enum class VertexDataType : uint8_t
  {
    Float,
    Double,

    Byte,
    Short,
    Int,

    UByte,
    UShort,
    UInt
  };

  enum class VertexDataSize : uint8_t
  {
    Single = 1,


    Double,
    Vec2 = Double,

    Triple,
    Vec3 = Triple,

    Quad,
    Vec4 = Quad
  };

  struct VertexAttribute
  {
    VertexDataType type;
    VertexDataSize size;

    bool normalized = false;

    intptr_t offset = 0;
    uint32_t stride = 0;
  };

  class Renderer;
  class VertexInputDesc : public GraphicsResource
  {
    friend Renderer;
  public:
    static constexpr size_t MaxAttributes = 8;
    typedef ArrayList<VertexAttribute, MaxAttributes> AttributeList;

    VertexInputDesc(std::_Uninitialized);
    VertexInputDesc();
    ~VertexInputDesc() noexcept;

    VertexInputDesc(VertexInputDesc &&move) noexcept;
    VertexInputDesc &operator=(VertexInputDesc &&move) noexcept;

    VertexInputDesc(const VertexInputDesc &copy) noexcept;
    VertexInputDesc &operator=(const VertexInputDesc &copy) noexcept;

    inline bool is_dirty() const { return m_dirty; }
    inline void mark_dirty() { m_dirty = true; }

    size_t calculate_vertex_size() const;

    inline Blob<const VertexAttribute> get_attributes() const { return { m_attributes.data(), m_attributes.size() }; }

    // will mark this as dirty
    AttributeList &get_modifiable_attributes();

  private:
    void _update();
  private:
    bool m_dirty = true;
    AttributeList m_attributes;
  };
}
