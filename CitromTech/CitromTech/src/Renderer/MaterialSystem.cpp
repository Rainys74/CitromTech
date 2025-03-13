#include "MaterialSystem.h"
#include "Renderer.h"
#include "CitromAssert.h"

#include "ShaderInterop/Common_ShaderInterop.h" // CTSI_CBSLOT_MATERIAL

#include "Profiling/Profiler.h"

namespace Citrom
{
    using namespace RenderAPI;

#define SAFE_FREE(x) if ((x) != nullptr) {Memory::Free(x); (x) = nullptr;}

    static RenderAPI::Shader g_NullShader;

    Material::Material()
        : m_Device(nullptr), m_Shader(g_NullShader)
    {
    }
    Material::Material(Shader& shader, const std::string* materialName)
        : m_Device(Device::Get()), m_Shader(shader)//, m_Name(m_Shader.descriptor.name)
    {
        /*
        ShaderDesc sd = {};
        sd.name = shaderName;

        m_Shader = m_Device->CreateShader(&sd);
        */
        
        if (materialName)
            m_Name = *materialName;
        else
            m_Name = m_Shader.descriptor.name;

        uint8 pad[16 /* * 20 */] = {};
        UniformBufferDesc ubd = {};
        ubd.data = &pad;
        ubd.dataBytes = sizeof(pad);
        ubd.usage = Usage::Dynamic;

        m_UniformBuffer = m_Device->CreateUniformBuffer(&ubd);
    }
    Material::Material(const MaterialData& materialData)
        : m_Device(Device::Get()), m_Shader(Renderer_GetShaders()[materialData.shaderName])
    {
        this->Material::Material(m_Shader, (materialData.materialName == "") ? &materialData.shaderName : &materialData.materialName);

        for (const auto& obj : materialData.materialProperties)
        {
            PushProperty(obj.name, obj.propertyFormat, obj.data);
            SetProperty(obj.name, obj.propertyFormat, obj.data);
        }
    }
    Material::~Material()
    {
    }
    void Material::PushPropertyNoAutoPad(const std::string& name, const MaterialFormat format, const void* propertyData)
    {
        CT_PROFILE_MEMBER_FUNCTION();

        uint8* dataBytes = (uint8*)propertyData;

        m_BufferData.Reserve(GetMaterialFormatSize(format));
        for (size_t i = 0; i < GetMaterialFormatSize(format); i++)
            m_BufferData.PushBack(dataBytes[i]);

        m_Properties.PushBack(MaterialProperty{name, format, m_BufferData.Count() - GetMaterialFormatSize(format)});
    }
//#define /*MATERIAL_*/GPU_BYTE_ALLIGNMENT (16)
#define /*MATERIAL_*/GPU_BYTE_ALIGNMENT (16)
    void Material::PushProperty(const std::string& name, const MaterialFormat format, const void* propertyData)
    {
        CT_PROFILE_MEMBER_FUNCTION();

        uint8* dataBytes = (uint8*)propertyData;

        /*size_t bytesLeftForSlot = GPU_BYTE_ALLIGNMENT;
        for (size_t i = 0; i < GetMaterialFormatSize(format); i++)
        {
            if (bytesLeftForSlot < GetMaterialFormatSize(format))
            {
                for (size_t j = 0; j < bytesLeftForSlot; j++)
                    m_BufferData.PushBack(0x00);
                bytesLeftForSlot = GPU_BYTE_ALLIGNMENT;
            }
            else
            {
                m_BufferData.PushBack(dataBytes[i]);
                bytesLeftForSlot--;
            }
        }

        m_Properties.PushBack(MaterialProperty{ name, format, (void*)&m_BufferData[(m_BufferData.Count() - GetMaterialFormatSize(format))] });*/

        uint32 currentOffset = m_BufferData.Count(); // Calculate current offset in the buffer

        /*
        // Determine remaining space in the current 16-byte slot
        size_t currentSlotUsed = currentOffset % 16;
        size_t remainingSpace = (currentSlotUsed > 0) ? (16 - currentSlotUsed) : 16;

        // If the current property doesn't fit in the remaining space, we need padding
        size_t padding = 0;
        if (GetMaterialFormatSize(format) > remainingSpace)
        {
            padding = 16 - currentSlotUsed; // Compute padding needed to align to 16 bytes
        }
        */

        size_t padding = 0;
        size_t leftOverPad = currentOffset % GPU_BYTE_ALIGNMENT; // e.g. 15 = 1, 16 = 0, 17 = 1
        //if (leftOverPad < GetMaterialFormatSize(format)) // e.g. 12 !< 4, 4 < 12 = wtf?!
        //    padding = (GPU_BYTE_ALIGNMENT - (currentOffset % GPU_BYTE_ALIGNMENT)) % GPU_BYTE_ALIGNMENT; // Compute padding needed to align to 16 bytes
        //if (leftOverPad + GetMaterialFormatSize(format) > GPU_BYTE_ALIGNMENT) {
        //    // If the current property doesn't fit in the remaining space in the current slot,
        //    // we need to pad to the next 16-byte boundary.
        //    padding = GPU_BYTE_ALIGNMENT - leftOverPad;
        //}
        //if (leftOverPad + GetMaterialFormatSize(format) > GPU_BYTE_ALIGNMENT) // bullshit, doesn't work, e.g., say the leftover pad is 1 byte (hypothetically), and then the formatSize is 12 (float3), float3 is not going to fit in 1 byte, however 13 is less than 16, so yeah, bullshit.
        //    padding = (GPU_BYTE_ALIGNMENT - (currentOffset % GPU_BYTE_ALIGNMENT)) % GPU_BYTE_ALIGNMENT; // Compute padding needed to align to 16 bytes
        //if (GetMaterialFormatSize(format) > leftOverPad)
        //    padding = (GPU_BYTE_ALIGNMENT - (currentOffset % GPU_BYTE_ALIGNMENT)) % GPU_BYTE_ALIGNMENT; // Compute padding needed to align to 16 bytes
        if (leftOverPad + GetMaterialFormatSize(format) > GPU_BYTE_ALIGNMENT)
            padding = GPU_BYTE_ALIGNMENT - leftOverPad; // Calculate the padding required to align the data to the next 16-byte boundary

        // Compute current aligned offset (AFTER padding is applied)
        //uint32 alignedOffset = m_BufferData.Count() + padding;

        for (size_t i = 0; i < padding; i++)
            m_BufferData.PushBack(0x00);

        const uint32 alignedOffset = m_BufferData.Count();

        for (size_t i = 0; i < GetMaterialFormatSize(format); i++)
            m_BufferData.PushBack(dataBytes[i]);

        m_Properties.PushBack(MaterialProperty{ name, format, alignedOffset});
    }

