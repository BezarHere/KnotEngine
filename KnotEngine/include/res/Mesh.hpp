#pragma once
#include "VertexInputDesc.hpp"
#include "StorageBuffer.hpp"
#include "MeshIndexBuffer.hpp"

namespace kt
{
  enum class PrimitiveType
  {
    Point,
    Triangles,
    TriangleFan,
    TriangleStrip,
  };

  class Mesh : public Resource
  {
  public:

  private:
    bool m_dirty;
    PrimitiveType m_primitive;
    VertexBuffer m_vertices;
    MeshIndexBuffer m_indicies;
    VertexInputDesc m_input;
  };

}
