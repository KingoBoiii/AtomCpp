#include "EditorLayer.h"
#include <Atom/Scene/SceneSerializer.h>
#include <Atom/Utils/PlatformUtils.h>
#include <Atom/ImGui/ImGuiUtillities.h>
#include <Atom/Editor/EditorResources.h>
#include <Atom/Scripting/ScriptEngine.h>
#include <Atom/Renderer/Font.h>

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include "ImGuizmo.h"

namespace Atom
{

#define ATOM_SCENE_FILE_EXTENSIONS "atsc"
#define ATOM_SCENE_FILE_DIALOG_FILTER "Atom Scene (*.atsc)\0*.atsc\0"

#define ATOM_PROJECT_FILE_EXTENSIONS "atpr"
#define ATOM_PROJECT_FILE_DIALOG_FILTER "Atom Project (*.atpr)\0*.atpr\0"

	static Font* s_Font;

	void EditorLayer::OnAttach()
	{
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

		m_Viewport = new Viewport(m_Framebuffer, &m_EditorCamera, m_SceneHierarchyPanel);
		m_Viewport->SetSceneContext(m_ActiveScene);

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

		UI_Toolbar();

		ImGui::Begin("Settings");

		ImGui::Image(s_Font->GetAtlasTexture()->GetTexture(), { 512, 512 }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

		ImGui::End();

#if 1
		static bool opened = true;
		ImGui::ShowDemoWindow(&opened);
#endif

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
				if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveAs();
				}

				ImGui::Separator();

				std::string filepath = fmt::format("Assets/Scenes/Example.{}", ATOM_SCENE_FILE_EXTENSIONS);

				if(ImGui::MenuItem("Save Example Scene"))
				{
					SceneSerializer serializer(m_ActiveScene);
					serializer.Serialize(filepath);
				}
				if(ImGui::MenuItem("Load Example Scene"))
				{
					SceneSerializer serializer(m_ActiveScene);
					serializer.Deserialize(filepath);

					m_ActiveScene->OnViewportResize(m_Viewport->m_ViewportSize.x, m_Viewport->m_ViewportSize.y);
				}

				ImGui::Separator();

				if(ImGui::MenuItem("Restart (W.I.P.)"))
				{
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

	void EditorLayer::NewProject()
	{
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
		bool projectLoaded = Project::Load(filepath);

		if(projectLoaded)
		{
			ScriptEngine::Initialize(Application::Get().GetOptions().ScriptConfig);

			m_ScriptEngineInspectorPanel->OnProjectChanged(Project::GetActiveProject());
			m_ProjectExplorer->OnProjectChanged(Project::GetActiveProject());

			auto startScenePath = Project::GetAssetFileSystemPath(Project::GetActiveProject()->GetConfig().StartScene);
			OpenScene(startScenePath);
		}
	}

	void EditorLayer::SaveProject()
	{
	}

	void EditorLayer::SaveProjectAs()
	{
		std::string filepath = FileDialogs::SaveFile(ATOM_PROJECT_FILE_DIALOG_FILTER);
		if(filepath.empty())
		{
			return;
		}

		Project::SaveActiveProject(filepath);
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = new Scene();
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

		m_ActiveScene = m_EditorScene;
		m_EditorScenePath = path;
	}

	void EditorLayer::SaveAs()
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
				if(control && shift)
				{
					SaveAs();
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