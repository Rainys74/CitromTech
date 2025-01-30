#include "Quaternion.h"
#include "CitromAssert.h"

/*function eulerToQuaternion(eulerXYZ) {
    var c1 = Math.cos(eulerXYZ[0] / 2),
        c2 = Math.cos(eulerXYZ[1] / 2),
        c3 = Math.cos(eulerXYZ[2] / 2),
        s1 = Math.sin(eulerXYZ[0] / 2),
        s2 = Math.sin(eulerXYZ[1] / 2),
        s3 = Math.sin(eulerXYZ[2] / 2),
        x = s1 * c2 * c3 + c1 * s2 * s3,
        y = c1 * s2 * c3 - s1 * c2 * s3,
        z = c1 * c2 * s3 + s1 * s2 * c3,
        w = c1 * c2 * c3 - s1 * s2 * s3;

    return[x, y, z, w];
};*/

namespace Citrom::Math
{
    namespace ThirdParty
    {
        ///////////////////////////////
        // Quaternion to Euler
        ///////////////////////////////
        enum RotSeq { zyx, zyz, zxy, zxz, yxz, yxy, yzx, yzy, xyz, xyx, xzy, xzx };

        static void twoaxisrot(double r11, double r12, double r21, double r31, double r32, double res[]) {
            res[0] = atan2(r11, r12);
            res[1] = acos(r21);
            res[2] = atan2(r31, r32);
        }

        static void threeaxisrot(double r11, double r12, double r21, double r31, double r32, double res[]) {
            res[0] = atan2(r31, r32);
            res[1] = asin(r21);
            res[2] = atan2(r11, r12);
        }

        static void Quaternion2Euler(const Quaternion& q, double res[], RotSeq rotSeq)
        {
            switch (rotSeq) {
            case zyx:
                threeaxisrot(2 * (q.x * q.y + q.w * q.z),
                    q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
                    -2 * (q.x * q.z - q.w * q.y),
                    2 * (q.y * q.z + q.w * q.x),
                    q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
                    res);
                break;

            case zyz:
                twoaxisrot(2 * (q.y * q.z - q.w * q.x),
                    2 * (q.x * q.z + q.w * q.y),
                    q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
                    2 * (q.y * q.z + q.w * q.x),
                    -2 * (q.x * q.z - q.w * q.y),
                    res);
                break;

            case zxy:
                threeaxisrot(-2 * (q.x * q.y - q.w * q.z),
                    q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
                    2 * (q.y * q.z + q.w * q.x),
                    -2 * (q.x * q.z - q.w * q.y),
                    q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
                    res);
                break;

            case zxz:
                twoaxisrot(2 * (q.x * q.z + q.w * q.y),
                    -2 * (q.y * q.z - q.w * q.x),
                    q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
                    2 * (q.x * q.z - q.w * q.y),
                    2 * (q.y * q.z + q.w * q.x),
                    res);
                break;

            case yxz:
                threeaxisrot(2 * (q.x * q.z + q.w * q.y),
                    q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
                    -2 * (q.y * q.z - q.w * q.x),
                    2 * (q.x * q.y + q.w * q.z),
                    q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
                    res);
                break;

            case yxy:
                twoaxisrot(2 * (q.x * q.y - q.w * q.z),
                    2 * (q.y * q.z + q.w * q.x),
                    q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
                    2 * (q.x * q.y + q.w * q.z),
                    -2 * (q.y * q.z - q.w * q.x),
                    res);
                break;

            case yzx:
                threeaxisrot(-2 * (q.x * q.z - q.w * q.y),
                    q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
                    2 * (q.x * q.y + q.w * q.z),
                    -2 * (q.y * q.z - q.w * q.x),
                    q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
                    res);
                break;

            case yzy:
                twoaxisrot(2 * (q.y * q.z + q.w * q.x),
                    -2 * (q.x * q.y - q.w * q.z),
                    q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
                    2 * (q.y * q.z - q.w * q.x),
                    2 * (q.x * q.y + q.w * q.z),
                    res);
                break;

            case xyz:
                threeaxisrot(-2 * (q.y * q.z - q.w * q.x),
                    q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
                    2 * (q.x * q.z + q.w * q.y),
                    -2 * (q.x * q.y - q.w * q.z),
                    q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
                    res);
                break;

            case xyx:
                twoaxisrot(2 * (q.x * q.y + q.w * q.z),
                    -2 * (q.x * q.z - q.w * q.y),
                    q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
                    2 * (q.x * q.y - q.w * q.z),
                    2 * (q.x * q.z + q.w * q.y),
                    res);
                break;

            case xzy:
                threeaxisrot(2 * (q.y * q.z + q.w * q.x),
                    q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
                    -2 * (q.x * q.y - q.w * q.z),
                    2 * (q.x * q.z + q.w * q.y),
                    q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
                    res);
                break;

            case xzx:
                twoaxisrot(2 * (q.x * q.z - q.w * q.y),
                    2 * (q.x * q.y + q.w * q.z),
                    q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
                    2 * (q.x * q.z + q.w * q.y),
                    -2 * (q.x * q.y - q.w * q.z),
                    res);
                break;
            default:
                CT_CORE_ASSERT(false, "Unknown rotation sequence");
                break;
            }
        }
    }

