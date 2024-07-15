#pragma once
#include "Resource.hpp"
#include "utility/Blob.hpp"
#include "utility/ArrayList.hpp"

namespace kt
{
  enum class SPInputType : uint8_t
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

  enum class SPInputSize : uint8_t
  {
    Single = 1,


    Double,
    Vec2 = Double,

    Triple,
    Vec3 = Triple,

    Quad,
    Vec4 = Quad
  };

  struct SPInput
  {
    SPInputType type;
    SPInputSize size;

    bool normalized;

    intptr_t offset;
    uint32_t stride;
  };

  class Renderer;
  class SPInputInterface : public Resource
  {
    friend Renderer;
  public:
    static constexpr size_t MaxInputs = 8;
    typedef ArrayList<SPInput, MaxInputs> InputsList;

    SPInputInterface(std::_Uninitialized);
    SPInputInterface();
    ~SPInputInterface() noexcept;

    SPInputInterface(SPInputInterface &&move) noexcept;
    SPInputInterface &operator=(SPInputInterface &&move) noexcept;

    SPInputInterface(const SPInputInterface &copy) noexcept;
    SPInputInterface &operator=(const SPInputInterface &copy) noexcept;

    inline bool is_dirty() const { return m_dirty; }
    inline void mark_dirty() { m_dirty = true; }

    size_t calculate_vertex_size() const;

    inline Blob<const SPInput> get_inputs() const { return { m_inputs.data(), m_inputs.size() }; }

    // will mark this as dirty
    InputsList &modifiable_inputs();

  private:
    void _update();
  private:
    GLID m_id;
    bool m_dirty = true;
    InputsList m_inputs;
  };
}
