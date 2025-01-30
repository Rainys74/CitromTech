#include "MaterialSystem.h"

namespace Citrom
{
    using namespace RenderAPI;

#define SAFE_FREE(x) if ((x) != nullptr) {Memory::Free(x); (x) = nullptr;}

    Material::Material(const std::string& shaderName)
        : m_Device(Device::Get())
    {
        ShaderDesc sd = {};
        sd.name = shaderName;

        //m_Shader = m_Device->CreateShader(&sd);
        
        UniformBufferDesc ubd = {};
        ubd.data = nullptr;
        ubd.dataBytes = 0;
        ubd.usage = Usage::Dynamic;

        //m_UniformBuffer = m_Device->CreateUniformBuffer(&ubd);
    }
    Material::~Material()
    {
        for (auto& property : m_Properties)
        {
            Memory::Free(property.data, GetMaterialFormatSize(property.propertyFormat));
        }
    }
    void Material::PushProperty(const std::string& name, const MaterialFormat format, const void* propertyData)
    {
        void* data = Memory::Allocate(GetMaterialFormatSize(format));
        Memory::Copy(data, propertyData, GetMaterialFormatSize(format));

        m_Properties.PushBack(MaterialProperty{name, format, data});
    }

    void Material::UpdateData(const std::string& name, const void* newData, const MaterialFormat format)
    {
        MaterialProperty* property = GetPropertyByName(name);

        //Memory::Free(property->data, GetMaterialFormatSize(property->propertyFormat));
        Memory::Reallocate(property->data, GetMaterialFormatSize(format));
        Memory::Copy(property->data, newData, GetMaterialFormatSize(format));

        //m_Device->BindUniformBuffer(&m_UniformBuffer);
        //m_Device->SetUniformBufferData(&m_UniformBuffer, newData, GetMaterialFormatSize(format));
    }

    void Material::Render()
    {
        m_Device->BindUniformBuffer(&m_UniformBuffer);

        size_t bufferSize = 0;
        for (auto& property : m_Properties)
            bufferSize += GetMaterialFormatSize(property.propertyFormat);

        uint8* bufferData = (uint8*)Memory::Allocate(bufferSize); // one byte
        size_t bufferProgress = 0;
        for (auto& property : m_Properties)
        {
            Memory::Copy(bufferData, static_cast<uint8*>(property.data) + bufferProgress, GetMaterialFormatSize(property.propertyFormat));
            bufferProgress += GetMaterialFormatSize(property.propertyFormat);
        }

        m_Device->SetUniformBufferData(&m_UniformBuffer, bufferData, bufferSize);

        Memory::Free(bufferData);
    }

    MaterialProperty* Material::GetPropertyByName(const std::string& name)
    {
        for (auto& property : m_Properties)
        {
            if (property.name == name)
                return &property;
        }
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