#include "pch.h"
#include "res/Mesh.hpp"

void kt::Mesh::set_vertices(VertexBuffer &&buffer) {
  m_vertices = std::forward<VertexBuffer>(buffer);
}

void kt::Mesh::set_vertices(const VertexBuffer &buffer) {
  m_vertices = buffer;
}
