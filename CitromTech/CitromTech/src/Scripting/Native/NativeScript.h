#pragma once

#include "EntitySystem/Scene.h"

namespace Citrom::Scripting
{
	/*class NativeScript
	{
	public:
	private:
		ScriptableEntity* instance;
	};*/

	class NativeScript
	{
	public:
		static void SetSceneInstance(Scene* scene) { s_BoundScene = scene; }

		static void Update(float64 deltaTime);
	private:
		static Scene* s_BoundScene;
	};
}