    Quaternion::Quaternion(std::initializer_list<float32> values)
    {
        CT_CORE_ASSERT(values.size() <= 4, "A Quaternion cannot be initialized with more than 4 values!");
        for (size_t i = 0; i < values.size(); i++)
            operator[](i) = *(values.begin() + i);
    }

    Vector3 Quaternion::ToEulerAngles()
    {
#if 0
        double results[3];
        ThirdParty::Quaternion2Euler(*this, /*(double*)(&result[0])*/ results, ThirdParty::xyz); // xyz, yxz, zyx

        Vector3 result = Vector3(results[0], results[1], results[2]);
        return result;
#else
// Heading = Yaw, Attitude = Pitch, Bank = Roll
// Unity (Left-Handed, Y-Up, Z-Forward): Uses YXZ (Yaw, Pitch, Roll) order.
// Unreal Engine (Left-Handed, Z-Up, X-Forward): Uses XYZ (Roll, Pitch, Yaw) order.
// Godot (Right-Handed, Y-Up, -Z-Forward): Uses YXZ by default.
        /*
        * def quaternion_to_euler(qx, qy, qz, qw):
                """
                Converts a quaternion (qx, qy, qz, qw) to Euler angles (heading, attitude, bank).
                """
                test = qx * qy + qz * qw
                
                if test > 0.499:  # singularity at north pole
                    heading = 2 * math.atan2(qx, qw)
                    attitude = math.pi / 2
                    bank = 0
                    return heading, attitude, bank
                
                if test < -0.499:  # singularity at south pole
                    heading = -2 * math.atan2(qx, qw)
                    attitude = -math.pi / 2
                    bank = 0
                    return heading, attitude, bank
                
                sqx = qx * qx
                sqy = qy * qy
                sqz = qz * qz
                sqw = qw * qw
                
                heading = math.atan2(2 * qy * qw - 2 * qx * qz, 1 - 2 * sqy - 2 * sqz)
                attitude = math.asin(2 * test)
                bank = math.atan2(2 * qx * qw - 2 * qy * qz, 1 - 2 * sqx - 2 * sqz)
                
                return heading, attitude, bank
        */
        auto test = x * y + z * w;
        float32 heading, attitude, bank;

        if (test > 0.499f) // singularity at north pole
        {
            heading = 2 * std::atan2f(x, w);
            attitude = Math::PIf * 0.5f;
            bank = 0.0f;
            return Vector3(heading, attitude, bank);
        }

        if (test < -0.499f) // singularity at south pole
        {
            heading = -2 * std::atan2f(x, w);
            attitude = -Math::PIf * 0.5f;
            bank = 0.0f;
            return Vector3(heading, attitude, bank);
        }

        const float32 sqx = x * x;
        const float32 sqy = y * y;
        const float32 sqz = z * z;
        const float32 sqw = w * w;

        heading = std::atan2f(2 * y * w - 2 * x * z, 1 - 2 * sqy - 2 * sqz);
        attitude = std::asinf(2 * test);
        bank = std::atan2f(2 * x * w - 2 * y * z, 1 - 2 * sqx - 2 * sqz);

        return Vector3(heading, attitude, bank);
#endif
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
    Quaternion Quaternion::Euler(const Vector3& vec3)
    {
        return Euler(vec3.x, vec3.y, vec3.z);
    }

    std::string Quaternion::ToString() const
    {
        std::string prettyString("(");
        prettyString.append(std::to_string(this->x));
        prettyString.append(", ");
        prettyString.append(std::to_string(this->y));
        prettyString.append(", ");
        prettyString.append(std::to_string(this->z));
        prettyString.append(", ");
        prettyString.append(std::to_string(this->w));
        prettyString.append(")");

        return prettyString;
    }
}