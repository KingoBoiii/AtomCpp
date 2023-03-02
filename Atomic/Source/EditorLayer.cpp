#include "EditorLayer.h"
#include <Atom/Scene/SceneSerializer.h>
#include <Atom/Utils/PlatformUtils.h>
#include <Atom/ImGui/ImGuiUtillities.h>
#include <Atom/ImGui/ImGuiStyle.h>
#include <Atom/Editor/EditorResources.h>
#include <Atom/Scripting/ScriptEngine.h>
#include <Atom/Renderer/Font.h>

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include "ImGuizmo.h"
#include <Atom/Project/ProjectSerializer.h>

#include <shellapi.h>

namespace Atom
{

#define ATOM_SCENE_FILE_EXTENSIONS "atsc"
#define ATOM_SCENE_FILE_DIALOG_FILTER "Atom Scene (*.atsc)\0*.atsc\0"

#define ATOM_PROJECT_FILE_EXTENSIONS "atpr"
#define ATOM_PROJECT_FILE_DIALOG_FILTER "Atom Project (*.atpr)\0*.atpr\0"

#define MAX_PROJECT_NAME_LENGTH 255
#define MAX_PROJECT_FILEPATH_LENGTH 512

	static char* s_ProjectNameBuffer = new char[MAX_PROJECT_NAME_LENGTH];
	static char* s_OpenProjectFilePathBuffer = new char[MAX_PROJECT_FILEPATH_LENGTH];
	static char* s_NewProjectFilePathBuffer = new char[MAX_PROJECT_FILEPATH_LENGTH];

	static Font* s_Font;

	static ImFont* s_TestFont = nullptr;
	static ImFont* s_TestFont1 = nullptr;

	void EditorLayer::OnAttach()
	{
		memset(s_ProjectNameBuffer, 0, MAX_PROJECT_NAME_LENGTH);
		memset(s_OpenProjectFilePathBuffer, 0, MAX_PROJECT_FILEPATH_LENGTH);
		memset(s_NewProjectFilePathBuffer, 0, MAX_PROJECT_FILEPATH_LENGTH);

		s_TestFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("Resources/Fonts/OpenSans/OpenSans-Bold.ttf", 36.0f);
		s_TestFont1 = ImGui::GetIO().Fonts->AddFontFromFileTTF("Resources/Fonts/OpenSans/OpenSans-Bold.ttf", 20.0f);

		//Font font("C:\\Windows\\Fonts\\Arial.ttf");
		//s_Font = new Font("Resources/Fonts/OpenSans/OpenSans-Regular.ttf");
		s_Font = Font::GetDefaultFont(); // new Font("C:\\Windows\\Fonts\\Arial.ttf");

		EditorResources::Initialize();

		Window* window = Application::Get().GetWindow();

		Atom::FramebufferOptions framebufferOptions{ };
		framebufferOptions.ClearColor = new float[4] { 0.15f, 0.15f, 0.15f, 1.0f };
		framebufferOptions.Width = window->GetWidth();
		framebufferOptions.Height = window->GetHeight();
		m_Framebuffer = Framebuffer::Create(framebufferOptions);

		m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);

		m_EditorScene = new Atom::Scene();
		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel = new SceneHierarchyPanel(m_ActiveScene);
		m_StatisticsPanel = new StatisticsPanel();
		m_ScriptEngineInspectorPanel = new ScriptEngineInspectorPanel();
		m_ProjectExplorer = new ProjectExplorer();
		m_ProjectSettingsPanel = new ProjectSettingsPanel();

