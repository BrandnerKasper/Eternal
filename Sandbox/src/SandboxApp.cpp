#include "ETpch.h"

#include <Eternal.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"


class ExampleLayer : public Eternal::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f), m_PlayerPosition(0,0,0)
	{
		m_VertexArray = Eternal::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.7f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,	0.0f, 0.7f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 0.7f, 1.0f
		};

		Eternal::Ref<Eternal::VertexBuffer> vertexBuffer;
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
		Eternal::Ref<Eternal::IndexBuffer> indexBuffer;
		indexBuffer.reset(Eternal::IndexBuffer::Create(indices, count));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;			

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_Shader = Eternal::Shader::Create("Triangle Shader", vertexSrc, fragmentSrc);



		//test square <3
		m_SquareVertexArray = Eternal::VertexArray::Create();

		float squaredVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Eternal::Ref<Eternal::VertexBuffer> squaredVertexBuffer;
		squaredVertexBuffer.reset(Eternal::VertexBuffer::Create(squaredVertices, sizeof(squaredVertices)));
		Eternal::BufferLayout squareLayout = {
			{Eternal::ShaderDataType::Float3, "a_Position"},
			{Eternal::ShaderDataType::Float2, "a_TexCoord"}
		};
		squaredVertexBuffer->SetLayout(squareLayout);
		m_SquareVertexArray->AddVertexBuffer(squaredVertexBuffer);

		uint32_t squaredIndices[6] = { 0, 1, 2, 2, 3, 0 };
		uint32_t squareCount = sizeof(squaredIndices) / sizeof(uint32_t);
		Eternal::Ref<Eternal::IndexBuffer> squaredIndexBuffer;
		squaredIndexBuffer.reset(Eternal::IndexBuffer::Create(squaredIndices, squareCount));
		m_SquareVertexArray->SetIndexBuffer(squaredIndexBuffer);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader = Eternal::Shader::Create("Flat Shader", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Eternal::Texture2D::Create("assets/textures/Checkerboard.png");
		m_EternalLogo = Eternal::Texture2D::Create("assets/textures/EternalLogo.png");

		textureShader->Bind();
		textureShader->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Eternal::Timestep ts) override
	{
		//ET_TRACE("Delta time {0}s ({1} milisecs) ", ts.GetSeconds(), ts.GetMilliseconds());

		//Update
		m_CameraController.OnUpdate(ts);

		//Test obj controller
		if (Eternal::Input::IsKeyPressed(ET_KEY_J))
			m_PlayerPosition.x -= m_PlayerMoveSpeed * ts;
		else if (Eternal::Input::IsKeyPressed(ET_KEY_L))
			m_PlayerPosition.x += m_PlayerMoveSpeed * ts;

		if (Eternal::Input::IsKeyPressed(ET_KEY_I))
			m_PlayerPosition.y += m_PlayerMoveSpeed * ts;
		else if (Eternal::Input::IsKeyPressed(ET_KEY_K))
			m_PlayerPosition.y -= m_PlayerMoveSpeed * ts;

		//Render
		Eternal::RenderCommand::SetClearColor({ 0.7f, 0.7f, 0.7f, 1 });
		Eternal::RenderCommand::Clear();

		Eternal::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		glm::mat4 playercontroller = glm::translate(glm::mat4(1.0f), m_PlayerPosition) * scale;

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		m_FlatColorShader->Bind();
		m_FlatColorShader->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for(int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.25f, y * 0.25f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				Eternal::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
				//ET_WARN("{0}", pos.x);
			}
		}
		
		Eternal::Renderer::Submit(m_Shader, m_VertexArray, playercontroller);


		//Texture Example
		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		glm::mat4 scaletex = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
		glm::vec3 postex(-2.25f, -2.25f, 0.0f);
		glm::mat4 transformtex = glm::translate(glm::mat4(1.0f), postex) * scaletex;
		Eternal::Renderer::Submit(textureShader, m_SquareVertexArray, transformtex);
		
		//add Logo
		m_EternalLogo->Bind();
		Eternal::Renderer::Submit(textureShader, m_SquareVertexArray, transformtex);

		Eternal::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override //Test that sandbox can use ImGui itself <3
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Eternal::Event& event) override
	{
		if (event.GetEventType() == Eternal::EventType::KeyPressed)
		{
			Eternal::KeyPressedEvent& e = (Eternal::KeyPressedEvent&)event;
			ET_INFO("{0}, {1}", (char)e.GetKeyCode(), e.GetRepeatCount());
		}

		m_CameraController.OnEvent(event);
	}

private:
	Eternal::ShaderLibrary m_ShaderLibrary;

	Eternal::Ref<Eternal::Shader> m_Shader;
	Eternal::Ref<Eternal::VertexArray> m_VertexArray;

	Eternal::Ref<Eternal::Shader> m_FlatColorShader;
	Eternal::Ref<Eternal::VertexArray> m_SquareVertexArray;

	Eternal::Ref<Eternal::Texture2D> m_Texture, m_EternalLogo;

	Eternal::OrthographicCameraController m_CameraController;

	glm::vec3 m_PlayerPosition;
	float m_PlayerMoveSpeed = 1.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f }; 
};

class Sandbox : public Eternal::Application
{
public:
	Sandbox() 
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() 
	{

	}
};

Eternal::Application* Eternal::CreateApplication()
{
	return new Sandbox();
}