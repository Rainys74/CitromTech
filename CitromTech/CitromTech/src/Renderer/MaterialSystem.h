#pragma once

#include "RenderAPI/GraphicsDevice.h"
#include "JSON/Writer.h"
#include "JSON/Reader.h"

#include "CTL/String.h"
#include "CTL/HashMap.h"

namespace Citrom
{
	enum class MaterialFormat
	{
		Float32,
		Float32x3,
		Float32x4,
		Float32x4x4,
		Int32,
		UInt32
	};
	constexpr size_t GetMaterialFormatSize(MaterialFormat format);
	constexpr const char* MaterialFormatToString(MaterialFormat format);

	struct MaterialProperty
	{
		std::string name;
		MaterialFormat propertyFormat;
		size_t bufferOffset;
	};

	struct MaterialData
	{
		std::string materialName, shaderName;
		struct MaterialPropertyObjLite
		{
			std::string name;
			MaterialFormat propertyFormat;
			uint8 data[64];
		};
		CTL::DArray<MaterialPropertyObjLite> materialProperties;

		static MaterialData DeserializeJson(const simdjson::dom::element& doc) // TODO: test this!
		{
			MaterialData material;

			//JSON_READER_GET_STRING("MaterialName", material.materialName); // redundant since file name is the material name
			JSON_READER_GET_STRING("ShaderName", material.shaderName);

			simdjson::dom::array objArray = doc["MaterialProperties"];
			for (simdjson::dom::element obj : objArray)
			{
				MaterialPropertyObjLite matProperty;

				JSON_READER_DOC_GET_STRING(obj, "PropertyName", matProperty.name);
				matProperty.propertyFormat = (MaterialFormat)(int64)obj["PropertyFormat"];

				uint8 propertyData[64]; // max matrix4x4 size
				switch (matProperty.propertyFormat)
				{
					default:
						//CT_CORE_ASSERT(false, "Unknown Material Format!");
						break;

					case MaterialFormat::Float32:
						*(float32*)propertyData = (float32)(double)obj["Value"];
						break;
					case MaterialFormat::Float32x3:
						*(float32*)propertyData = (float32)(double)obj["Value0"];
						*((float32*)propertyData + 1) = (float32)(double)obj["Value1"];
						*((float32*)propertyData + 2) = (float32)(double)obj["Value2"];
						break;
					case MaterialFormat::Float32x4:
						*(float32*)propertyData = (float32)(double)obj["Value0"];
						*((float32*)propertyData + 1) = (float32)(double)obj["Value1"];
						*((float32*)propertyData + 2) = (float32)(double)obj["Value2"];
						*((float32*)propertyData + 3) = (float32)(double)obj["Value3"];
						break;

					case MaterialFormat::Float32x4x4:
						for (int j = 0; j < 4; ++j)
						{
							for (int i = 0; i < 4; ++i)
							{
								*((float32*)propertyData + i * 4 + j) = (float32)(double)obj[std::string("Value" + std::to_string(i) + "_" + std::to_string(j)).c_str()];
							}
						}
						break;

					case MaterialFormat::Int32:
						*(int32*)propertyData = (int32)(int64)obj["Value"];
						break;
					case MaterialFormat::UInt32:
						*(uint32*)propertyData = (uint32)(uint64)obj["Value"];
						break;
				}
				Memory::Copy(matProperty.data, propertyData, 64); //matProperty.data = propertyData;

				material.materialProperties.PushBack(matProperty);
			}

			return material;
		}
	};

	// TODO: "macrofy" the system to allow to register properties to a member variable for easier access, also implement a texture system.
	class Material
	{
	public:
		//Material(const std::string& shaderName = "Standard");
		Material();
		Material(RenderAPI::Shader& shader, const std::string* materialName = nullptr);
		Material(const MaterialData& materialData);
		Material(const Material&) = default;
		~Material();

		void PushPropertyNoAutoPad(const std::string& name, const MaterialFormat format, const void* propertyData);
		void PushProperty(const std::string& name, const MaterialFormat format, const void* propertyData);
		void SetProperty(const std::string& name, const MaterialFormat format, const void* newData);
		void Render();

		void Bind();

		//virtual void RegisterProperties() = 0;

		template<typename T>
		void Set(const std::string& name, const T value)
		{
			//CT_CORE_ASSERT(false, "Unimplemented template type.");
		}

		template<>
		void Set(const std::string& name, const float32 value)
		{
			SetProperty(name, MaterialFormat::Float32, &value);
		}

		FORCE_INLINE RenderAPI::Shader* GetShader() { return &m_Shader; };
		FORCE_INLINE RenderAPI::UniformBuffer* GetUniformBuffer() { return &m_UniformBuffer; };
		FORCE_INLINE std::string GetName() const { return m_Name; }
		FORCE_INLINE auto& GetProperties() { return m_Properties; }

