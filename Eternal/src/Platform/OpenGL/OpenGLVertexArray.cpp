#include "ETpch.h"

#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Eternal {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Eternal::ShaderDataType::Float:  return GL_FLOAT;
		case Eternal::ShaderDataType::Float2: return GL_FLOAT;
		case Eternal::ShaderDataType::Float3: return GL_FLOAT;
		case Eternal::ShaderDataType::Float4: return GL_FLOAT;
		case Eternal::ShaderDataType::Mat3:	  return GL_FLOAT;
		case Eternal::ShaderDataType::Mat4:	  return GL_FLOAT;
		case Eternal::ShaderDataType::Int:	  return GL_INT;
		case Eternal::ShaderDataType::Int2:	  return GL_INT;
		case Eternal::ShaderDataType::Int3:	  return GL_INT;
		case Eternal::ShaderDataType::Int4:	  return GL_INT;
		case Eternal::ShaderDataType::Bool:	  return GL_BOOL;
		default:
			ET_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void Eternal::OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void Eternal::OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void Eternal::OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		ET_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no Layout!");

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void Eternal::OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
