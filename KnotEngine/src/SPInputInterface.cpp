#include "pch.h"
#include "res/SPInputInterface.hpp"
#include "GLCommon.hpp"

static GLID CreateVAO();
static inline GLuint SPInputSizeToNative(kt::SPInputSize size);
static inline GLuint SPInputTypeToNative(kt::SPInputType type);
static inline size_t SPInputSizeBytes(kt::SPInputType type, kt::SPInputSize size);

namespace kt
{
  SPInputInterface::SPInputInterface(std::_Uninitialized)
    : m_id{}, m_inputs{} {
  }

  SPInputInterface::SPInputInterface()
    : m_id{ CreateVAO() }, m_inputs{} {
  }

  SPInputInterface::~SPInputInterface() noexcept {
    glDeleteVertexArrays(1, &m_id);
  }

  SPInputInterface::SPInputInterface(SPInputInterface &&move) noexcept
    : m_id{ move.m_id }, m_inputs{ move.m_inputs } {
  }

  SPInputInterface &SPInputInterface::operator=(SPInputInterface &&move) noexcept {
    this->~SPInputInterface();

    m_id = move.m_id;
    m_inputs = move.m_inputs;

    move.m_id = NULL;

    return *this;
  }

  SPInputInterface::SPInputInterface(const SPInputInterface &copy) noexcept
    : m_id{ CreateVAO() }, m_inputs{ copy.m_inputs } {
  }

  SPInputInterface &SPInputInterface::operator=(const SPInputInterface &copy) noexcept {
    if (std::addressof(copy) == this)
    {
      return *this;
    }

    this->~SPInputInterface();

    m_id = CreateVAO();
    m_inputs = copy.m_inputs;

    return *this;
  }

  size_t SPInputInterface::calculate_vertex_size() const {
    size_t acumm = 0;
    for (size_t i = 0; i < m_inputs.size(); i++)
    {
      acumm += SPInputSizeBytes(m_inputs[i].type, m_inputs[i].size);
    }
    return acumm;
  }

  SPInputInterface::InputsList &SPInputInterface::modifiable_inputs() {
    this->mark_dirty();
    return m_inputs;
  }

  void SPInputInterface::_update() {
    glBindVertexArray(m_id);

    // disable un-needed locations
    for (size_t i = m_inputs.size(); i < m_inputs.max_size(); i++)
    {
      glDisableVertexAttribArray(i);
    }

    const size_t stride = calculate_vertex_size();

    intptr_t current_offset = 0;
    for (size_t i = 0; i < m_inputs.size(); i++)
    {
      const auto &input = m_inputs[i];
      glVertexAttribPointer(
        i,
        SPInputSizeToNative(input.size),
        SPInputTypeToNative(input.type),
        input.normalized,
        input.stride + stride,
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

inline GLuint SPInputSizeToNative(kt::SPInputSize size) {
  return GLuint(size);
}

inline GLuint SPInputTypeToNative(kt::SPInputType type) {
  switch (type)
  {
  case kt::SPInputType::Float:
    return GL_FLOAT;
  case kt::SPInputType::Double:
    return GL_DOUBLE;
  case kt::SPInputType::Byte:
    return GL_BYTE;
  case kt::SPInputType::Short:
    return GL_SHORT;
  case kt::SPInputType::Int:
    return GL_INT;
  case kt::SPInputType::UByte:
    return GL_UNSIGNED_BYTE;
  case kt::SPInputType::UShort:
    return GL_UNSIGNED_SHORT;
  case kt::SPInputType::UInt:
    return GL_UNSIGNED_INT;
  default:
    return GL_FLOAT;
  }
}

inline size_t SPInputSizeBytes(kt::SPInputType type, kt::SPInputSize size) {
  const size_t sz = SPInputSizeToNative(size);
  switch (type)
  {
  case kt::SPInputType::Double:
    return 8 * sz;
  case kt::SPInputType::Float:
  case kt::SPInputType::Int:
  case kt::SPInputType::UInt:
    return 4 * sz;
  case kt::SPInputType::Short:
  case kt::SPInputType::UShort:
    return 2 * sz;
  case kt::SPInputType::Byte:
  case kt::SPInputType::UByte:
    return sz;
  default:
    return 4 * sz;
  }
}
