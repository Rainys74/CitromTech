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
		template<typename T> // serves no real purpose just for syntactic sugar. Actually it might just do
		static void RegisterBehavior(const std::string& name)
		{
			s_BehaviourCallbacks[name] = [](NativeScriptComponent* nsc)
			{
				nsc->SetBehavior<T>();
			};
		}
		
		FORCE_INLINE static BehaviorCallbackPFN GetBehaviorCallback(const std::string& behaviourName) { return s_BehaviourCallbacks[behaviourName]; }
		FORCE_INLINE static CTL::StdStrHashMap<BehaviorCallbackPFN>& GetBehaviorMap() { return s_BehaviourCallbacks; }
		FORCE_INLINE static bool HasBehavior(const std::string& behaviorName) { return s_BehaviourCallbacks.find(behaviorName) != s_BehaviourCallbacks.end(); }
	private:
		static CTL::StdStrHashMap<BehaviorCallbackPFN> s_BehaviourCallbacks;
	};
	class NativeScript
	{
	public:
		static void SetSceneInstance(Scene* scene) { s_BoundScene = scene; }

		static void ResetScriptComponentInstance();
		static void Update(float64 deltaTime);
		static void Tick(float64 fixedDeltaTime);
	private:
		static Scene* s_BoundScene;
	};
}