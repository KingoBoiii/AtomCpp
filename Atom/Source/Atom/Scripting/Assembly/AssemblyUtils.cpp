#include "ATPCH.h"
#include "AssemblyUtils.h"

#include "Atom/Core/FileSystem.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/mono-debug.h>

namespace Atom
{

	namespace Utils
	{

		static void InitializeMonoDebug(MonoImage* monoImage, const std::filesystem::path& pdbFilepath)
		{
			if(!std::filesystem::exists(pdbFilepath))
			{
				AT_CORE_ERROR("Failed to initialize Mono debugging. PDB file does not exist: {0}", pdbFilepath.string());
				return;
			}

			ScopedBuffer pdbFileBuffer = FileSystem::ReadFileBinary(pdbFilepath);

			mono_debug_open_image_from_memory(monoImage, pdbFileBuffer.As<const mono_byte>(), pdbFileBuffer.Size());

			AT_CORE_INFO("Successfully loaded PDB file: {}", pdbFilepath);
		}

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& filepath, bool enableDebugging)
		{
			if(!std::filesystem::exists(filepath))
			{
				return nullptr;
			}

			ScopedBuffer buffer = FileSystem::ReadFileBinary(filepath.string());

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(buffer.As<char>(), buffer.Size(), 1, &status, 0);

			if(status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				AT_CORE_ERROR("[ScriptEngine] Failed to open C# assembly '{0}'\n\t\tMessage: {1}", filepath, errorMessage);
				return nullptr;
			}

			if(enableDebugging)
			{
				std::filesystem::path pdbPath = filepath;
				pdbPath.replace_extension(".pdb");

				InitializeMonoDebug(image, pdbPath);
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, filepath.string().c_str(), &status, 0);
			mono_image_close(image);

			return assembly;
		}

	}

	AssemblyInfo* AssemblyUtils::LoadAssembly(const std::filesystem::path& filepath, bool enableDebugging)
	{
		MonoAssembly* monoAssembly = Utils::LoadMonoAssembly(filepath, enableDebugging);
		if(monoAssembly == nullptr)
		{
			return nullptr;
		}

		AssemblyInfo* assemblyInfo = new AssemblyInfo();
		assemblyInfo->Filepath = filepath;
		assemblyInfo->Assembly = monoAssembly;
		assemblyInfo->AssemblyImage = mono_assembly_get_image(monoAssembly);
		assemblyInfo->IsCoreAssembly = true;
		assemblyInfo->Metadata = AssemblyUtils::LoadAssemblyMetadata(assemblyInfo);
		assemblyInfo->ReferencedAssemblies = AssemblyUtils::GetReferencedAssembliesMetadata(assemblyInfo);

		return assemblyInfo;
	}

	AssemblyMetadata AssemblyUtils::LoadAssemblyMetadata(AssemblyInfo* assemblyInfo)
	{
		AssemblyMetadata metadata;

		const MonoTableInfo* t = mono_image_get_table_info(assemblyInfo->AssemblyImage, MONO_TABLE_ASSEMBLY);
		uint32_t cols[MONO_ASSEMBLY_SIZE];
		mono_metadata_decode_row(t, 0, cols, MONO_ASSEMBLY_SIZE);

		metadata.Name = mono_metadata_string_heap(assemblyInfo->AssemblyImage, cols[MONO_ASSEMBLY_NAME]);
		metadata.MajorVersion = cols[MONO_ASSEMBLY_MAJOR_VERSION] > 0 ? cols[MONO_ASSEMBLY_MAJOR_VERSION] : 1;
		metadata.MinorVersion = cols[MONO_ASSEMBLY_MINOR_VERSION];
		metadata.BuildVersion = cols[MONO_ASSEMBLY_BUILD_NUMBER];
		metadata.RevisionVersion = cols[MONO_ASSEMBLY_REV_NUMBER];

		return metadata;
	}

	std::vector<AssemblyMetadata> AssemblyUtils::GetReferencedAssembliesMetadata(AssemblyInfo* assemblyInfo)
	{
		const MonoTableInfo* t = mono_image_get_table_info(assemblyInfo->AssemblyImage, MONO_TABLE_ASSEMBLYREF);
		int rows = mono_table_info_get_rows(t);

		std::vector<AssemblyMetadata> metadata;
		for(int i = 0; i < rows; i++)
		{
			uint32_t cols[MONO_ASSEMBLYREF_SIZE];
			mono_metadata_decode_row(t, i, cols, MONO_ASSEMBLYREF_SIZE);

			auto& assemblyMetadata = metadata.emplace_back();
			assemblyMetadata.Name = mono_metadata_string_heap(assemblyInfo->AssemblyImage, cols[MONO_ASSEMBLYREF_NAME]);
			assemblyMetadata.MajorVersion = cols[MONO_ASSEMBLYREF_MAJOR_VERSION];
			assemblyMetadata.MinorVersion = cols[MONO_ASSEMBLYREF_MINOR_VERSION];
			assemblyMetadata.BuildVersion = cols[MONO_ASSEMBLYREF_BUILD_NUMBER];
			assemblyMetadata.RevisionVersion = cols[MONO_ASSEMBLYREF_REV_NUMBER];
		}

		return metadata;
	}

}