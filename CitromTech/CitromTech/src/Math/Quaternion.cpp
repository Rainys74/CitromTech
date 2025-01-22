#include "Quaternion.h"
#include "CitromAssert.h"

namespace Citrom::Math
{
    Quaternion::Quaternion()
    {
    }

    Quaternion::Quaternion(std::initializer_list<float32> values)
    {
        CT_CORE_ASSERT(values.size() <= 4, "A Quaternion cannot be initialized with more than 4 values!");
        for (size_t i = 0; i < values.size(); i++)
            operator[](i) = *(values.begin() + i);
    }

    Vector3 Quaternion::ToEulerAngles()
    {
        return Vector3();
    }

    Quaternion Quaternion::Euler(float eulerX, float eulerY, float eulerZ)
    {
        Quaternion quat;
        
        // Compute half-angles
        float cosx = Cosine(eulerX * 0.5f);
        float sinx = Sine(eulerX * 0.5f);
        float cosy = Cosine(eulerY * 0.5f);
        float siny = Sine(eulerY * 0.5f);
        float cosz = Cosine(eulerZ * 0.5f);
        float sinz = Sine(eulerZ * 0.5f);

        quat.w = cosx * cosy * cosz + sinx * siny * sinz;
        quat.x = sinx * cosy * cosz - cosx * siny * sinz;
        quat.y = cosx * siny * cosz + sinx * cosy * sinz;
        quat.z = cosx * cosy * sinz - sinx * siny * cosz;
        return quat;
    }
}