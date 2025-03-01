#pragma once

#include "CTL/HashMap.h"

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
	class NativeScriptBehaviors // ClassDataBase, NativeScriptDB?
	{
	public:
		using BehaviorCallbackPFN = void(*)(void);

		template<typename T>
		void BindBehavior(const std::string& name) // registers class
		{
			/*m_BehaviourCallbacks[name] = []()
				{
					//SetBehaviour<T>();
				};*/

		}
	private:
		CTL::StdStrHashMap<BehaviorCallbackPFN> m_BehaviourCallbacks;
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