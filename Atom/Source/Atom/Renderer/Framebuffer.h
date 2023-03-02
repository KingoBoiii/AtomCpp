#pragma once

namespace Atom
{

	enum class FramebufferTextureFormat
	{
		None = 0,
		
		RGBA8,
		RED_INTEGER,

		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureOptions
	{
		FramebufferTextureOptions() = default;
		FramebufferTextureOptions(FramebufferTextureFormat textureFormat)
			: TextureFormat(textureFormat)
		{
		}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};
	
	struct FramebufferAttachmentOptions
	{
		FramebufferAttachmentOptions() = default;
		FramebufferAttachmentOptions(std::initializer_list<FramebufferTextureOptions> attachments)
			: Attachments(attachments)
		{
		}

		std::vector<FramebufferTextureOptions> Attachments;
	};

	struct FramebufferOptions
	{
		float* ClearColor;
		uint32_t Width;
		uint32_t Height;

		FramebufferAttachmentOptions Attachments;
	};

	class Framebuffer
	{
	public:
		static Framebuffer* Create(const FramebufferOptions& framebufferOptions);
	public:
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void Clear() const = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int32_t value) const = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual int32_t ReadPixel(uint32_t attachmentIndex, int32_t x, int32_t y) const = 0;
		
		virtual void* GetColorAttachment(uint32_t attachmentIndex) const = 0;
		virtual void* GetImage() const = 0;
		virtual FramebufferOptions GetOptions() const = 0;
	};

}
