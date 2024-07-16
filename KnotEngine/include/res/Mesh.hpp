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


    void set_vertices(VertexBuffer &&buffer);
    void set_vertices(const VertexBuffer &buffer);

    PrimitiveType primitive;
    MeshIndexBuffer indicies;
  private:
    VertexBuffer m_vertices;
    VertexInputDesc m_input;
  };

}
