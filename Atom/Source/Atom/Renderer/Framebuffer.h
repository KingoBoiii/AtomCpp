#pragma once
#include "FramebufferSpecification.h"
#include "Texture2D.h"

namespace Atom
{

	class Framebuffer
	{
	public:
		static Framebuffer* Create(const FramebufferSpecification& specification);
	public:
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void Clear() const = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int32_t value) const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual int32_t ReadPixel(uint32_t attachmentIndex, int32_t x, int32_t y) const = 0;
		
		virtual void* GetColorAttachment(uint32_t attachmentIndex) const = 0;
		virtual void* GetImage() const = 0;
		virtual FramebufferSpecification GetOptions() const = 0;
	};

}