		m_Viewport = new Viewport(m_Framebuffer, &m_EditorCamera, m_SceneHierarchyPanel);
		m_Viewport->SetSceneContext(m_ActiveScene);
		m_Viewport->SetDragDropCallback(AT_BIND_EVENT_FN(EditorLayer::OnViewportDragDropTarget));

#if 1
		OpenProject(m_ProjectPath);
#else
		auto commandLineArgs = Application::Get().GetOptions().CommandLineArgs;
		if(commandLineArgs.Count > 1)
		{
			auto projectFilepath = commandLineArgs[1];
			OpenProject(projectFilepath);
		}
		else
		{
			// TODO: prompt the user to select a directory
			// NewProject();

			// If no project is opened, close Atomic
			// NOTE: this is while we don't have a new project path
			Application::Get().Close();
		}
#endif
	}

	void EditorLayer::OnDetach()
	{
		EditorResources::Shutdown();
		delete m_ActiveScene;
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
		const glm::vec2& viewportSize = m_Viewport->GetViewportSize();
		m_ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);

		FramebufferOptions framebufferOptions = m_Framebuffer->GetOptions();
		if(m_Viewport->GetViewportSize().x > 0.0f && m_Viewport->GetViewportSize().y > 0.0f && (framebufferOptions.Width != m_Viewport->GetViewportSize().x || framebufferOptions.Height != m_Viewport->GetViewportSize().y))
		{
			m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_EditorCamera.SetViewportSize(viewportSize.x, viewportSize.y);
		}

		m_EditorCamera.OnUpdate(deltaTime);

		m_Framebuffer->Bind();
		m_Framebuffer->Clear();

		switch(m_SceneState)
		{
			case Atom::EditorLayer::SceneState::Edit:		m_ActiveScene->OnEditorUpdate(deltaTime, m_EditorCamera); break;
			case Atom::EditorLayer::SceneState::Simulate:	m_ActiveScene->OnSimulationUpdate(deltaTime, m_EditorCamera); break;
			case Atom::EditorLayer::SceneState::Play:		m_ActiveScene->OnRuntimeUpdate(deltaTime); break;
			default: break;
		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnGUI()
	{
		Atom::UI::Dockspace();
		DrawTopMenuBar();

		static bool isOpen = true;
		m_Viewport->OnImGuiRender(isOpen);
		m_SceneHierarchyPanel->OnImGuiRender(isOpen);
		m_StatisticsPanel->OnImGuiRender(isOpen);
		m_ScriptEngineInspectorPanel->OnImGuiRender(isOpen);
		m_ProjectExplorer->OnImGuiRender(isOpen);
		m_ProjectSettingsPanel->OnImGuiRender(isOpen);

		UI_Toolbar();

		ImGui::Begin("Settings");

		ImGui::Image(s_Font->GetAtlasTexture()->GetTexture(), { 512, 512 }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

		ImGui::End();

#if 1
		static bool opened = true;
		ImGui::ShowDemoWindow(&opened);
#endif

		if(m_ShowNewProjectDialog)
		{
			UI_ShowNewProjectDialog();
		}

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(AT_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::DrawTopMenuBar()
	{
		if(ImGui::BeginMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("New Project", "Ctrl+N"))
				{
					NewProject();
				}
				if(ImGui::MenuItem("Open Project...", "Ctrl+O"))
				{
					OpenProject();
				}
				if(ImGui::MenuItem("Save Project", "Ctrl+S"))
				{
					SaveProject();
				}
				if(ImGui::MenuItem("Save Project As...", "Ctrl+Shift+S"))
				{
					SaveProjectAs();
				}

				ImGui::Separator();

				if(ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}
				if(ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}
				if(ImGui::MenuItem("Save", "Ctrl+S"))
				{
					SaveScene();
				}
				if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				ImGui::Separator();

				if(ImGui::MenuItem("Restart (W.I.P.)"))
				{
					// TODO: Fix Swap Chain!

					AT_CORE_ASSERT("SwapChain failing! - TODO: FIX!!");
					Application::Get().Restart();
				}
				if(ImGui::MenuItem("Close"))
				{
					Application::Get().Close();
				}
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Script"))
			{
				if(ImGui::MenuItem("Reload Assembly"))
				{
					ScriptEngine::ReloadAssembly();
				}

				if(ImGui::MenuItem("Open C# Solution"))
				{
					static std::filesystem::path s_ProjectSolutionPath = Project::GetProjectDirectory() / std::filesystem::path(Project::GetProjectName() + ".sln");

					auto absolutePath = std::filesystem::canonical(s_ProjectSolutionPath);
					AT_CORE_ASSERT(std::filesystem::exists(absolutePath));

					ShellExecute(NULL, L"open", absolutePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
				}

				if(ImGui::MenuItem("Regenerate C# Solution"))
				{
					std::string s_BatchFilePath = Project::GetProjectDirectory().string();

					std::replace(s_BatchFilePath.begin(), s_BatchFilePath.end(), '/', '\\');

					s_BatchFilePath += "\\Generate.bat";

					uint32_t result = system(s_BatchFilePath.c_str());
					AT_CORE_INFO("Regenerated C# Solution! Result: {}", result);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 2.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0.0f, 0.0f));

		auto& colors = ImGui::GetStyle().Colors;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGuiWindowFlags toolbarFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("##toolbar", nullptr, toolbarFlags);

		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		bool hasPlayButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
		bool hasSimulateButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate;
		bool hasPauseButton = m_SceneState != SceneState::Edit;

		if(hasPlayButton)
		{
			Texture2D* icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? EditorResources::PlayIcon : EditorResources::StopIcon;
			if(ImGui::ImageButton(icon->GetTexture(), { size, size }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 0))
			{
				if(m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
				{
					OnScenePlay();
				}
				else if(m_SceneState == SceneState::Play)
				{
					OnSceneStop();
				}
			}
		}
		if(hasSimulateButton)
		{
			if(hasPlayButton)
			{
				ImGui::SameLine();
			}

			Texture2D* icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? EditorResources::SimulateIcon : EditorResources::StopIcon;
			if(ImGui::ImageButton(icon->GetTexture(), { size, size }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 0))
			{
				if(m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
				{
					OnSceneSimulate();
				}
				else if(m_SceneState == SceneState::Simulate)
				{
					OnSceneStop();
				}
			}
		}
		if(hasPauseButton)
		{
			bool isPaused = m_ActiveScene->IsPaused();
			ImGui::SameLine();
			{
				Texture2D* icon = EditorResources::PauseIcon;
				if(ImGui::ImageButton(icon->GetTexture(), { size, size }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 0))
				{
					m_ActiveScene->SetPaused(!isPaused);
				}
			}

			if(isPaused)
			{
				ImGui::SameLine();
				{
					Texture2D* icon = EditorResources::StepIcon;
					if(ImGui::ImageButton(icon->GetTexture(), { size, size }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 0))
					{
						m_ActiveScene->Step(1);
					}
				}
			}
		}

		ImGui::End();

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(2);
	}

	void EditorLayer::UI_ShowNewProjectDialog()
	{
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2{ 600.0f, 0.0f });

		ImGui::OpenPopup("New Project");

		if(ImGui::BeginPopupModal("New Project", &m_ShowNewProjectDialog, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static float frameRounding = 5.5f;

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));

			// Project Name
			{
				UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
				UI::ScopedStyle frameRounding(ImGuiStyleVar_FrameRounding, frameRounding);

				UI::ScopedFont projectNameFont(s_TestFont);

				ImGui::SetNextItemWidth(-1);
				ImGui::InputTextWithHint("##new_project_name", "Project Name", s_ProjectNameBuffer, MAX_PROJECT_NAME_LENGTH);
			}

			// Project Location 
			{
				UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
				UI::ScopedStyle frameRounding(ImGuiStyleVar_FrameRounding, frameRounding);

				UI::ScopedFont projectLocationFont(s_TestFont1);

				ImVec2 label_size = ImGui::CalcTextSize("...", NULL, true);
				auto& style = ImGui::GetStyle();
				ImVec2 button_size = ImGui::CalcItemSize(ImVec2(0, 0), label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

				ImGui::SetNextItemWidth(600.0f - button_size.x - style.FramePadding.x * 2.0f - style.ItemInnerSpacing.x - 1);
				ImGui::InputTextWithHint("##new_project_location", "Project Location", s_NewProjectFilePathBuffer, MAX_PROJECT_FILEPATH_LENGTH);

				ImGui::SameLine();

				if(ImGui::Button("..."))
				{
					std::filesystem::path result = FileDialogs::OpenFolderDialog();
					memcpy(s_NewProjectFilePathBuffer, result.string().data(), result.string().length());
				}
			}

			ImGui::PopStyleVar();

			std::string fullProjectPath = strlen(s_NewProjectFilePathBuffer) > 0 ? std::string(s_NewProjectFilePathBuffer) + "/" + std::string(s_ProjectNameBuffer) : "";
			ImGui::Text("Full Project Path: %s", fullProjectPath.c_str());

			ImGui::Separator();

			{
				UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
				UI::ScopedFont font(s_TestFont1);

				if(ImGui::Button("Create"))
				{
					CreateProject(fullProjectPath);
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if(ImGui::Button("Cancel"))
				{
					memset(s_NewProjectFilePathBuffer, 0, MAX_PROJECT_FILEPATH_LENGTH);
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
	}

	void EditorLayer::OnViewportDragDropTarget(const ImGuiPayload* payload)
	{
		if(!payload)
		{
			return;
		}

		const char* path = (const char*)payload->Data;
		std::filesystem::path filepath = Project::GetAssetDirectory() / path;
		OpenScene(filepath);
	}

	void EditorLayer::NewProject()
	{
		m_ShowNewProjectDialog = true;
		ImGui::OpenPopup("New Project");
	}

	static void ReplaceToken(std::string& str, const char* token, const std::string& value)
	{
		size_t pos = 0;
		while((pos = str.find(token, pos)) != std::string::npos)
		{
			str.replace(pos, strlen(token), value);
			pos += strlen(token);
		}
	}

	void EditorLayer::CreateProject(const std::filesystem::path& projectPath)
	{
		if(!std::filesystem::exists(projectPath))
		{
			std::filesystem::create_directory(projectPath);
		}

		std::filesystem::copy("Resources/NewProjectTemplate", projectPath, std::filesystem::copy_options::recursive);
		std::filesystem::path atomRootDirectory = std::filesystem::absolute("./Resources").parent_path().string();
		std::string atomRootDirectoryString = atomRootDirectory.string();

		if(atomRootDirectory.stem().string() == "Atomic")
		{
			atomRootDirectoryString = atomRootDirectory.parent_path().string();
		}

		std::replace(atomRootDirectoryString.begin(), atomRootDirectoryString.end(), '\\', '/');

		// premake5.lua
		{
			std::ifstream stream(projectPath / "premake5.lua");
			AT_CORE_ASSERT(stream.is_open());
			std::stringstream ss;
			ss << stream.rdbuf();
			stream.close();

			std::string str = ss.str();
			ReplaceToken(str, "$$PROJECT_NAME$$", s_ProjectNameBuffer);

			std::ofstream ostream(projectPath / "premake5.lua");
			ostream << str;
			ostream.close();
		}

		std::string newProjectFileName = std::string(fmt::format("{}.{}", s_ProjectNameBuffer, ATOM_PROJECT_FILE_EXTENSIONS));
		// Project file
		{
			std::ifstream stream(projectPath / fmt::format("{}.{}", "Project", ATOM_PROJECT_FILE_EXTENSIONS));
			AT_CORE_ASSERT(stream.is_open());
			std::stringstream ss;
			ss << stream.rdbuf();
			stream.close();

			std::string str = ss.str();
			ReplaceToken(str, "$$PROJECT_NAME$$", s_ProjectNameBuffer);

			std::ofstream ostream(projectPath / fmt::format("{}.{}", "Project", ATOM_PROJECT_FILE_EXTENSIONS));
			ostream << str;
			ostream.close();

			std::filesystem::rename(projectPath / fmt::format("{}.{}", "Project", ATOM_PROJECT_FILE_EXTENSIONS), projectPath / newProjectFileName);
		}

		std::filesystem::create_directory(projectPath / "Assets");

		{
			std::string s_BatchFilePath = projectPath.string();

			std::replace(s_BatchFilePath.begin(), s_BatchFilePath.end(), '/', '\\');

			s_BatchFilePath += "\\Generate.bat";

			uint32_t result = system(s_BatchFilePath.c_str());
			AT_CORE_INFO("Regenerated C# Solution! Result: {}", result);
		}

		OpenProject(projectPath / newProjectFileName);
	}

	void EditorLayer::OpenProject()
	{
		std::string filepath = FileDialogs::OpenFile(ATOM_PROJECT_FILE_DIALOG_FILTER);
		if(filepath.empty())
		{
			return;
		}

		OpenProject(filepath);
	}

	void EditorLayer::OpenProject(const std::filesystem::path& filepath)
	{
		if(Project::GetActiveProject())
		{
			CloseProject();
		}

		Project* newProject = new Project();

		ProjectSerializer serializer(newProject);
		if(!serializer.Deserialize(filepath))
		{
			AT_CORE_ERROR("Failed to load project at projectPath: {0}", filepath.string());
			return;
		}

		Project::SetActiveProject(newProject);

		ScriptEngine::LoadAppAssembly();

		m_ScriptEngineInspectorPanel->OnProjectChanged(Project::GetActiveProject());
		m_ProjectExplorer->OnProjectChanged(Project::GetActiveProject());

		if(!Project::GetActiveProject()->GetConfig().StartScene.empty())
		{
			auto startScenePath = Project::GetAssetFileSystemPath(Project::GetActiveProject()->GetConfig().StartScene);
			OpenScene(startScenePath);
			return;
		}
		NewScene();
		AT_CORE_WARN("No start scene has been set - new scene");
	}

	void EditorLayer::SaveProject()
	{
		const std::filesystem::path& projectFilepath = Project::GetProjectDirectory() / Project::GetProjectFileName();
		SaveProject(projectFilepath);

		//Project::SaveActiveProject(Project::GetProjectDirectory() / fmt::format("{}.{}", Project::GetActiveProject()->GetConfig().Name, ATOM_PROJECT_FILE_EXTENSIONS));
	}

	void EditorLayer::SaveProjectAs()
	{
		std::string filepath = FileDialogs::SaveFile(ATOM_PROJECT_FILE_DIALOG_FILTER);
		if(filepath.empty())
		{
			return;
		}

		SaveProject(filepath);

		//Project::SaveActiveProject(filepath);
	}

	void EditorLayer::SaveProject(const std::filesystem::path& filepath)
	{
		if(!Project::GetActiveProject())
		{
			AT_CORE_WARN("Cannot save project - No project loaded");
			return;
		}

		ProjectSerializer serializer(Project::GetActiveProject());
		const std::filesystem::path& projectFilepath = Project::GetProjectDirectory() / Project::GetProjectFileName();
		if(!serializer.Serialize(projectFilepath))
		{
			AT_CORE_ERROR("Failed to save project at projectPath: {0}", projectFilepath.string());
			return;
		}
	}

	void EditorLayer::CloseProject(bool unloadProject)
	{
		SaveProject();

		//ScriptEngine::UnloadAppAssembly();
		ScriptEngine::SetSceneContext(nullptr);

		if(unloadProject)
		{
			Project::SetActiveProject(nullptr);
		}
	}

	void EditorLayer::NewScene()
	{
		delete m_EditorScene;
		m_EditorScene = nullptr;

		//delete m_ActiveScene;
		//m_ActiveScene = nullptr;

		m_ActiveScene = new Scene();
		m_EditorScene = m_ActiveScene;

		//m_ActiveScene->OnViewportResize(m_Viewport->m_ViewportSize.x, m_Viewport->m_ViewportSize.y);
		m_SceneHierarchyPanel->SetSceneContext(m_ActiveScene);
		m_Viewport->SetSceneContext(m_ActiveScene);

	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile(ATOM_SCENE_FILE_DIALOG_FILTER);
		if(filepath.empty())
		{
			AT_CORE_WARN("Cancelled open file dialog!");
			return;
		}

		OpenScene(filepath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if(m_SceneState != SceneState::Edit)
		{
			OnSceneStop();
		}

		if(path.extension().string() != fmt::format(".{}", ATOM_SCENE_FILE_EXTENSIONS))
		{
			AT_CORE_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		Scene* newScene = new Scene();
		SceneSerializer serializer(newScene);
		bool success = serializer.Deserialize(path.string());
		AT_CORE_ASSERT(success, "Failed to deserialize scene");

		m_EditorScene = newScene;
		//m_EditorScene->OnViewportResize(m_Viewport->m_ViewportSize.x, m_Viewport->m_ViewportSize.y);
		m_SceneHierarchyPanel->SetSceneContext(m_EditorScene);

		delete m_ActiveScene;
		m_ActiveScene = nullptr;

		m_ActiveScene = m_EditorScene;
		m_EditorScenePath = path;
	}

	void EditorLayer::SaveScene()
	{
		SceneSerializer serializer(m_ActiveScene);
		serializer.Serialize(m_EditorScenePath.string());
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile(ATOM_SCENE_FILE_DIALOG_FILTER);
		if(filepath.empty())
		{
			AT_CORE_WARN("Cancelled save file dialog!");
			return;
		}

		SceneSerializer serializer(m_ActiveScene);
		serializer.Serialize(filepath);
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel->SetSceneContext(m_ActiveScene);
		m_Viewport->SetGizmoType(-1);
	}

	void EditorLayer::OnSceneStop()
	{
		AT_CORE_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate);

		if(m_SceneState == SceneState::Play)
		{
			m_ActiveScene->OnRuntimeStop();
		}
		else if(m_SceneState == SceneState::Simulate)
		{
			m_ActiveScene->OnSimulationStop();
		}

		delete m_ActiveScene;
		m_ActiveScene = nullptr;

		m_SceneState = SceneState::Edit;

		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel->SetSceneContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneSimulate()
	{
		if(m_SceneState == SceneState::Play)
		{
			OnSceneStop();
		}

		m_SceneState = SceneState::Simulate;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();

		m_SceneHierarchyPanel->SetSceneContext(m_ActiveScene);
	}

	void EditorLayer::OnScenePause()
	{
		if(m_SceneState == SceneState::Edit)
		{
			return;
		}

		m_ActiveScene->SetPaused(true);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if(m_SceneState != SceneState::Edit)
		{
			return;
		}

		Entity selectedEntity = m_SceneHierarchyPanel->GetSelectedEntity();
		if(selectedEntity)
		{
			m_EditorScene->DuplicateEntity(selectedEntity);
		}
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if(e.GetRepeatCount() > 0)
		{
			return false;
		}

		bool control = Input::IsKeyDown(Key::LeftControl) || Atom::Input::IsKeyDown(Key::RightControl);
		bool shift = Input::IsKeyDown(Key::LeftShift) || Input::IsKeyDown(Key::RightShift);

		switch(e.GetKeyCode())
		{
			// Gizmo shortcuts
			case Key::Q:
				m_Viewport->SetGizmoType(-1);
				break;
			case Key::W:
				m_Viewport->SetGizmoType(ImGuizmo::OPERATION::TRANSLATE);
				break;
			case Key::E:
				m_Viewport->SetGizmoType(ImGuizmo::OPERATION::ROTATE);
				break;
			case Key::R:
				m_Viewport->SetGizmoType(ImGuizmo::OPERATION::SCALE);
				break;

			// Entity shortcuts
			case Key::D:
			{
				if(control)
				{
					OnDuplicateEntity();
				}
			} break;

			// Scene shortcuts
			case Key::N:
			{
				if(control)
				{
					NewScene();
				}
			} break;
			case Key::O:
			{
				if(control)
				{
					OpenScene();
				}
			} break;
			case Key::S:
			{
				if(control)
				{
					SaveScene();
					break;

				}

				if(control && shift)
				{
					SaveSceneAs();
				}
			} break;
		}

		if(m_SceneState == SceneState::Play)
		{
			m_Viewport->SetGizmoType(-1);
		}

		return true;
	}

}