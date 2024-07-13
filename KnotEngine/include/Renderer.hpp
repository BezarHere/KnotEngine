#pragma once
#include "Window.hpp"
#include "res/StorageBuffer.hpp"
#include "math/Transform.hpp"
#include "math/Color.hpp"

#include "RenderCommand.hpp"

namespace kt
{

  class Renderer
  {
  public:
    Renderer(Window &window);
    
    void clear();
    void flush();

    void draw_rect(const Rect &rect, const Color &color);

    Color clear_color = { 0.4F, 0.2F, 0.2F };
    Transform3D world_transform;
  private:
    GLID m_id;
    Window &m_window;
  };

}
