#include "pch.h"
#include "Window.hpp"
#include "GLCommon.hpp"
#include <vector>

using namespace kt;

typedef GLFWwindow *GLWindowHandle;



static std::vector<const Window *> s_windows{};

static inline GLFWwindow *create_window(const kt::WindowAttributes &attributes);

static inline Window *get_window_from_handle(Window::WindowHandle handle);

static inline void setup_window_callbacks(GLWindowHandle handle, Window *window);

namespace kt
{
  struct Window::CallbackServer
  {
    static void close(GLWindowHandle handle);
    static void resized(GLWindowHandle handle, int width, int height);
    static void moved(GLWindowHandle handle, int x_pos, int y_pos);

    static void focused(GLWindowHandle handle, int focused);
    static void minimized(GLWindowHandle handle, int minimized);
    static void maximized(GLWindowHandle handle, int maximized);
  };
}

namespace kt
{
  Window::Window(const WindowAttributes &attributes) : m_handle{ create_window(attributes) } {
    setup_window_callbacks((GLWindowHandle)m_handle, this);
  }

  void Window::poll() {
    glfwPollEvents();
  }
}

namespace kt
{
  void Window::CallbackServer::close(GLWindowHandle handle) {
    Window *const window = get_window_from_handle(handle);

    window->m_events.emplace_back(WindowEventType::Closed);
    window->m_state = WindowState::Closed;
  }

  void Window::CallbackServer::resized(GLWindowHandle handle, int width, int height) {
    Window *const window = get_window_from_handle(handle);
    window->m_events.emplace_back(
      WindowEventType::Resized,
      WindowEventData{ width, height }
    );
  }

  void Window::CallbackServer::moved(GLWindowHandle handle, int x_pos, int y_pos) {
    Window *const window = get_window_from_handle(handle);
    window->m_events.emplace_back(
      WindowEventType::Moved,
      WindowEventData{ x_pos, y_pos }
    );
  }

  void Window::CallbackServer::focused(GLWindowHandle handle, int focused) {
    Window *const window = get_window_from_handle(handle);
    if (focused == 0)
    {
      window->m_events.emplace_back(WindowEventType::Unfocused);
      return;
    }

    window->m_events.emplace_back(WindowEventType::Focused);

  }

  void Window::CallbackServer::minimized(GLWindowHandle handle, int minimized) {
    Window *const window = get_window_from_handle(handle);
    if (window->closed())
    {
      return;
    }

    if (minimized == 0)
    {
      window->m_state = WindowState::Restored;
      window->m_events.emplace_back(WindowEventType::Restored);
      return;
    }

    window->m_state = WindowState::Minimized;
    window->m_events.emplace_back(WindowEventType::Minimized);
  }

  void Window::CallbackServer::maximized(GLWindowHandle handle, int maximized) {
    Window *const window = get_window_from_handle(handle);
    if (window->closed())
    {
      return;
    }

    if (maximized == 0)
    {
      window->m_state = WindowState::Restored;
      window->m_events.emplace_back(WindowEventType::Restored);
      return;
    }

    window->m_state = WindowState::Maximized;
    window->m_events.emplace_back(WindowEventType::Maximized);
  }
}

inline GLFWwindow *create_window(const kt::WindowAttributes &attributes) {
  // currently, full screen unsupported
  GLFWmonitor *monitor = attributes.mode == WindowMode::FullScreen ? nullptr : nullptr;

  OpenGL::InitGLFW();

  OpenGL::SetupWindowHints();

  GLFWwindow *window = glfwCreateWindow(
    attributes.size.x, attributes.size.y, attributes.title,
    monitor, nullptr
  );

  if (window == nullptr)
  {
    LOG_ERR_GLFW;
    return nullptr;
  }

  glfwMakeContextCurrent(window);

  OpenGL::InitGLEW();

  glfwMakeContextCurrent(NULL);


  return window;
}

inline Window *get_window_from_handle(Window::WindowHandle handle) {
  return reinterpret_cast<Window *>(
    glfwGetWindowUserPointer(reinterpret_cast<GLFWwindow *>(handle))
    );
}

inline void setup_window_callbacks(GLWindowHandle handle, Window *window) {
  glfwSetWindowUserPointer(handle, window);

  glfwSetWindowCloseCallback(handle, Window::CallbackServer::close);

  glfwSetWindowPosCallback(handle, Window::CallbackServer::moved);
  glfwSetWindowSizeCallback(handle, Window::CallbackServer::resized);

  glfwSetWindowFocusCallback(handle, Window::CallbackServer::focused);

  glfwSetWindowIconifyCallback(handle, Window::CallbackServer::minimized);
  glfwSetWindowMaximizeCallback(handle, Window::CallbackServer::maximized);
}
