#include "pch.h"
#include "GLCommon.hpp"

#include <stdarg.h>

errno_t OpenGL::Start() {

  const int glfw_init_result = glfwInit();
  if (glfw_init_result == GLFW_FALSE)
  {
    LOG_ERR_GLFW;
  }

  const int glew_init_result = glewInit();

  return errno_t();
}

errno_t OpenGL::Stop() {
  return errno_t();
}

errno_t OpenGL::_Impl_CheckGL(const char *msg, ...) {
  errno_t error = glGetError();

  if (error == 0)
  {
    return 0;
  }

  GL_INVALID_ENUM;
  fprintf(stdout, "GlError[%s]", KT_GetErrorName(error));

  va_list list;
  va_start(list, msg);
  vfprintf(stdout, msg, list);
  va_end(list);
  
  return error;
}
