#include "ATPCH.h"
#include "AssetRegistrySerializer.h"
#include "Atom/Asset/AssetType.h"

#include <fstream>
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace YAML
{

	template<>
	struct convert<Atom::AssetHandle>
	{
		static Node encode(const Atom::AssetHandle& assetHandle)
		{
			Node node;
			node.push_back((uint64_t)assetHandle);
			return node;
		}

		static bool decode(const Node& node, Atom::AssetHandle& assetHandle)
		{
			assetHandle = node.as<uint64_t>();
			return true;
		}
	};

}

namespace Atom
{

	AssetRegistrySerializer::AssetRegistrySerializer(AssetRegistry* assetRegistry)
		: m_AssetRegistry(assetRegistry)
	{
	}

	bool AssetRegistrySerializer::Serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap; // Root

		out << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq; // Assets
		for(const auto& [handle, metadata] : *m_AssetRegistry)
		{
			out << YAML::BeginMap; // Asset
			out << YAML::Key << "Handle" << YAML::Value << metadata.Handle;
			out << YAML::Key << "Type" << YAML::Value << Utils::AssetTypeToString(metadata.Type);
			out << YAML::Key << "Filepath" << YAML::Value << metadata.Filepath.string();
			out << YAML::EndMap; // Asset
		}
		out << YAML::EndSeq; // Assets

		out << YAML::EndMap; // Root

		std::ofstream fout(filepath);
		fout << out.c_str();

		return true;
	}

	bool AssetRegistrySerializer::Deserialize(const std::filesystem::path& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if(!data["AssetRegistry"])
		{
			AT_CORE_ERROR("AssetRegistry file '{0}' is not valid!", filepath);
			return false;
		}

		auto assets = data["Assets"];
		if(!assets)
		{
			AT_CORE_WARN("No assets to serialize in Asset Registry '{}'", filepath);
			return false;
		}

		for(auto asset : assets)
		{
			AssetMetadata metadata;
			metadata.Handle = asset["Handle"].as<AssetHandle>();
			metadata.Type = Utils::AssetTypeFromString(asset["Type"].as<std::string>());
			metadata.Filepath = asset["Filepath"].as<std::string>();

			(*m_AssetRegistry)[metadata.Handle] = metadata;
		}


		return true;
	}

}