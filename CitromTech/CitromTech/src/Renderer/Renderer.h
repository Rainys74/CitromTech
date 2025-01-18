#pragma once

#include "Core.h"

#include "ShaderCompiler.h"
#include "RenderAPI/GraphicsDevice.h"
#include "Math/Vector.h"
#include "Events/WindowEvents.h"
#include "EntitySystem/Scene.h"

#include "CTL/ScopedPointer.h"

namespace Citrom
{
	struct Vertex
	{
		Math::Vector3 position;
	};

	struct Mesh
	{
		CTL::DArray<Vertex> m_Vertices;
		CTL::DArray<uint32> m_Indices;
	};

	struct Material
	{
		std::string shaderName;
	};

	struct Model
	{
		Mesh mesh;
		Material material;
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
		static void SubmitScene(const Scene* scene);

		// TODO: temporary
		static void DrawTest();

		// ImGui
		static void ImGuiInit();
		static void ImGuiRenderDrawData(void* imDrawData);
	private:
		// RenderMesh
	private:
		static RenderAPI::Device* m_Device;
		static EventListener<WindowEvents> s_WindowEventListener;

		//static CTL::DArray<Model> s_ModelsToDraw;
	};
}