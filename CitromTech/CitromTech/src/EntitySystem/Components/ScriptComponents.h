#pragma once

#include "EntitySystem/ScriptableEntity.h"

namespace Citrom
{
	class NullScriptBehavior : public ScriptableEntity // NullNativeScriptBehaviour?
	{
	public:
		static const char* _GetBehaviorName() { return "NullScriptBehavior"; }
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;
		std::string behaviorName;

		void (*InstantiateFunction)(ScriptableEntity*&) = nullptr;
		void (*DestroyInstanceFunction)(ScriptableEntity*&) = nullptr;

		void (*OnCreateFunction)(ScriptableEntity*) = nullptr;
		void (*OnDestroyFunction)(ScriptableEntity*) = nullptr;

		void (*OnUpdateFunction)(ScriptableEntity*, float64 deltaTime) = nullptr;
		void (*OnTickFunction)(ScriptableEntity*, float64 fixedDeltaTime) = nullptr;

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

			if constexpr (requires { T::_GetBehaviorName(); })
				behaviorName = T::_GetBehaviorName();
			else
				static_assert(requires { T::_GetBehaviorName(); }, "Cannot retrieve the behavior name from a native script class!");

			if constexpr (requires(T t) { t.OnCreate(); })
				OnCreateFunction = [](ScriptableEntity* _instance) { ((T*)_instance)->OnCreate(); };
			if constexpr (requires(T t) { t.OnDestroy(); })
				OnDestroyFunction = [](ScriptableEntity* _instance) { ((T*)_instance)->OnDestroy(); };

			if constexpr (requires(T t, float64 deltaTime) { t.OnUpdate(deltaTime); })
				OnUpdateFunction = [](ScriptableEntity* _instance, float64 deltaTime) { ((T*)_instance)->OnUpdate(deltaTime); };
			if constexpr (requires(T t, float64 fixedDeltaTime) { t.OnTick(fixedDeltaTime); })
				OnTickFunction = [](ScriptableEntity* _instance, float64 fixedDeltaTime) { ((T*)_instance)->OnTick(fixedDeltaTime); };
		}

		void SetBehaviorWithString(const std::string& string);
		FORCE_INLINE void SetBehaviorString(const std::string& string) { behaviorName = string; }
		FORCE_INLINE std::string GetBehaviorString() { return behaviorName; } // Done, probably add a set behaviour string that gets set automatically using RegisterClass?
	};
}