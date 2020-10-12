#include "ETpch.h"

#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Eternal {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TextureID;
		float TilingFactor;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 1000; //per single drawcall depending on the GPU
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; //Most common GPUs on stationar devices have at least 32 but to be precise we can add a fct that tells us how many texture slot our system has

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TwoInOneShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white Texture!!

		glm::vec4 QuadVertexPositions[4];
		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{

		//test square <3
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer= VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		BufferLayout quadLayout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexID"},
			{ShaderDataType::Float, "a_TilFactor"}
		};
		s_Data.QuadVertexBuffer->SetLayout(quadLayout);
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (int32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TwoInOneShader = Shader::Create("assets/shaders/2in1TextureAndColor.glsl");
		s_Data.TwoInOneShader->Bind();
		s_Data.TwoInOneShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data.TwoInOneShader->Bind();
		s_Data.TwoInOneShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
		
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0)
			return; // Nothing to draw

		//Bind Textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

		s_Data.Stats.DrawCalls++;
	}

	//TODO: Refactor DrawQuad fct to take in transform and spriteComponent? 

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		constexpr glm::mat4 std_Mat = glm::mat4(1.0f);

		glm::mat4 transform = glm::translate(std_Mat, position)
			* glm::scale(std_Mat, { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		//Check if too many Quads got drawn!
		if (TooManyQuads())
		{
			StartNewBatch();
		}

		const float texID = 0; //White Texture!
		const float tilingFactor = 1.0f;

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TextureID = texID;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		// if Statistics are on!!
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const int textureScale, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, textureScale, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const int textureScale, const glm::vec4& tintColor)
	{
		constexpr glm::mat4 std_Mat = glm::mat4(1.0f);

		glm::mat4 transform = glm::translate(std_Mat, position)
			* glm::scale(std_Mat, { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, textureScale, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const int textureScale, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		//Check if too many Quads got drawn or textures!
		if (TooManyQuads() || TooManyTextures())
		{
			StartNewBatch();
		}

		glm::vec4 color = tintColor;

		float texID = GetTextureSlot(texture);

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TextureID = texID;
			s_Data.QuadVertexBufferPtr->TilingFactor = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		// if Statistics are on!!
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0 }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr glm::mat4 std_Mat = glm::mat4(1.0f);

		//Check if too many Quads got drawn!
		if (TooManyQuads())
		{
			StartNewBatch();
		}

		const float texID = 0; //White Texture!
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(std_Mat, position)
			* glm::rotate(std_Mat, glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(std_Mat, { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TextureID = texID;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		// if Statistics are on!!
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const int textureScale, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0 }, size, rotation, texture, textureScale, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const int textureScale, const glm::vec4& tintColor)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		//Check if too many Quads got drawn or textures!
		if (TooManyQuads() || TooManyTextures())
		{
			StartNewBatch();
		}

		glm::vec4 color = tintColor;

		float texID = GetTextureSlot(texture);

		glm::mat4 std_Mat = glm::mat4(1.0f);
		glm::mat4 transform = glm::translate(std_Mat, position)
			* glm::rotate(std_Mat, glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(std_Mat, { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TextureID = texID;
			s_Data.QuadVertexBufferPtr->TilingFactor = textureScale;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		// if Statistics are on!!
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::StartNewBatch()
	{
		EndScene();
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	bool Renderer2D::TooManyQuads()
	{
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			return true;
		else
			return false;
	}

	bool Renderer2D::TooManyTextures()
	{
		if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			return true;
		return false;
	}

	float Renderer2D::GetTextureSlot(const Ref<Texture2D>& texture)
	{
		float texID = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				texID = (float)i;
				return texID;
			}
		}

		if (texID == 0.0f)
		{
			texID = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		return texID;
	}

	
}