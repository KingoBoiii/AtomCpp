#pragma once
#include "Atom/Core/Base.h"
#include "Atom/Renderer/Texture2D.h"

namespace Atom
{

#define ReleasePtr(ptr) delete ptr; ptr = nullptr

	class EditorResources
	{
	public:
		inline static Texture2D* PlayIcon = nullptr;
		inline static Texture2D* StopIcon = nullptr;
	public:
		static void Initialize()
		{
			PlayIcon = LoadTexture("Viewport/Play.png");
			StopIcon = LoadTexture("Viewport/Stop.png");
		}

		static void Shutdown()
		{
			ReleasePtr(PlayIcon);
			ReleasePtr(StopIcon);
		}
	private:
		static Texture2D* LoadTexture(const std::filesystem::path& relativePath, const Texture2DOptions& textureOptions = Texture2DOptions())
		{
			std::filesystem::path path = std::filesystem::path("Resources") / "Editor" / relativePath;

			if(!std::filesystem::exists(path))
			{
				AT_CORE_CRITICAL("Failed to load icon '{0}'! The file doesn't exist!", path.string());
				AT_CORE_ASSERT(false);
				return nullptr;
			}

			return Texture2D::Create(path.string(), textureOptions);
		}
	};

}
