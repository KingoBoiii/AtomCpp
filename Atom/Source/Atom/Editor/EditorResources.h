#pragma once
#include "Atom/Core/Base.h"
#include "Atom/Renderer/Texture2D.h"

namespace Atom
{

#define ReleasePtr(ptr) delete ptr; ptr = nullptr

	class EditorResources
	{
	public:
		inline static Texture2D* DirectoryIcon = nullptr;
		inline static Texture2D* FileIcon = nullptr;

		inline static Texture2D* PlayIcon = nullptr;
		inline static Texture2D* StopIcon = nullptr;
		inline static Texture2D* SimulateIcon = nullptr;
		inline static Texture2D* PauseIcon = nullptr;
		inline static Texture2D* StepIcon = nullptr;
	public:
		static void Initialize()
		{
			DirectoryIcon = LoadTexture("ProjectExplorer/Directory.png");
			FileIcon = LoadTexture("ProjectExplorer/File.png");

			PlayIcon = LoadTexture("Viewport/Play.png");
			StopIcon = LoadTexture("Viewport/Stop.png");
			SimulateIcon = LoadTexture("Viewport/Simulate.png");
			PauseIcon = LoadTexture("Viewport/Pause.png");
			StepIcon = LoadTexture("Viewport/Step.png");
		}

		static void Shutdown()
		{
			ReleasePtr(DirectoryIcon);
			ReleasePtr(FileIcon);
			
			ReleasePtr(PlayIcon);
			ReleasePtr(StopIcon);
			ReleasePtr(SimulateIcon);
			ReleasePtr(PauseIcon);
			ReleasePtr(StepIcon);
		}
	private:
		static Texture2D* LoadTexture(const std::filesystem::path& relativePath, const Texture2DSpecification& textureOptions = Texture2DSpecification())
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
