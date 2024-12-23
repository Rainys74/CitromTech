#pragma once

#include "LayerSystem.h"

#include "Platform/PlatformWindow.h"

namespace Citrom
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;

		void Initialize(Platform::Window* window);
			
		void Begin();
		void End();
	};
}