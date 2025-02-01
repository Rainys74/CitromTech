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
	
	// Additional editor stuff
	class EditorRenderer
	{
	public:
		EditorRenderer()
			: m_Device(RenderAPI::Device::Get()) {}
		~EditorRenderer() = default;

		void Initialize();
		void Render(Camera* camera, Math::Transform* camTransform);
	private:
		RenderAPI::Device* m_Device;

		struct VertexUBO
		{
			Math::Matrix4x4 VP;
			float GridSize;
			Math::Vector3 CameraWorldPos;

			uint8 padding1[(4+6+4)];
		} m_GridVertUBData;
		struct FragmentUBO
		{
			Math::Vector3 CameraWorldPos;
			float GridSize;
			float GridMinPixelsBetweenCells;
			float GridCellSize;

			Math::ColorF32x4 GridColorThin;
			Math::ColorF32x4 GridColorThick;

			uint8 padding1[8];
		} m_GridFragUBData;

		RenderAPI::Shader m_GridShader;
		RenderAPI::UniformBuffer m_GridVertUB;
		RenderAPI::UniformBuffer m_GridFragUB;
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