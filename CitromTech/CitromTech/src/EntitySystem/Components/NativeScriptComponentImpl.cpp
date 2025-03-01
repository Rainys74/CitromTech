#include "ScriptComponents.h"
#include "Scripting/Native/NativeScript.h"

namespace Citrom
{
	using namespace Scripting;

	void NativeScriptComponent::SetBehaviorWithString(const std::string& string)
	{
		// do a for loop through all bound classes and set the behaviour based on that class
		// e.g. if (typeName == typeid(ScriptClass)).name()) SetBehavior<ScriptClass>(); // <-- don't use typeid since it's unreproducible on certain platforms, instead require the user to pass the name of the class to the system.

		NativeScriptDB::GetBehaviorCallback(string)(this);
	}
	std::string NativeScriptComponent::GetBehaviorString()
	{
		//for (auto& behaviourPair : NativeScriptDB::GetBehaviorMap())
		//	if (behaviourPair.second == this)
		return std::string();
	}
}