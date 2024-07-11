#pragma once
#include "Object.hpp"

namespace kt
{

  class Scene
  {
  public:
    typedef std::unique_ptr<Object> SPObject;

     

  private:
    std::vector<size_t> m_roots;
    std::vector<SPObject> m_objects;

    std::vector<size_t> m_update_order;
  };

}
