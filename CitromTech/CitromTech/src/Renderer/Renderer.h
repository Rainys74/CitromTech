#pragma once

#include "Core.h"

#include "RenderAPI/GraphicsDevice.h"
#include "Math/Vector.h"

#include "CTL/ScopedPointer.h"

namespace Citrom
{
	struct Vertex
	{
		Math::Vector3 position;
	};

	class Renderer
	{
	public:
		static void Initialize();

		// Submit, BeginScene, EndScene

		// TODO: temporary
		static void DrawTest();

		// ImGui
		static void ImGuiInit();
		static void ImGuiRenderDrawData();
	private:
		// RenderMesh
	private:
		static RenderAPI::Device* m_Device;
	};
}