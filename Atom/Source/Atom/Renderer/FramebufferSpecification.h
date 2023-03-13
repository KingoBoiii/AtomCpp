#pragma once
#include "Texture2DSpecification.h"

namespace Atom
{

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(TextureFormat textureFormat)
			: TextureFormat(textureFormat)
		{
		}

		TextureFormat TextureFormat = TextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments)
		{
		}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		float* ClearColor;
		uint32_t Width;
		uint32_t Height;

		FramebufferAttachmentSpecification Attachments;
	};

}