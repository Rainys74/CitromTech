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

	// or instead just make a shader/uniform buffer wrapper with functions for setting specific values?
	struct Material
	{
		std::string shaderName; // TODO: shader reference or shader name?

		Math::Color albedoColor;
		// TODO: convert into a class that combines multipliers and textures as well as maybe combines all PBR stuff
		float32 smoothness; // smoothness/glossiness vs roughness
		float32 metallic; // metalness
	};
	struct MaterialInstance
	{
		Material* mat;
		Material values;
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
		static void DrawTest();

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