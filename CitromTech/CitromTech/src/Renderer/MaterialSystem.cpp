#include "MaterialSystem.h"

namespace Citrom
{
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