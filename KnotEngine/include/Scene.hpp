#pragma once
#include "Object.hpp"

namespace kt
{

  class Scene
  {
  public:
    typedef std::unique_ptr<Object> SPObject;

    struct ObjectStateFrame
    {
      Object::State state = {};
      size_t obj_index;

      size_t parent_frame = npos;
    };

    void rebuild();

  private:
    std::vector<size_t> m_roots;
    std::vector<SPObject> m_objects;

    std::vector<size_t> m_hierarchy_map;
    std::vector<ObjectStateFrame> m_hierarchy;
  };

}
