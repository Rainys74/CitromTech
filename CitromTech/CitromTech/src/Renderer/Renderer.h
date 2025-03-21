#pragma once

#include "Core.h"

#include "ShaderCompiler.h"
#include "MaterialSystem.h"
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
#include "CTL/HashMap.h"

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
		EditorRenderer() = default;
		~EditorRenderer() = default;

		void Initialize();
		void Render(Camera* camera, Math::Transform* camTransform);
	private:
		RenderAPI::Device* m_Device;

		struct alignas(16) VertexUBO
		{
			Math::Matrix4x4 VP; // (4*4)*4=64 bytes fits

			float GridSize; // 4*1=4 bytes
			Math::Vector3 CameraWorldPos; // 4*3=12 bytes fits

			//uint8 padding1[(4+6+4)]; // handled by my new juicy DX11 CB feature
		} m_GridVertUBData;
		struct alignas(16) FragmentUBO
		{
			Math::Vector3 CameraWorldPos; // 4*3=12 bytes
			float GridSize; //4*1=4 bytes - fits into 16

			float GridMinPixelsBetweenCells; //4*1=4 bytes
			float GridCellSize; //4*1=4 bytes - total 8 bytes
			uint8 padding1[8]; // from 24 to 32

			Math::ColorF32x4 GridColorThin; // 4*4=16 does NOT fit into 8 bytes ^ requires padding
			Math::ColorF32x4 GridColorThick; // 4*4=16 fits
		} m_GridFragUBData;

		RenderAPI::Shader* m_GridShader;
		RenderAPI::UniformBuffer m_GridVertUB;
		RenderAPI::UniformBuffer m_GridFragUB;

		RenderAPI::PipelineState m_GridPipeline;
	};

	// Global Value access
	CTL::StdStrHashMap<Material*>& Renderer_GetMaterials();
	Material* Renderer_GetMaterial(const std::string& name);
	Material* Renderer_CreateMaterial(const std::string& materialName, const std::string& shaderName); // returns the pointer to the material
	Material* Renderer_GetOrCreateMaterial(const std::string& materialName, const std::string& shaderName);
	Material* Renderer_CreateMaterialFromData(const MaterialData& matData);

	bool Renderer_HasMaterial(const std::string& materialName);

	void Renderer_SaveMaterialsToFiles(); // Should this actually be in the renderer? i don't know..

	CTL::StdStrHashMap<RenderAPI::Shader>& Renderer_GetShaders();
	void Renderer_RecompileShader(const std::string& shaderName);
	void Renderer_RecompileAllShaders();
	//std::string Renderer_GetShaderName(const RenderAPI::Shader& shader); // should not be called for other purposes than debugging

	//void //FinalizeInstance, CompleteInstance, SaveAndCleanup

	inline RenderAPI::Shader* Renderer_GetShader(const std::string& shaderName)
	{
		return &Renderer_GetShaders()[shaderName];
	}

	// TODO: create a modular render path system with
	// extendable render steps
	class Renderer
	{
	public:
		struct CameraData
		{
			CameraData() = default;
			CameraData(Camera* cam)
				: camera(cam) {}
			CameraData(Camera* cam, Math::Transform* transform)
				: camera(cam), cameraTransform(transform) {}

			Camera* camera = nullptr;
			Math::Transform* cameraTransform = nullptr;
		};

		static void Initialize(Platform::Window* window);

		// Submit, BeginScene, EndScene
		static void BeginFrame(Scene* scene, Camera* camera, Math::Transform* cameraTransform); // TODO: decide how many of these arguments are needed
		static void EndFrame();

		//static void Submit(void* model);
		static void SubmitScene(Scene* scene);

		// TODO: temporary
		static void DrawTest(Camera* camera, Math::Transform* cameraTransform);

		// ImGui
        static void ImGuiInit();
		static void ImGuiNewFrame();
		static void ImGuiRenderDrawData(void* imDrawData);

		// Statistics
		//struct Stats
		//{
		//	uint32 drawCalls = 0;
		//};
		//static void ResetStats();
		//static Stats GetStats();
	private:
		// RenderMesh
		static void RenderMesh(const Mesh& mesh);
		static void RenderMeshWithMaterial(Mesh& mesh, Material& material);
	private:
		static RenderAPI::Device* m_Device;
		static EventListener<WindowEvents> s_WindowEventListener;

		static CameraData s_CurrentCamera;
		static Scene* s_CurrentScene;

		//static CTL::DArray<Model> s_ModelsToDraw;
	};

	/*class RenderPath
	{
	public:
		void RenderModel(Material* material);
	};*/
}