		FORCE_INLINE void* GetDataPtr(const size_t bufferOffset) { return (void*)&m_BufferData[bufferOffset]; }
		FORCE_INLINE void* GetDataPtr(const size_t bufferOffset) const { return (void*)&m_BufferData[bufferOffset]; }
	public:
		// Serialization
		void SerializeJson(rapidjson::Document& doc, rapidjson::Document::AllocatorType& allocator) const
		{
			doc.SetObject();

			//JSON_WRITER_SET_STRING("MaterialName", GetName()); // redundant since file name is the material name
			JSON_WRITER_SET_STRING("ShaderName", m_Shader.descriptor.name);

			//doc.SetArray();
			rapidjson::Value objArray(rapidjson::kArrayType);
			for (const auto& obj : m_Properties)
			{
				rapidjson::Value jsonObject(rapidjson::kObjectType);

				jsonObject.AddMember("PropertyName", rapidjson::Value(obj.name.c_str(), allocator), allocator);
				jsonObject.AddMember("PropertyFormat", (int64)obj.propertyFormat, allocator);

				void* dataPtr = GetDataPtr(obj.bufferOffset);
				switch (obj.propertyFormat)
				{
					default:
						//CT_CORE_ASSERT(false, "Unknown Material Format!");
						break;

					case MaterialFormat::Float32:
						jsonObject.AddMember("Value", *static_cast<float*>(dataPtr), allocator);
						break;
					case MaterialFormat::Float32x3:
						jsonObject.AddMember("Value0", static_cast<float*>(dataPtr)[0], allocator); // Value1?
						jsonObject.AddMember("Value1", static_cast<float*>(dataPtr)[1], allocator);
						jsonObject.AddMember("Value2", static_cast<float*>(dataPtr)[2], allocator);
						break;
					case MaterialFormat::Float32x4:
						jsonObject.AddMember("Value0", static_cast<float*>(dataPtr)[0], allocator);
						jsonObject.AddMember("Value1", static_cast<float*>(dataPtr)[1], allocator);
						jsonObject.AddMember("Value2", static_cast<float*>(dataPtr)[2], allocator);
						jsonObject.AddMember("Value3", static_cast<float*>(dataPtr)[3], allocator);
						break;
					// Matrix Begin
					case MaterialFormat::Float32x4x4:
						jsonObject.AddMember("Value0_0", static_cast<float**>(dataPtr)[0][0], allocator);
						jsonObject.AddMember("Value1_0", static_cast<float**>(dataPtr)[1][0], allocator);
						jsonObject.AddMember("Value2_0", static_cast<float**>(dataPtr)[2][0], allocator);
						jsonObject.AddMember("Value3_0", static_cast<float**>(dataPtr)[3][0], allocator);

						jsonObject.AddMember("Value0_1", static_cast<float**>(dataPtr)[0][1], allocator);
						jsonObject.AddMember("Value1_1", static_cast<float**>(dataPtr)[1][1], allocator);
						jsonObject.AddMember("Value2_1", static_cast<float**>(dataPtr)[2][1], allocator);
						jsonObject.AddMember("Value3_1", static_cast<float**>(dataPtr)[3][1], allocator);

						jsonObject.AddMember("Value0_2", static_cast<float**>(dataPtr)[0][2], allocator);
						jsonObject.AddMember("Value1_2", static_cast<float**>(dataPtr)[1][2], allocator);
						jsonObject.AddMember("Value2_2", static_cast<float**>(dataPtr)[2][2], allocator);
						jsonObject.AddMember("Value3_2", static_cast<float**>(dataPtr)[3][2], allocator);

						jsonObject.AddMember("Value0_3", static_cast<float**>(dataPtr)[0][3], allocator);
						jsonObject.AddMember("Value1_3", static_cast<float**>(dataPtr)[1][3], allocator);
						jsonObject.AddMember("Value2_3", static_cast<float**>(dataPtr)[2][3], allocator);
						jsonObject.AddMember("Value3_3", static_cast<float**>(dataPtr)[3][3], allocator);

						//jsonObject.AddMember("Value0_0", static_cast<float**>(dataPtr)[0][0], allocator);
						//jsonObject.AddMember("Value0_1", static_cast<float**>(dataPtr)[0][1], allocator);
						//jsonObject.AddMember("Value0_2", static_cast<float**>(dataPtr)[0][2], allocator);
						//jsonObject.AddMember("Value0_3", static_cast<float**>(dataPtr)[0][3], allocator);
						break;
					// Matrix End

					case MaterialFormat::Int32:
						jsonObject.AddMember("Value", *static_cast<int32*>(dataPtr), allocator);
						break;
					case MaterialFormat::UInt32:
						jsonObject.AddMember("Value", *static_cast<uint32*>(dataPtr), allocator);
						break;
				}

				//doc.PushBack(jsonObject, allocator);
				objArray.PushBack(jsonObject, allocator);
			}
			doc.AddMember("MaterialProperties", objArray, allocator);
		}
		//static Material DeserializeJsonOnDemand(simdjson::ondemand::object doc)
		static Material DeserializeJson(const simdjson::dom::element& doc) // TODO: test this!, also might cause issues due to there being no constructor
		{
			Material material;

			JSON_READER_GET_STRING("ShaderName", material.m_Shader.descriptor.name);

			simdjson::dom::array objArray = doc["MaterialProperties"];
			for (simdjson::dom::element obj : objArray)
			{
				MaterialProperty matProperty;

				JSON_READER_DOC_GET_STRING(obj, "PropertyName", matProperty.name);
				matProperty.propertyFormat = (MaterialFormat)(int64)obj["PropertyFormat"];
				
				uint8 propertyData[64]; // max matrix4x4 size
				switch (matProperty.propertyFormat)
				{
					default:
						//CT_CORE_ASSERT(false, "Unknown Material Format!");
						break;

					case MaterialFormat::Float32:
						*(float32*)propertyData = (float32)(double)obj["Value"];
						break;
					case MaterialFormat::Float32x3:
						*(float32*)propertyData = (float32)(double)obj["Value0"];
						*((float32*)propertyData + 1) = (float32)(double)obj["Value1"];
						*((float32*)propertyData + 2) = (float32)(double)obj["Value2"];
						break;
					case MaterialFormat::Float32x4:
						*(float32*)propertyData = (float32)(double)obj["Value0"];
						*((float32*)propertyData + 1) = (float32)(double)obj["Value1"];
						*((float32*)propertyData + 2) = (float32)(double)obj["Value2"];
						*((float32*)propertyData + 3) = (float32)(double)obj["Value3"];
						break;

					case MaterialFormat::Float32x4x4:
						for (int j = 0; j < 4; ++j)
						{
							for (int i = 0; i < 4; ++i)
							{
								*((float32*)propertyData + i * 4 + j) = (float32)(double)obj[std::string("Value" + std::to_string(i) + "_" + std::to_string(j)).c_str()];
							}
						}
						break;
						
					case MaterialFormat::Int32:
						*(int32*)propertyData = (int32)(int64)obj["Value"];
						break;
					case MaterialFormat::UInt32:
						*(uint32*)propertyData = (uint32)(uint64)obj["Value"];
						break;
				}

				material.PushProperty(matProperty.name, matProperty.propertyFormat, propertyData);
				material.SetProperty(matProperty.name, matProperty.propertyFormat, propertyData);
			}

			return material;
		}
	protected:
		MaterialProperty* GetPropertyByName(const std::string& name);
	private:
		RenderAPI::Shader& m_Shader;
		RenderAPI::UniformBuffer m_UniformBuffer;
		std::string m_Name;

