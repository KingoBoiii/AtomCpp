#include "ATPCH.h"
#include "SceneSerializer.h"
#include "Entity.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace YAML
{

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if(!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if(!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if(!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Atom::UUID>
	{
		static Node encode(const Atom::UUID& uuid)
		{
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, Atom::UUID& uuid)
		{
			uuid = node.as<uint64_t>();
			return true;
		}
	};

}

namespace Atom
{

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	namespace Utils
	{

		static void SerializeEntity(YAML::Emitter& out, Entity entity)
		{
			out << YAML::BeginMap;		// Entity

			out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

			if(entity.HasComponent<Component::Identifier>())
			{
				auto& name = entity.GetComponent<Component::Identifier>().Name;

				out << YAML::Key << "Identifier";
				out << YAML::BeginMap;		// Identifier

				out << YAML::Key << "Name" << YAML::Value << name;

				out << YAML::EndMap;		// Identifier
			}

			if(entity.HasComponent<Component::Transform>())
			{
				auto& transform = entity.GetComponent<Component::Transform>();

				out << YAML::Key << "Transform";
				out << YAML::BeginMap;		// Transform

				out << YAML::Key << "Position" << YAML::Value << transform.Position;
				out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
				out << YAML::Key << "Scale" << YAML::Value << transform.Scale;

				out << YAML::EndMap;		// Transform
			}

			if(entity.HasComponent<Component::Camera>())
			{
				out << YAML::Key << "Camera";
				out << YAML::BeginMap; // Camera

				auto& cameraComponent = entity.GetComponent<Component::Camera>();
				auto& camera = cameraComponent.SceneCamera;

				out << YAML::Key << "SceneCamera" << YAML::Value;
				out << YAML::BeginMap; // SceneCamera
				out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();

				out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
				out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
				out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();

				out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
				out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
				out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
				out << YAML::EndMap; // SceneCamera

				out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
				out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

				out << YAML::EndMap; // Camera
			}

			if(entity.HasComponent<Component::BasicRenderer>())
			{
				auto& basicRenderer = entity.GetComponent<Component::BasicRenderer>();

				out << YAML::Key << "BasicRenderer";
				out << YAML::BeginMap;		// BasicRenderer

				out << YAML::Key << "Color" << YAML::Value << basicRenderer.Color;

				out << YAML::EndMap;		// BasicRenderer
			}

			out << YAML::EndMap;		// Entity
		}

	}

	SceneSerializer::SceneSerializer(Scene* scene)
		: m_Scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;		// Scene

		out << YAML::Key << "Scene" << YAML::Value << "Untitled"; // TODO: Add actual scene name!

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene };
		if(!entity)
			return;

		Utils::SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;

		out << YAML::EndMap;		// Scene

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if(!data["Scene"])
		{
			AT_CORE_ERROR("Scene file '{0}' is not valid!", filepath);
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		AT_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if(!entities)
		{
			AT_CORE_WARN("No entities to serialize in scene '{}'", sceneName);
			return false;
		}

		for(auto entity : entities)
		{
			UUID uuid = entity["Entity"].as<UUID>();

			std::string name;
			auto identifier = entity["Identifier"];
			if(identifier)
			{
				name = identifier["Name"].as<std::string>();
			}

			AT_CORE_TRACE("Deserializing entity with ID = {0}, name = {1}", uuid, name);

			Entity deserializedEntity = m_Scene->CreateEntity(uuid, name);

			auto transformComponent = entity["Transform"];
			if(transformComponent)
			{
				// Entities always have transforms
				auto& transform = deserializedEntity.GetComponent<Component::Transform>();
				transform.Position = transformComponent["Position"].as<glm::vec3>();
				transform.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				transform.Scale = transformComponent["Scale"].as<glm::vec3>();
			}

			auto cameraComponent = entity["Camera"];
			if(cameraComponent)
			{
				auto& camera = deserializedEntity.AddComponent<Component::Camera>();
				auto& sceneCamera = camera.SceneCamera;

				auto sceneCameraProps = cameraComponent["SceneCamera"];
				sceneCamera.SetProjectionType((SceneCamera::ProjectionType)sceneCameraProps["ProjectionType"].as<int>());

				sceneCamera.SetPerspectiveVerticalFOV(sceneCameraProps["PerspectiveFOV"].as<float>());
				sceneCamera.SetPerspectiveNearClip(sceneCameraProps["PerspectiveNear"].as<float>());
				sceneCamera.SetPerspectiveFarClip(sceneCameraProps["PerspectiveFar"].as<float>());

				sceneCamera.SetOrthographicSize(sceneCameraProps["OrthographicSize"].as<float>());
				sceneCamera.SetOrthographicNearClip(sceneCameraProps["OrthographicNear"].as<float>());
				sceneCamera.SetOrthographicFarClip(sceneCameraProps["OrthographicFar"].as<float>());

				camera.Primary = cameraComponent["Primary"].as<bool>();
				camera.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
			}

			auto basicRendererComponent = entity["BasicRenderer"];
			if(basicRendererComponent)
			{
				auto& basicRenderer = deserializedEntity.AddComponent<Component::BasicRenderer>();
				basicRenderer.Color = basicRendererComponent["Color"].as<glm::vec4>();
			}
		}

		return true;
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		AT_CORE_ASSERT(false);
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		AT_CORE_ASSERT(false);
		return false;
	}

}