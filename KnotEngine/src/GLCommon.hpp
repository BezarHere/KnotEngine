#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utility/Log.hpp"
#include "res/Resource.hpp"

using kt::GLID;

#define LOG_ERR_GLFW { const char *glfw_err_msg = nullptr; KT_LOG_ERR_V(glfwGetError(&glfw_err_msg), "GLFW Error: %s", glfw_err_msg); }

class OpenGL
{
public:
  static errno_t Start();
  static errno_t Stop();

  static errno_t _Impl_CheckGL(const char *msg, ...);
};
