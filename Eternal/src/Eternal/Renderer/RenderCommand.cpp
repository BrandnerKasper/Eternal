#include "ETpch.h"

#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Eternal {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}