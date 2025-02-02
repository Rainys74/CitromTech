#include "MaterialSystem.h"
#include "CitromAssert.h"

namespace Citrom
{
    using namespace RenderAPI;

#define SAFE_FREE(x) if ((x) != nullptr) {Memory::Free(x); (x) = nullptr;}

    Material::Material(const std::string& shaderName)
        : m_Device(Device::Get())
    {
        ShaderDesc sd = {};
        sd.name = shaderName;

        m_Shader = m_Device->CreateShader(&sd);
        
        uint8 pad[16] = {};
        UniformBufferDesc ubd = {};
        ubd.data = &pad;
        ubd.dataBytes = sizeof(pad);
        ubd.usage = Usage::Dynamic;

        m_UniformBuffer = m_Device->CreateUniformBuffer(&ubd);
    }
    Material::~Material()
    {
    }
    void Material::PushProperty(const std::string& name, const MaterialFormat format, const void* propertyData)
    {
        uint8* dataBytes = (uint8*)propertyData;

        m_BufferData.Reserve(GetMaterialFormatSize(format));
        for (size_t i = 0; i < GetMaterialFormatSize(format); i++)
            m_BufferData.PushBack(dataBytes[i]);

        m_Properties.PushBack(MaterialProperty{name, format, (void*)&m_BufferData[(m_BufferData.Count() - GetMaterialFormatSize(format))]});
    }

    void Material::UpdateData(const std::string& name, const void* newData, const MaterialFormat format)
    {
        MaterialProperty* property = GetPropertyByName(name);

        //Memory::Free(property->data, GetMaterialFormatSize(property->propertyFormat));
        //Memory::Reallocate(property->data, GetMaterialFormatSize(format));
        //Memory::Copy(property->data, newData, GetMaterialFormatSize(format));

        Memory::Copy(property->dataPtr, newData, GetMaterialFormatSize(property->propertyFormat));

        m_Device->BindShader(&m_Shader);

        m_Device->BindUniformBuffer(&m_UniformBuffer, ShaderType::Vertex, 1); //hard coded
        m_Device->SetUniformBufferData(&m_UniformBuffer, m_BufferData.Data(), m_BufferData.Size());
    }

    void Material::Render()
    {
        //m_Device->BindUniformBuffer(&m_UniformBuffer);
        //
        //size_t bufferSize = 0;
        //for (auto& property : m_Properties)
        //    bufferSize += GetMaterialFormatSize(property.propertyFormat);
        //
        //uint8* bufferData = (uint8*)Memory::Allocate(bufferSize); // one byte
        //size_t bufferProgress = 0;
        //for (auto& property : m_Properties)
        //{
        //    Memory::Copy(bufferData, static_cast<uint8*>(property.data) + bufferProgress, GetMaterialFormatSize(property.propertyFormat));
        //    bufferProgress += GetMaterialFormatSize(property.propertyFormat);
        //}
        //
        //m_Device->SetUniformBufferData(&m_UniformBuffer, bufferData, bufferSize);
        //
        //Memory::Free(bufferData);
    }

    MaterialProperty* Material::GetPropertyByName(const std::string& name)
    {
        for (auto& property : m_Properties)
        {
            if (property.name == name)
                return &property;
        }
        CT_CORE_ASSERT(false, "Could not find material property with this name!");
        return nullptr;
    }

    constexpr size_t GetMaterialFormatSize(MaterialFormat format)
    {
#define FORMATSIZE_CASE(x, y, z) case (x): return sizeof(y) * (z); break

        switch (format)
        {
            FORMATSIZE_CASE(MaterialFormat::Float32, float32, 1);
            FORMATSIZE_CASE(MaterialFormat::Float32x3, float32, 3);
            FORMATSIZE_CASE(MaterialFormat::Float32x4, float32, 4);
            FORMATSIZE_CASE(MaterialFormat::Float32x4x4, float32, 4 * 4);
            FORMATSIZE_CASE(MaterialFormat::Int32, int32, 1);
            FORMATSIZE_CASE(MaterialFormat::UInt32, uint32, 1);

            default: return 0; break;
        }
        return 0;
    }
}