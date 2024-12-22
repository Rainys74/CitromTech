#pragma once

#include "LayerSystem.h"

namespace Citrom
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;

		void Begin();
		void End();
	};
}