		//CTL::StdStrHashMap<MaterialProperty> m_Properties; CTL::DArray<MaterialProperty> properties;
		CTL::DArray<MaterialProperty> m_Properties;
		CTL::DArray<uint8> m_BufferData;

		RenderAPI::Device* m_Device;
	};
	struct MaterialInstance
	{
		//Material* mat;
		//Material values;
	};

/*#define MATERIAL_BUFFER_CLASS(NAME)
//#define MATERIAL_BUFFER_VAR(VAR) VAR; void Set ## VAR(VAR TYPEvalue) {VAR = value;}

	template<typename CB>
	class Material
	{
	public:
		Material(const std::string& shaderName = "Standard")
			: m_Device(RenderAPI::Device::Get())
		{
			RenderAPI::ShaderDesc sd = {};
			sd.name = shaderName;

			m_Shader = m_Device->CreateShader(&sd);

			RenderAPI::UniformBufferDesc ubd = {};
			ubd.data = &m_CBData;
			ubd.dataBytes = sizeof(CB);
			ubd.usage = RenderAPI::Usage::Dynamic;

			m_UniformBuffer = m_Device->CreateUniformBuffer(&ubd);
		}
		~Material() = default;

		void Bind()
		{
			m_Device->BindShader(&m_Shader);
			m_Device->BindUniformBuffer(&m_UniformBuffer, RenderAPI::ShaderType::Vertex, 1);
		}
		void UpdateData()
		{
			Bind();
			m_Device->SetUniformBufferData(&m_UniformBuffer, &m_CBData, sizeof(CB));
		}

		FORCE_INLINE void SetBufferData(const CB& newBuffer) { m_CBData = newBuffer; }
		FORCE_INLINE CB* GetBufferData() { return &m_CBData; }
		FORCE_INLINE constexpr size_t GetBufferSize() { return sizeof(CB); }
	private:
		CB m_CBData;

		RenderAPI::Shader m_Shader;
		RenderAPI::UniformBuffer m_UniformBuffer;

		RenderAPI::Device* m_Device;
	};*/
}