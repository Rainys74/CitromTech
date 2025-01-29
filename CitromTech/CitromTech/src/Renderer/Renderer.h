#pragma once

#include "Core.h"

#include "ShaderCompiler.h"
#include "Camera.h"
#include "RenderAPI/GraphicsDevice.h"
#include "Math/Vector.h"
#include "Math/Matrix4x4.h"
#include "Math/Color.h"
#include "Events/WindowEvents.h"

#include "EntitySystem/Scene.h"
//#include "EntitySystem/Components/EssentialComponents.h"
#include "Math/Transform.h"

#include "CTL/ScopedPointer.h"

namespace Citrom
{
	struct Vertex
	{
		Math::Vector3 position;
		struct TextureCoordinate
		{
			float32 u, v;
		} texCoord;
	};

	struct Mesh
	{
		CTL::DArray<Vertex> vertices;
		CTL::DArray<uint32> indices;
	};
	
	// TODO: create a modular render path system with
	// extendable render steps
	class Renderer
	{
	public:
		static void Initialize(Platform::Window* window);

		// Submit, BeginScene, EndScene
		static void BeginFrame();
		static void EndFrame();

		// Main Submission
		static void Begin();
		static void End();

		//static void Submit(void* model);
		static void SubmitScene(Scene* scene);

		// TODO: temporary
		static void DrawTest(Camera* camera, Math::Transform* cameraTransform);

		// ImGui
		static void ImGuiInit();
		static void ImGuiRenderDrawData(void* imDrawData);
	private:
		// RenderMesh
		static void RenderMesh(const Mesh& mesh);
	private:
		static RenderAPI::Device* m_Device;
		static EventListener<WindowEvents> s_WindowEventListener;

		//static CTL::DArray<Model> s_ModelsToDraw;
	};
}