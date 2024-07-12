#include "pch.h"
#include "Scene.hpp"



void kt::Scene::rebuild() {
  m_hierarchy_map.clear();
  m_hierarchy.clear();

  std::vector<size_t> order_stack{};
  std::vector<size_t> listing{};

  const size_t roots_count = m_roots.size();
  for (size_t i = 0; i < roots_count; i++)
  {
    order_stack.push_back(m_roots[i]);
  }

  while (!order_stack.empty())
  {
    const size_t current_ind = order_stack.back();
    order_stack.pop_back();
    listing.push_back(current_ind);
    const Object *obj = m_objects[current_ind].get();

    for (size_t i = 0; i < obj->m_children.size(); i++)
    {
      if (obj->m_children[i].scene_index == npos)
      {
        continue;
      }

      order_stack.push_back(obj->m_children[i].scene_index);
    }

  }

}
