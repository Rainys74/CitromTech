#include "NativeScript.h"
#include "EntitySystem/Scene.h"
#include "EntitySystem/Components/ScriptComponents.h"

namespace Citrom::Scripting
{
	CTL::StdStrHashMap<NativeScriptDB::BehaviorCallbackPFN> NativeScriptDB::s_BehaviourCallbacks;

	Scene* NativeScript::s_BoundScene = nullptr;

	void NativeScript::ResetScriptComponentInstance()
	{
		CT_CORE_ASSERT(s_BoundScene, "No Scene has been currently bound to the Native Scripting engine!");
	}

	void NativeScript::Update(float64 deltaTime)
	{
		CT_CORE_ASSERT(s_BoundScene, "No Scene has been currently bound to the Native Scripting engine!");

		s_BoundScene->m_SceneRegistry.view<NativeScriptComponent>().each([=](auto entityID, auto& nativeScriptComponent)
		{
			if (!nativeScriptComponent.instance)
			{
				nativeScriptComponent.InstantiateFunction(nativeScriptComponent.instance);
				nativeScriptComponent.instance->m_Entity = Entity(entityID, s_BoundScene);
				nativeScriptComponent.OnCreateFunction(nativeScriptComponent.instance);
			}

			nativeScriptComponent.OnUpdateFunction(nativeScriptComponent.instance, deltaTime);
		});
	}
}