#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utility/Log.hpp"
#include "res/Resource.hpp"

using kt::GLID;
using kt::npos;

#define LOG_ERR_GLFW { const char *glfw_err_msg = nullptr; KT_LOG_ERR_V(glfwGetError(&glfw_err_msg), "GLFW Error: %s", glfw_err_msg); }
#define CHECK_OPENGL(msg) OpenGL::_Impl_CheckGL(":%s:%d: " msg, __FUNCTION__, __LINE__)
#define CHECK_OPENGL_V(msg, ...) OpenGL::_Impl_CheckGL(":%s:%d: " msg, __FUNCTION__, __LINE__, __VA_ARGS__)

class OpenGL
{
public:
  static constexpr size_t LogBufferSz = 512;
  typedef char LogBuffer[LogBufferSz];

  static errno_t Start();
  static errno_t Stop();

  static errno_t _Impl_CheckGL(const char *msg, ...);
};
