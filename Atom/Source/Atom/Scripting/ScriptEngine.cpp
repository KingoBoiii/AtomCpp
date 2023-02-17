#include "ATPCH.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"

#include <glm/glm.hpp>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

namespace Atom
{

	namespace Utils
	{

		char* ReadBytes(const std::string& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if(!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if(size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if(status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for(int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				AT_CORE_TRACE("{}.{}", nameSpace, name);
			}
		}

	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;
	};

	static ScriptEngineData* s_ScriptEngineData = nullptr;

	ScriptEngine* ScriptEngine::s_Instance = nullptr;

	ScriptEngine::ScriptEngine()
	{
		AT_CORE_ASSERT(!s_Instance, "ScriptEngine already exists!");
		s_Instance = this;

		s_ScriptEngineData = new ScriptEngineData();
	}

	ScriptEngine::~ScriptEngine()
	{

		delete s_ScriptEngineData;
	}

	void ScriptEngine::Initialize()
	{
		InitializeMono();
		LoadAssembly("Resources/Scripts/Atom.Core.dll");

		ScriptGlue::RegisterInternalCalls();
		
#if 0
		MonoClass* monoClass = mono_class_from_name(s_ScriptEngineData->CoreAssemblyImage, "Atom", "Main");
		MonoObject* instance = mono_object_new(s_ScriptEngineData->AppDomain, monoClass);
		mono_runtime_object_init(instance);

		MonoMethod* printMessageMethod = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		mono_runtime_invoke(printMessageMethod, instance, nullptr, nullptr);

		MonoString* str = mono_string_new(s_ScriptEngineData->AppDomain, "Hello world from C++");
		MonoMethod* printCustomMessageMethod = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
		mono_runtime_invoke(printCustomMessageMethod, instance, (void**)&str, nullptr);
#endif
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
	}

	void ScriptEngine::InitializeMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("AtomJITRuntime");
		AT_CORE_ASSERT(rootDomain);

		s_ScriptEngineData->RootDomain = rootDomain;
	}

	void ScriptEngine::ShutdownMono()
	{
		s_ScriptEngineData->AppDomain = nullptr;

		mono_jit_cleanup(s_ScriptEngineData->RootDomain);
		s_ScriptEngineData->RootDomain = nullptr;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_ScriptEngineData->AppDomain = mono_domain_create_appdomain("AtomScriptRuntime", nullptr);
		mono_domain_set(s_ScriptEngineData->AppDomain, true);

		s_ScriptEngineData->CoreAssembly = Utils::LoadCSharpAssembly("Resources/Scripts/Atom.Core.dll");
		s_ScriptEngineData->CoreAssemblyImage = mono_assembly_get_image(s_ScriptEngineData->CoreAssembly);
		//Utils::PrintAssemblyTypes(s_ScriptEngineData->CoreAssembly);
	}

}