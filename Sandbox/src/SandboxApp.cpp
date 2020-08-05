#include "ETpch.h"

#include <Eternal.h>

#include "imgui/imgui.h"

class ExampleLayer : public Eternal::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0,0,0)
	{
		m_VertexArray.reset(Eternal::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.7f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,	0.0f, 0.7f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 0.7f, 1.0f
		};

		std::shared_ptr<Eternal::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Eternal::VertexBuffer::Create(vertices, sizeof(vertices)));
		Eternal::BufferLayout layout = {
			{Eternal::ShaderDataType::Float3, "a_Position"},
			{Eternal::ShaderDataType::Float4, "a_Color"},
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//index Buffer
		uint32_t indices[3] = { 0, 1, 2 };
		uint32_t count = sizeof(indices) / sizeof(uint32_t);
		std::shared_ptr<Eternal::IndexBuffer> indexBuffer;
		indexBuffer.reset(Eternal::IndexBuffer::Create(indices, count));
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

		m_Shader.reset(Eternal::Shader::Create(vertexSrc, fragmentSrc));



		//test square <3
		m_SquareVertexArray.reset(Eternal::VertexArray::Create());

		float squaredVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Eternal::VertexBuffer> squaredVertexBuffer;
		squaredVertexBuffer.reset(Eternal::VertexBuffer::Create(squaredVertices, sizeof(squaredVertices)));
		Eternal::BufferLayout squareLayout = {
			{Eternal::ShaderDataType::Float3, "a_Position"},
		};
		squaredVertexBuffer->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squaredVertexBuffer);

		uint32_t squaredIndices[6] = { 0, 1, 2, 2, 3, 0 };
		uint32_t squareCount = sizeof(squaredIndices) / sizeof(uint32_t);
		std::shared_ptr<Eternal::IndexBuffer> squaredIndexBuffer;
		squaredIndexBuffer.reset(Eternal::IndexBuffer::Create(squaredIndices, squareCount));
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

		m_BlueShader.reset(Eternal::Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void OnUpdate(Eternal::Timestep ts) override
	{
		//ET_TRACE("Delta time {0}s ({1} milisecs) ", ts.GetSeconds(), ts.GetMilliseconds());

		if (Eternal::Input::IsKeyPressed(ET_KEY_LEFT) 
			|| Eternal::Input::IsKeyPressed(ET_KEY_A))
			m_CameraPosition.x += m_CameraSpeed * ts;
		else if (Eternal::Input::IsKeyPressed(ET_KEY_RIGHT) 
			|| Eternal::Input::IsKeyPressed(ET_KEY_D))
			m_CameraPosition.x -= m_CameraSpeed * ts;

		if (Eternal::Input::IsKeyPressed(ET_KEY_UP) 
			|| Eternal::Input::IsKeyPressed(ET_KEY_W))
			m_CameraPosition.y -= m_CameraSpeed * ts;
		else if (Eternal::Input::IsKeyPressed(ET_KEY_DOWN) 
			|| Eternal::Input::IsKeyPressed(ET_KEY_S))
			m_CameraPosition.y += m_CameraSpeed * ts;

		if (Eternal::Input::IsKeyPressed(ET_KEY_E))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Eternal::Input::IsKeyPressed(ET_KEY_Q))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Eternal::RenderCommand::SetClearColor({ 0.7f, 0.7f, 0.7f, 1 });
		Eternal::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Eternal::Renderer::BeginScene(m_Camera);

		Eternal::Renderer::Submit(m_BlueShader, m_SquareVertexArray);
		Eternal::Renderer::Submit(m_Shader, m_VertexArray);


		Eternal::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override //Test that sandbox can use ImGui itself <3
	{

	}

	void OnEvent(Eternal::Event& event) override
	{
		if (event.GetEventType() == Eternal::EventType::KeyPressed)
		{
			Eternal::KeyPressedEvent& e = (Eternal::KeyPressedEvent&)event;
			ET_INFO("{0}, {1}", (char)e.GetKeyCode(), e.GetRepeatCount());
		}

		Eternal::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Eternal::KeyPressedEvent>(ET_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Eternal::KeyPressedEvent& event)
	{
		

		return false;
	}

private:
	std::shared_ptr<Eternal::Shader> m_Shader;
	std::shared_ptr<Eternal::VertexArray> m_VertexArray;

	std::shared_ptr<Eternal::Shader> m_BlueShader;
	std::shared_ptr<Eternal::VertexArray> m_SquareVertexArray;

	Eternal::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 3.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;
};

class Sandbox : public Eternal::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() 
	{

	}
};

Eternal::Application* Eternal::CreateApplication()
{
	return new Sandbox();
}