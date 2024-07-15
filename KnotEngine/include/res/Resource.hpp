#pragma once
#include "utility/FieldVar.hpp"
#include <inttypes.h>

namespace kt
{
  typedef uint32_t GLID;


  class Resource
  {
  public:
    typedef uint64_t OwnerID;

    inline virtual ~Resource() {}

    // returns Null fieldvar if not serializable
    inline virtual FieldVar serialize() const { return nullptr; }

    inline OwnerID get_owner() const { return m_owner; }


  protected:
    OwnerID m_owner = 0;
  };


  class GraphicsResource : public Resource
  {
  public:
    inline GraphicsResource(GraphicsResource &&move) : m_id{ move.m_id } {
      move.m_id = NULL;
    }

    // specified by child classes
    inline GraphicsResource &operator=(GraphicsResource &&move) = delete;
    inline GraphicsResource(const GraphicsResource &copy) = delete;
    inline GraphicsResource &operator=(const GraphicsResource &copy) = delete;

    inline bool is_valid() const { return m_id != NULL; }
    inline GLID get_id() const { return m_id; }

  protected:
    inline GraphicsResource(GLID id = NULL) : m_id{ id } {}

  protected:
    GLID m_id = NULL;
  };

}
