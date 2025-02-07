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
		Math::Vector3 normal;
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
			Math::Matrix4x4 VP; // (4*4)*4=64 bytes fits

			float GridSize; // 4*1=4 bytes
			Math::Vector3 CameraWorldPos; // 4*3=12 bytes fits

			//uint8 padding1[(4+6+4)]; // handled by my new juicy DX11 CB feature
		} m_GridVertUBData;
		struct FragmentUBO
		{
			Math::Vector3 CameraWorldPos; // 4*3=12 bytes
			float GridSize; //4*1=4 bytes - fits into 16

			float GridMinPixelsBetweenCells; //4*1=4 bytes
			float GridCellSize; //4*1=4 bytes - total 8 bytes
			uint8 padding1[8]; // from 24 to 32

			Math::ColorF32x4 GridColorThin; // 4*4=16 does NOT fit into 8 bytes ^ requires padding
			Math::ColorF32x4 GridColorThick; // 4*4=16 fits
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

		//static Scene* s_CurrentScene;

		//static CTL::DArray<Model> s_ModelsToDraw;
	};
}