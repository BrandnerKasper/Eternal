#pragma once

#include <memory>
#include "Eternal/Renderer/Buffer.h"

namespace Eternal {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(SharedPtr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(SharedPtr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffer() const = 0;
		virtual const SharedPtr<IndexBuffer>& GetIndexBuffer() const = 0;

		static SharedPtr<VertexArray> Create();
	};

}