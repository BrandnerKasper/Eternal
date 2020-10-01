#pragma once

#include "Eternal/Core/Core.h"

namespace Eternal {

	struct FrameBufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class FrameBuffer 
	{
	public:
		virtual ~FrameBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};
}