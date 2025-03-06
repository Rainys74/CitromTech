#pragma once

#include "CTL/HashMap.h"
#include "EntitySystem/Components/ScriptComponents.h"

namespace Citrom
{
	class Scene;
}
namespace Citrom::Scripting
{
	/*class NativeScript
	{
	public:
	private:
		ScriptableEntity* instance;
	};*/

	class NativeScriptDB // ClassDataBase
	{
	public:
		using BehaviorCallbackPFN = void(*)(NativeScriptComponent* nativeScriptComponent);

		/*template<typename T>
		void BindBehavior(const std::string& name) // registers script class
		{
			/*m_BehaviourCallbacks[name] = []()
				{
					//SetBehaviour<T>();
				};* /

		}*/
		template<typename T>
		static void RegisterBehavior(const std::string& name)
		{
			s_BehaviourCallbacks[name] = [](NativeScriptComponent* nsc) // name == "" ? T::_GetBehaviorName() : name
			{
				nsc->SetBehavior<T>();
				//nsc->SetBehaviorString(name);
			};
		}
		
		FORCE_INLINE static BehaviorCallbackPFN GetBehaviorCallback(const std::string& behaviourName) { return s_BehaviourCallbacks[behaviourName]; }
		FORCE_INLINE static CTL::StdStrHashMap<BehaviorCallbackPFN>& GetBehaviorMap() { return s_BehaviourCallbacks; }
		FORCE_INLINE static bool HasBehavior(const std::string& behaviorName) { return s_BehaviourCallbacks.find(behaviorName) != s_BehaviourCallbacks.end(); }

	public:
		//void RegisterFunction(const std::string& functionName, void(*functionPtr)(void*)) {} // Registers/Binds Method/Function to system
		//void CallFunction(const std::string& functionName, void* userArgs = nullptr) {}
		//
		//void RegisterVariable(const std::string& variableName, void(*setterFunction)(void*), void* (*getterFunction)(void)) {} // you should probably use names

		template<typename Ret, typename... Args>
		using FunctionCallbackPFN = Ret(*)(Args...);

		/*template<typename Ret, typename... Args>
		static Ret CallFunction(const std::string& functionName, Args... args)
		{
			auto it = s_FunctionCallbacks.find(functionName);
			if (it != s_FunctionCallbacks.end())
			{
				Ret ret;
				CTL::DArray<void*> dArgs = { &args... };
				it->second(&it->second, &ret, dArgs);
				return ret;
			}
		}

		template<typename Ret, typename... Args>
		static void RegisterFunction(const std::string& functionName, FunctionCallbackPFN<Ret, Args...> functionPtr)
		{
			s_FunctionCallbacks[functionName] = [](void* func, void* ret, CTL::DArray<void*> args)
			{
				NativeScriptDB::CallFunction<Ret, Args...>(functionName, args);
			};
		}*/
	private:
		static CTL::StdStrHashMap<BehaviorCallbackPFN> s_BehaviourCallbacks;

		using FunctionCallbackWrapper = void(*)(void* func, void* ret, CTL::DArray<void*> args);
		static CTL::StdStrHashMap<FunctionCallbackWrapper> s_FunctionCallbacks;
	};
	class NativeScript
	{
	public:
		static void SetSceneInstance(Scene* scene) { s_BoundScene = scene; }

		enum class ScriptComponentCallbackType
		{
			OnEnable,
			OnDisable
		};
		static void ScriptCallback(ScriptComponentCallbackType callbackType);

		static void InstantiateScriptComponentInstance();
		static void DestroyScriptComponentInstance();
		static void Update(float64 deltaTime);
		static void Tick(float64 fixedDeltaTime);
	private:
		static Scene* s_BoundScene;
	};
}
