#include "pch.h"
#include "GLCommon.hpp"

#include <array>
#include <stdarg.h>

static void GLFWErrorCallback(int error_code, const char *description);
static void GLDebugTraceback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

errno_t OpenGL::InitGLFW() {

  const int glfw_init_result = glfwInit();
  if (glfw_init_result == GLFW_FALSE)
  {
    LOG_ERR_GLFW;
    return EFAULT;
  }

  glfwSetErrorCallback(GLFWErrorCallback);

  return EOK;
}

errno_t OpenGL::InitGLEW() {
  const int glew_init_result = glewInit();

  if (glew_init_result != EOK)
  {
    return kt::Log::error(glew_init_result, "glewInit: %s", glewGetErrorString(glew_init_result));
  }

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(GLDebugTraceback, nullptr);

  return EOK;
}

errno_t OpenGL::SetupWindowHints() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLVersionMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  return EOK;
}

errno_t OpenGL::Stop() {
  glfwTerminate();
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

void GLFWErrorCallback(int error_code, const char *description) {
  kt::Log::error(error_code, "GLFW error: %s", description);
}

constexpr std::array<const char *, 6> DebugSources =
{
  "API", "WindowSys", "ShaderCompiler", "3rdParty", "Application", "Other"
};

constexpr std::array<const char *, 9> DebugMsgTypes =
{
  "Error", "DeprecatedBehavior", "UndefinedBehavior", "Portability", "Performance", "Marker",
  "PushGroup", "PopGroup", "Other"
};

constexpr std::array<const char *, 4> DebugMsgSeverity =
{
  "High", "Med", "Low", "None"
};

static constexpr const char *gldbg_get_severity(const GLenum s) {
  if (s == GL_DEBUG_SEVERITY_NOTIFICATION)
    return DebugMsgSeverity[3];
  return DebugMsgSeverity[(size_t)(s - GL_DEBUG_SEVERITY_HIGH)];
}


void GLDebugTraceback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return; // skips notifications

  (void)userParam;
  (void)id;

  const auto callback = (severity == GL_DEBUG_SEVERITY_HIGH) ? kt::Log::error : kt::Log::warning;

  callback(
    EFAULT,
    " [%s] [%s] from %s: %.*s",
    gldbg_get_severity(severity),

    DebugMsgTypes[size_t(type - GL_DEBUG_TYPE_ERROR)],

    DebugSources[size_t(source - GL_DEBUG_SOURCE_API)],

    length,
    message
  );
}
