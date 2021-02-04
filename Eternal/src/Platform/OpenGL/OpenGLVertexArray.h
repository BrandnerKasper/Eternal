#pragma once

#include "Eternal/Renderer/VertexArray.h"

namespace Eternal {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(SharedPtr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(SharedPtr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffer() const { return m_VertexBuffers; }
		virtual const SharedPtr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<SharedPtr<VertexBuffer>> m_VertexBuffers;
		SharedPtr<IndexBuffer> m_IndexBuffer;

	};

}