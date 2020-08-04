#include "ETpch.h"

#include "Application.h"

#include "Eternal/Renderer/Renderer.h"

#include "Input.h"

namespace Eternal {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() 
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		ET_CORE_ASSERT(s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.7f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,	0.0f, 0.7f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 0.7f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//index Buffer
		uint32_t indices[3] = { 0, 1, 2 };
		uint32_t count = sizeof(indices) / sizeof(uint32_t);
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, count));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;			

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));



		//test square <3
		m_SquareVertexArray.reset(VertexArray::Create());

		float squaredVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squaredVertexBuffer;
		squaredVertexBuffer.reset(VertexBuffer::Create(squaredVertices, sizeof(squaredVertices)));
		BufferLayout squareLayout = {
			{ShaderDataType::Float3, "a_Position"},
		};
		squaredVertexBuffer->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squaredVertexBuffer);

		uint32_t squaredIndices[6] = { 0, 1, 2, 2, 3, 0 };
		uint32_t squareCount = sizeof(squaredIndices) / sizeof(uint32_t);
		std::shared_ptr<IndexBuffer> squaredIndexBuffer;
		squaredIndexBuffer.reset(IndexBuffer::Create(squaredIndices, squareCount));
		m_SquareVertexArray->SetIndexBuffer(squaredIndexBuffer);

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {
			//printf("Slaying Demons...");

			RenderCommand::SetClearColor({ 0.7f, 0.7f, 0.7f, 1 });
			RenderCommand::Clear();

			//m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
			//m_Camera.SetRotation(45.0f);

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_BlueShader, m_SquareVertexArray);
			Renderer::Submit(m_Shader, m_VertexArray);

			m_Camera.RotateCamera();

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		};
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}