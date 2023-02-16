#pragma once

namespace Atom
{
	
	class ATOM_API Layer
	{
	public:
		Layer(const std::string& debugName);
		virtual	~Layer() = default;

		virtual void OnAttach() { }
		virtual void OnDetach() { }
		virtual void OnUpdate(float deltaTime) { }
	private:
		std::string m_DebugName;
	};

}
