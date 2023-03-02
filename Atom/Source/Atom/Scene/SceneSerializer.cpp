#include "ATPCH.h"
#include "SceneSerializer.h"
#include "Entity.h"

#include "Atom/Scripting/ScriptEngine.h"
#include "Atom/Scripting/ScriptCache.h"
#include "Atom/Scripting/Managed/ManagedClass.h"

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

		static std::string Rigidbody2DBodyTypeToString(Component::Rigidbody2D::BodyType bodyType)
		{
			switch(bodyType)
			{
				case Atom::Component::Rigidbody2D::BodyType::Static: return "Static";
				case Atom::Component::Rigidbody2D::BodyType::Dynamic: return "Dynamic";
				case Atom::Component::Rigidbody2D::BodyType::Kinematic: return "Kinematic";
			}

			AT_CORE_ASSERT(false, "Unknown Rigidbody2D BodyType!");
			return "Unknown";
		}

		static Component::Rigidbody2D::BodyType Rigidbody2DBodyTypeFromString(const std::string& bodyTypeString)
		{
			if(bodyTypeString == "Static") return Component::Rigidbody2D::BodyType::Static;
			if(bodyTypeString == "Dynamic") return Component::Rigidbody2D::BodyType::Dynamic;
			if(bodyTypeString == "Kinematic") return Component::Rigidbody2D::BodyType::Kinematic;

			AT_CORE_ASSERT(false, "Unknown Rigidbody2D BodyType!");
			return Component::Rigidbody2D::BodyType::Static;
		}

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

			if(entity.HasComponent<Component::CircleRenderer>())
			{
				auto& circleRenderer = entity.GetComponent<Component::CircleRenderer>();

				out << YAML::Key << "CircleRenderer";
				out << YAML::BeginMap;		// CircleRenderer

				out << YAML::Key << "Color" << YAML::Value << circleRenderer.Color;
				out << YAML::Key << "Thickness" << YAML::Value << circleRenderer.Thickness;
				out << YAML::Key << "Fade" << YAML::Value << circleRenderer.Fade;

				out << YAML::EndMap;		// CircleRenderer
			}

			if(entity.HasComponent<Component::Script>())
			{
				auto& script = entity.GetComponent<Component::Script>();

				out << YAML::Key << "Script";
				out << YAML::BeginMap;		// Script

				out << YAML::Key << "ClassName" << YAML::Value << script.ClassName;

				ManagedClass* managedClass = AT_CACHED_ENTITY_CLASS(script.ClassName);
				if(managedClass)
				{
					const auto& fields = managedClass->GetFields();
					if(fields.size() > 0)
					{
						out << YAML::Key << "ScriptFields" << YAML::Value;
						out << YAML::BeginSeq;		// ScriptFields

						for(const auto& [fieldName, field] : fields)
						{
							if(field.GetFieldAccessModifier() != FieldAccessModifierFlag::Public)
							{
								continue;
							}

							out << YAML::BeginMap;		// ScriptField

							out << YAML::Key << "Name" << YAML::Value << fieldName;
							out << YAML::Key << "Type" << YAML::Value << field.GetTypeString();
							
							out << YAML::Key << "Data" << YAML::Key;
							ManagedClassField& classField = managedClass->GetField(fieldName);

#define AT_CASE_SERIALIZE_FIELD(managedType, type) case managedType: out << classField.GetValue<type>(); break
							
							switch(field.GetType())
							{
								AT_CASE_SERIALIZE_FIELD(ManagedFieldType::Bool, bool);
								AT_CASE_SERIALIZE_FIELD(ManagedFieldType::Char, char);
								AT_CASE_SERIALIZE_FIELD(ManagedFieldType::Float, float);
								default: AT_CORE_ASSERT(false, "Unknown field type - TODO: Add the field type you want to serialize"); break;
							}
							
#undef AT_CASE_SERIALIZE_FIELD

							out << YAML::EndMap;		// ScriptField
						}

						out << YAML::EndSeq;		// ScriptFields
					}
				}

				out << YAML::EndMap;		// Script
			}

			if(entity.HasComponent<Component::Rigidbody2D>())
			{
				auto& rigidbody2D = entity.GetComponent<Component::Rigidbody2D>();

				out << YAML::Key << "Rigidbody2D";
				out << YAML::BeginMap;		// Rigidbody2D

				out << YAML::Key << "BodyType" << YAML::Value << Rigidbody2DBodyTypeToString(rigidbody2D.Type);
				out << YAML::Key << "FixedRotation" << YAML::Value << rigidbody2D.FixedRotation;

				out << YAML::EndMap;		// Rigidbody2D
			}

			if(entity.HasComponent<Component::BoxCollider2D>())
			{
				auto& boxCollider2D = entity.GetComponent<Component::BoxCollider2D>();

				out << YAML::Key << "BoxCollider2D";
				out << YAML::BeginMap;		// BoxCollider2D

				out << YAML::Key << "Offset" << YAML::Value << boxCollider2D.Offset;
				out << YAML::Key << "Size" << YAML::Value << boxCollider2D.Size;
				out << YAML::Key << "IsTrigger" << YAML::Value << boxCollider2D.IsTrigger;

				out << YAML::Key << "Density" << YAML::Value << boxCollider2D.Density;
				out << YAML::Key << "Friction" << YAML::Value << boxCollider2D.Friction;
				out << YAML::Key << "Restitution" << YAML::Value << boxCollider2D.Restitution;
				out << YAML::Key << "RestitutionThreshold" << YAML::Value << boxCollider2D.RestitutionThreshold;

				out << YAML::EndMap;		// BoxCollider2D
			}

			if(entity.HasComponent<Component::TextRenderer>())
			{
				auto& textRenderer = entity.GetComponent<Component::TextRenderer>();

				out << YAML::Key << "TextRenderer";
				out << YAML::BeginMap;		// TextRenderer

				out << YAML::Key << "Text" << YAML::Value << textRenderer.Text;
				out << YAML::Key << "Color" << YAML::Value << textRenderer.Color;
				out << YAML::Key << "Kerning" << YAML::Value << textRenderer.Kerning;
				out << YAML::Key << "LineSpacing" << YAML::Value << textRenderer.LineSpacing;

				out << YAML::EndMap;		// TextRenderer
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

			auto circleRendererComponent = entity["CircleRenderer"];
			if(circleRendererComponent)
			{
				auto& circleRenderer = deserializedEntity.AddComponent<Component::CircleRenderer>();
				circleRenderer.Color = circleRendererComponent["Color"].as<glm::vec4>();
				circleRenderer.Thickness = circleRendererComponent["Thickness"].as<float>();
				circleRenderer.Fade = circleRendererComponent["Fade"].as<float>();
			}

			auto scriptComponent = entity["Script"];
			if(scriptComponent)
			{

				auto& script = deserializedEntity.AddComponent<Component::Script>();
				script.ClassName = scriptComponent["ClassName"].as<std::string>();

				auto& scriptFields = scriptComponent["ScriptFields"];
				if(scriptFields)
				{
					ManagedClass* managedClass = AT_CACHED_ENTITY_CLASS(script.ClassName);
					if(managedClass)
					{
						const auto& fields = managedClass->GetFields();

						for(auto scriptField : scriptFields)
						{
							std::string fieldName = scriptField["Name"].as<std::string>();
							std::string fieldTypeString = scriptField["Type"].as<std::string>();
							ManagedFieldType fieldType = Utils::ManagedFieldTypeFromString(fieldTypeString);

							ManagedClassField& classField = managedClass->GetField(fieldName);
#define AT_CASE_DESERIALIZE_FIELD(managedType, type) case managedType: classField.SetValue<type>(scriptField["Data"].as<type>()); break

							switch(fieldType)
							{
								AT_CASE_DESERIALIZE_FIELD(ManagedFieldType::Bool, bool);
								AT_CASE_DESERIALIZE_FIELD(ManagedFieldType::Char, char);
								AT_CASE_DESERIALIZE_FIELD(ManagedFieldType::Float, float);
								default: AT_CORE_ASSERT(false, "Unknown field type - TODO: Add the field type you want to deserialize"); break;
							}

#undef AT_CASE_DESERIALIZE_FIELD
						}
					}
				}
#if 0
					ScriptClass* entityClass = ScriptEngine::GetEntityClass(script.ClassName);
					AT_CORE_ASSERT(entityClass);
					const auto& fields = entityClass->GetFields();

					auto& entityFields = ScriptEngine::GetScriptFieldMap(deserializedEntity);

					for(auto scriptField : scriptFields)
					{
						std::string name = scriptField["Name"].as<std::string>();
						std::string typeString = scriptField["Type"].as<std::string>();
						ScriptFieldType type = Utils::ScriptFieldTypeFromString(typeString);

						ScriptFieldInstance& fieldInstance = entityFields[name];

						AT_CORE_ASSERT(fields.find(name) != fields.end());

						if(fields.find(name) == fields.end())
						{
							continue;
						}
						
						fieldInstance.Field = fields.at(name);

						switch(type)
						{
							READ_SCRIPT_FIELD(Bool, bool);
							READ_SCRIPT_FIELD(Char, char);
							READ_SCRIPT_FIELD(Float, float);
							READ_SCRIPT_FIELD(Double, double);
							READ_SCRIPT_FIELD(Byte, int8_t);
							READ_SCRIPT_FIELD(Short, int16_t);
							READ_SCRIPT_FIELD(Int, int32_t);
							READ_SCRIPT_FIELD(Long, int64_t);
							READ_SCRIPT_FIELD(Vector2, glm::vec2);
							READ_SCRIPT_FIELD(Vector3, glm::vec3);
							READ_SCRIPT_FIELD(Vector4, glm::vec4);
							READ_SCRIPT_FIELD(Entity, UUID);
						}
					}
				}
#endif
			}

			auto rigidbody2DComponent = entity["Rigidbody2D"];
			if(rigidbody2DComponent)
			{
				auto& rigidbody2D = deserializedEntity.AddComponent<Component::Rigidbody2D>();

				rigidbody2D.Type = Utils::Rigidbody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
				rigidbody2D.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
			}

			auto boxCollider2DComponent = entity["BoxCollider2D"];
			if(boxCollider2DComponent)
			{
				auto& boxCollider2D = deserializedEntity.AddComponent<Component::BoxCollider2D>();

				boxCollider2D.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
				boxCollider2D.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
				boxCollider2D.IsTrigger = boxCollider2DComponent["IsTrigger"].as<bool>();

				boxCollider2D.Density = boxCollider2DComponent["Density"].as<float>();
				boxCollider2D.Friction = boxCollider2DComponent["Friction"].as<float>();
				boxCollider2D.Restitution = boxCollider2DComponent["Restitution"].as<float>();
				boxCollider2D.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();

			}

			auto textRendererComponent = entity["TextRenderer"];
			if(textRendererComponent)
			{
				auto& textRenderer = deserializedEntity.AddComponent<Component::TextRenderer>();

				textRenderer.Text = textRendererComponent["Text"].as<std::string>();
				textRenderer.Color = textRendererComponent["Color"].as<glm::vec4>();
				textRenderer.Kerning = textRendererComponent["Kerning"].as<float>();
				textRenderer.LineSpacing = textRendererComponent["LineSpacing"].as<float>();
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