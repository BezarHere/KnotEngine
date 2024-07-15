#include "pch.h"
#include "res/MeshIndexBuffer.hpp"

static inline size_t IndexRangeSize(const kt::IndicesRange range) {
  return 1ULL << (int)range;
}

static inline const char *IndexRangeName(const kt::IndicesRange range) {
  switch (range)
  {
  case kt::IndicesRange::Byte:
    return "byte";
  case kt::IndicesRange::Short:
    return "short";
  case kt::IndicesRange::Int:
    return "int";
  default:
    return "unknown";
  }
}

namespace kt
{
  MeshIndexBuffer::MeshIndexBuffer(IndicesRange range, size_t size)
    : m_range{ range }, m_buffer{ StorageBufferUsage::StaticDraw, size * IndexRangeSize(range) } {
  }
  MeshIndexBuffer &MeshIndexBuffer::operator=(MeshIndexBuffer &&move) noexcept {
    if (move.m_range != this->m_range)
    {
      Log::error(
        EINVAL,
        "Can not move assign a %s-indexed to a %s-index!",
        IndexRangeName(move.m_range),
        IndexRangeName(this->m_range)
      );
      return *this;
    }

    m_buffer = std::forward<IndexBuffer>(move.m_buffer);
    return *this;
  }

  void MeshIndexBuffer::update(const void *indices, size_t count, size_t offset) {
    m_buffer.update(
      indices,
      count * IndexRangeSize(m_range),
      offset * IndexRangeSize(m_range)
    );
  }

}
