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
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void Clear() const = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() = 0;
		
		virtual void* GetImage() const = 0;
	};

}
