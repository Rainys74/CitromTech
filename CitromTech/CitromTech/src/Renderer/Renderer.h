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

		// Submit

		// TODO: temporary
		static void DrawTest();
	private:
		// RenderMesh
	private:
		static CTL::ScopedPtr<RenderAPI::Device> m_Device;
	};
}