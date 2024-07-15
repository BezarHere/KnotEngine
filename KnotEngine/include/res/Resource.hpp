#pragma once
#include <inttypes.h>

namespace kt
{
  typedef uint32_t GLID;


  class Resource
  {
  public:
    typedef uint64_t OwnerID;

    inline virtual ~Resource() {}

    inline OwnerID get_owner() const { return m_owner; }

  protected:
    OwnerID m_owner = 0;
  };

}
