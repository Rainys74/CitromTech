#pragma once

#include "LayerSystem.h"

#include "Scripting/Native/NativeScript.h"

namespace Citrom
{
	class NativeScriptLayer : public Layer
	{
	public:
		NativeScriptLayer() = default;
		virtual ~NativeScriptLayer() = default;

		void StartPlaying(Scene* currentScene);
		void StopPlaying();

		//void OnAttach() override;
		//void OnDetach() override;

		void OnUpdate(float64 deltaTime) override;
		void OnTick(float64 fixedDeltaTime) override;
	};
}