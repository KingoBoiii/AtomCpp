#pragma once
#include <string>

extern "C" {
	typedef struct _MonoMethod MonoMethod;
}

namespace Atom
{

	class ManagedMethod
	{
	public:
		ManagedMethod() = default;
		ManagedMethod(const ManagedMethod&) = default;
		ManagedMethod(MonoMethod* rawMethod);
		~ManagedMethod();

		MonoMethod* GetRawMethod() const { return m_RawMethod; }
		const std::string& GetFullName() const { return m_FullName; }
	private:
		void Invalidate();
	private:
		std::string m_MethodName;
		std::string m_FullName;

		uint32_t m_ParameterCount = 0;

		MonoMethod* m_RawMethod = nullptr;
	};

#if 0
	struct ManagedMethod
	{
		uint32_t Id;
		std::string FullName;
		int32_t ParameterCount;

		MonoMethod* Method = nullptr;
		
		~ManagedMethod()
		{
			Method = nullptr;
		}
	};
#endif

}
