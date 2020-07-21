#pragma once

namespace Eternal {
	
	enum class RendererAPI
	{
		None = 0, OpenGl = 1
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};

}