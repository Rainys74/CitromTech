#pragma once

#include "LayerSystem.h"

#include "Input/KeyMouseInputSystem.h"

namespace Citrom
{
	class SimpleInputLayer : public Layer
	{
	public:
		SimpleInputLayer() = default;
		virtual ~SimpleInputLayer() = default;

		void OnAttach() override;
		void OnUpdate(float64 deltaTime) override;
	};
}