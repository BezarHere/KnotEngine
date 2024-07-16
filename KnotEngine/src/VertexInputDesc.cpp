#include "pch.h"
#include "res/VertexInputDesc.hpp"
#include "GLCommon.hpp"

static GLID CreateVAO();
static inline GLuint SPInputSizeToNative(kt::VertexDataSize size);
static inline GLuint SPInputTypeToNative(kt::VertexDataType type);
static inline size_t SPInputSizeBytes(kt::VertexDataType type, kt::VertexDataSize size);

namespace kt
{
  VertexInputDesc::VertexInputDesc(std::_Uninitialized)
    : GraphicsResource{}, m_attributes{} {
  }

  VertexInputDesc::VertexInputDesc()
    : GraphicsResource{ CreateVAO() }, m_attributes{} {
  }

  VertexInputDesc::~VertexInputDesc() noexcept {
    glDeleteVertexArrays(1, &m_id);
  }

  VertexInputDesc::VertexInputDesc(VertexInputDesc &&move) noexcept
    : GraphicsResource{ move.m_id }, m_attributes{ move.m_attributes } {
  }

  VertexInputDesc &VertexInputDesc::operator=(VertexInputDesc &&move) noexcept {
    this->~VertexInputDesc();

    m_id = move.m_id;
    m_attributes = move.m_attributes;

    move.m_id = NULL;

    return *this;
  }

  VertexInputDesc::VertexInputDesc(const VertexInputDesc &copy) noexcept
    : GraphicsResource{ CreateVAO() }, m_attributes{ copy.m_attributes } {
  }

  VertexInputDesc &VertexInputDesc::operator=(const VertexInputDesc &copy) noexcept {
    if (std::addressof(copy) == this)
    {
      return *this;
    }

    this->~VertexInputDesc();

    m_id = CreateVAO();
    m_attributes = copy.m_attributes;

    return *this;
  }

  size_t VertexInputDesc::calculate_vertex_size() const {
    size_t acumm = 0;
    for (size_t i = 0; i < m_attributes.size(); i++)
    {
      acumm += SPInputSizeBytes(m_attributes[i].type, m_attributes[i].size);
    }
    return acumm;
  }

  VertexInputDesc::AttributeList &VertexInputDesc::get_modifiable_attributes() {
    this->mark_dirty();
    return m_attributes;
  }

  void VertexInputDesc::__gl_update(VertexBuffer *vbuf) {
    if (vbuf)
    {
      glBindBuffer(vbuf->gl_type, vbuf->get_id());
    }

    glBindVertexArray(m_id);

    const uint32_t inputs_count = uint32_t(m_attributes.size());

    // disable un-needed locations
    for (GLuint i = inputs_count; i < m_attributes.max_size(); i++)
    {
      glDisableVertexAttribArray(i);
    }

    const size_t stride = calculate_vertex_size();

    intptr_t current_offset = 0;
    for (uint32_t i = 0; i < inputs_count; i++)
    {
      const auto &input = m_attributes[i];
      glVertexAttribPointer(
        i,
        SPInputSizeToNative(input.size),
        SPInputTypeToNative(input.type),
        input.normalized,
        uint32_t(input.stride + stride),
        (const char *)(input.offset + current_offset)
      );
      current_offset += SPInputSizeBytes(input.type, input.size);

      if (CHECK_OPENGL_V("binding vertex attrib pointer %llu", i))
      {
        continue;
      }

      glEnableVertexAttribArray(i);
    }

    glBindVertexArray(m_id);
    if (vbuf)
    {
      glBindBuffer(vbuf->gl_type, NULL);
    }

    m_dirty = false;
  }

}

GLID CreateVAO() {
  GLID vao;
  glGenVertexArrays(1, &vao);

  if (vao == NULL)
  {
    (void)CHECK_OPENGL("failed to create a VAO");
    return NULL;
  }

  return vao;
}

inline GLuint SPInputSizeToNative(kt::VertexDataSize size) {
  return GLuint(size);
}

inline GLuint SPInputTypeToNative(kt::VertexDataType type) {
  switch (type)
  {
  case kt::VertexDataType::Float:
    return GL_FLOAT;
  case kt::VertexDataType::Double:
    return GL_DOUBLE;
  case kt::VertexDataType::Byte:
    return GL_BYTE;
  case kt::VertexDataType::Short:
    return GL_SHORT;
  case kt::VertexDataType::Int:
    return GL_INT;
  case kt::VertexDataType::UByte:
    return GL_UNSIGNED_BYTE;
  case kt::VertexDataType::UShort:
    return GL_UNSIGNED_SHORT;
  case kt::VertexDataType::UInt:
    return GL_UNSIGNED_INT;
  default:
    return GL_FLOAT;
  }
}

inline size_t SPInputSizeBytes(kt::VertexDataType type, kt::VertexDataSize size) {
  const size_t sz = SPInputSizeToNative(size);
  switch (type)
  {
  case kt::VertexDataType::Double:
    return 8 * sz;
  case kt::VertexDataType::Float:
  case kt::VertexDataType::Int:
  case kt::VertexDataType::UInt:
    return 4 * sz;
  case kt::VertexDataType::Short:
  case kt::VertexDataType::UShort:
    return 2 * sz;
  case kt::VertexDataType::Byte:
  case kt::VertexDataType::UByte:
    return sz;
  default:
    return 4 * sz;
  }
}
