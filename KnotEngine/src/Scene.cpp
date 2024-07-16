#include "pch.h"
#include "Scene.hpp"



void kt::Scene::rebuild() {
  m_hierarchy_map.clear();
  m_hierarchy.clear();
  m_roots.clear();

  this->_put_roots(m_roots);

  /*
  * Scene ex:
  *  -A
  *   +-B
  *   +-C
  *     +-D
  *  -E
  *   +-F
  */

  std::vector<size_t> result_stack{};
  std::vector<size_t> order_stack{};

  /*
  * Update hierarchy
  *    + EXPECTED: ABCDEF
  *
  *   result stack: []
  *   order stack start: [EA] (reversed roots)
  *    -> after popping each, add it's children (reversed) to the order stack
  *   result stack: [A]
  *   order stack start: [ECB]
  *    -> AGAIN
  *   result stack: [AB]
  *   order stack start: [EC]
  *    -> AGAIN
  *   result stack: [ABC]
  *   order stack start: [ED]
  *    -> AGAIN UNTIL EMPTY ORDER STACK
  *   result stack: [ABCDEF]
  *   order stack start: []
  *    -> FINISHED!
  *
  */

  const size_t roots_count = m_roots.size();
  for (size_t i = 1; i <= roots_count; i++)
  {
    order_stack.push_back(m_roots[roots_count - i]);
  }

  while (!order_stack.empty())
  {
    const size_t current_ind = order_stack.back();
    order_stack.pop_back();
    result_stack.push_back(current_ind);
    const Object *obj = m_objects[current_ind].get();

    const size_t children_count = obj->m_children.size();
    for (size_t i = 1; i <= children_count; i++)
    {
      const auto &child_link = obj->m_children[children_count - i];

      // validate child
      if (child_link.scene_index == npos)
      {
        continue;
      }

      order_stack.push_back(child_link.scene_index);
    }

  }

}

void kt::Scene::_put_roots(std::vector<size_t> &out_roots) const {
  for (size_t i = 0; i < m_objects.size(); i++)
  {
    if (m_objects[i]->m_parent.scene_index == npos)
    {
      out_roots.push_back(i);
    }
  }
}
