#pragma once

#include "Core.h"

#include "RenderAPI/GraphicsDevice.h"
#include "Math/Vector.h"

namespace Citrom
{
	struct Vertex
	{
		Math::Vector3 position;
	};

	class Renderer
	{
	public:
		// Submit

		// TODO: temporary
		static void DrawTest();
	private:
		// RenderMesh
	private:
		static RenderAPI::Device m_Device;
	};
}