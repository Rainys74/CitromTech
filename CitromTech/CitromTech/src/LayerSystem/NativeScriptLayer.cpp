#include "NativeScriptLayer.h"

namespace Citrom
{
	void NativeScriptLayer::StartPlaying(Scene* currentScene)
	{
		Scripting::NativeScript::SetSceneInstance(currentScene);
		Scripting::NativeScript::InstantiateScriptComponentInstance();
	}

	void NativeScriptLayer::StopPlaying()
	{
		Scripting::NativeScript::DestroyScriptComponentInstance();
		Scripting::NativeScript::SetSceneInstance(nullptr);
	}

	/*void NativeScriptLayer::OnAttach()
	{
	}

	void NativeScriptLayer::OnDetach()
	{
	}*/

	void NativeScriptLayer::OnUpdate(float64 deltaTime)
	{
		CT_PROFILE_MEMBER_FUNCTION();

		Scripting::NativeScript::Update(deltaTime);
	}

	void NativeScriptLayer::OnTick(float64 fixedDeltaTime)
	{
		CT_PROFILE_MEMBER_FUNCTION();

		Scripting::NativeScript::Tick(fixedDeltaTime);
	}
}