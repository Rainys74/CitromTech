#pragma once

#include "EntitySystem/ScriptableEntity.h"

namespace Citrom
{
	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		void (*InstantiateFunction)(ScriptableEntity*&) = nullptr;
		void (*DestroyInstanceFunction)(ScriptableEntity*&) = nullptr;

		void (*OnCreateFunction)(ScriptableEntity*);
		void (*OnDestroyFunction)(ScriptableEntity*);

		void (*OnUpdateFunction)(ScriptableEntity*, float64 deltaTime);
		void (*OnTickFunction)(ScriptableEntity*, float64 fixedDeltaTime);

		template<typename T>
		void SetBehavior() // SetBehavior vs SetBehaviour
		{
			InstantiateFunction = [](ScriptableEntity*& _instance)
			{ 
				_instance = new T(); 
			};
			DestroyInstanceFunction = [](ScriptableEntity*& _instance) 
			{ 
				delete (T*)_instance; 
				/*IF_DEBUG(_instance = nullptr);*/ 
			};

			OnCreateFunction = [](ScriptableEntity* _instance) { ((T*)_instance)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* _instance) { ((T*)_instance)->OnDestroy(); };

			OnUpdateFunction = [](ScriptableEntity* _instance, float64 deltaTime) { ((T*)_instance)->OnUpdate(deltaTime); };
			OnTickFunction = [](ScriptableEntity* _instance, float64 fixedDeltaTime) { ((T*)_instance)->OnTick(fixedDeltaTime); };
		}

		void SetBehaviorWithString(const std::string& string)
		{
			// do a for loop through all bound classes and set the behaviour based on that class
			// e.g. if (typeName == typeid(ScriptClass)).name()) SetBehavior<ScriptClass>(); // <-- don't use typeid since it's unreproducible on certain platforms, instead require the user to pass the name of the class to the system.
		}
		std::string GetBehaviorString()
		{
			return std::string();
		}
	};
}