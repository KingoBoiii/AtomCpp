#pragma once

namespace Atom
{

	struct FramebufferOptions
	{
		float* ClearColor;
		uint32_t Width;
		uint32_t Height;
	};

	class Framebuffer
	{
	public:
		static Framebuffer* Create(const FramebufferOptions& framebufferOptions);
	public:
		virtual void Resize(uint32_t width, uint32_t height, uint32_t x = 0, uint32_t y = 0) = 0;

		virtual void Clear() const = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		
		virtual void* GetImage() const = 0;
		virtual FramebufferOptions GetOptions() const = 0;
	};

}
