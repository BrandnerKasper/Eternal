#include "ETpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

#include <Glad/glad.h>

namespace Eternal {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		ET_CORE_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ET_CORE_ASSERT(status, "Failed to initialize Glad!");

		ET_CORE_WARN("OpenGL Info:");
		ET_CORE_INFO(" Vendor: {0}", glGetString(GL_VENDOR));
		ET_CORE_INFO(" Renderer: {0}", glGetString(GL_RENDERER));
		ET_CORE_INFO(" Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

	float OpenGLContext::OpenGLGetTime()
	{
		return glfwGetTime();
	}
}