    void Material::SetProperty(const std::string& name, const MaterialFormat format, const void* newData)
    {
        CT_PROFILE_MEMBER_FUNCTION();

        MaterialProperty* property = GetPropertyByName(name);

        //Memory::Free(property->data, GetMaterialFormatSize(property->propertyFormat));
        //Memory::Reallocate(property->data, GetMaterialFormatSize(format));
        //Memory::Copy(property->data, newData, GetMaterialFormatSize(format));

        Memory::Copy(GetDataPtr(property->bufferOffset), newData, GetMaterialFormatSize(property->propertyFormat));

        //Bind();
        m_Device->SetUniformBufferData(&m_UniformBuffer, m_BufferData.Data(), m_BufferData.Size()); // Causes errors!!
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

    void Material::Bind()
    {
        //m_Device->BindShader(&m_Shader);
        m_Device->RCBindUniformBuffer(&m_UniformBuffer, ShaderType::Fragment, CTSI_CBSLOT_MATERIAL);
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

    /*inline*/ constexpr size_t GetMaterialFormatSize(MaterialFormat format)
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
    constexpr const char* MaterialFormatToString(MaterialFormat format)
    {
#define FORMATSTRING_CASE(x) case (x): return (#x); break;
        switch (format)
        {
            FORMATSTRING_CASE(MaterialFormat::Float32);
            FORMATSTRING_CASE(MaterialFormat::Float32x3);
            FORMATSTRING_CASE(MaterialFormat::Float32x4);
            FORMATSTRING_CASE(MaterialFormat::Float32x4x4);
            FORMATSTRING_CASE(MaterialFormat::Int32);
            FORMATSTRING_CASE(MaterialFormat::UInt32);

            default: return "Unknown"; break;
        }
    }
}
