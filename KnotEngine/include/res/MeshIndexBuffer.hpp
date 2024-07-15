#pragma once
#pragma once
#include "StorageBuffer.hpp"
#include "utility/Buffer.hpp"

namespace kt
{
  enum class IndicesRange
  {
    Byte,
    Short,
    Int
  };

  class Mesh;
  class MeshIndexBuffer
  {
  public:
    friend Mesh;
    typedef uint32_t genaric_index_type;

    template <IndicesRange Range>
    static constexpr auto index_npos = nullptr;

    template <>
    static constexpr auto index_npos<IndicesRange::Byte> = std::numeric_limits<uint8_t>::max();

    template <>
    static constexpr auto index_npos<IndicesRange::Short> = std::numeric_limits<uint16_t>::max();

    template <>
    static constexpr auto index_npos<IndicesRange::Int> = std::numeric_limits<uint32_t>::max();

    template <IndicesRange Range>
    using index_type = decltype(index_npos<Range>);

    MeshIndexBuffer(IndicesRange range = IndicesRange::Short, size_t size = 0);

    MeshIndexBuffer &operator=(MeshIndexBuffer &&move) noexcept;

    inline IndicesRange get_range() const noexcept { return m_range; }

    // indices is an array of index_type<range>
    void update(const void *indices, size_t count, size_t offset = 0);


  private:
    const IndicesRange m_range;
    IndexBuffer m_buffer;
  };

}
