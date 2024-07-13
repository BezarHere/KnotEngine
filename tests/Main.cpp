#include <Engine.hpp>

int main() {
  kt::WindowAttributes attributes{};
  attributes.title = "test - KnotEngine";
  kt::Window window{attributes};
  kt::Renderer renderer{ window };

  while (!window.closed())
  {
    renderer.clear();
    renderer.flush();
    window.poll();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

}
