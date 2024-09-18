#define _USE_MATH_DEFINES

#include "Math/matrice.h"

float ConvertDegToRad(float degrees)
{
    return ((M_PI / 180.0) * degrees);
}

mat4x4 mat4x4::GetTranslationMatrix(Vector3D position)
{
    return {
        1, 0, 0, position.x,
        0, 1, 0, position.y,
        0, 0, 1, position.z,
        0, 0, 0, 1,
    };
}

mat4x4 mat4x4::GetRotationMatrix(Vector3D anglesDeg)
{
    anglesDeg = { ConvertDegToRad(anglesDeg.x), ConvertDegToRad(anglesDeg.y), ConvertDegToRad(anglesDeg.z) };

    mat4x4 rotationX = {
        1, 0, 0, 0,
        0, cos(anglesDeg.x), sin(anglesDeg.x), 0,
        0, -sin(anglesDeg.x), cos(anglesDeg.x), 0,
        0, 0, 0, 1,
    };

    mat4x4 rotationY = {
        cos(anglesDeg.y), 0, sin(anglesDeg.y), 0,
        0, 1, 0, 0,
        -sin(anglesDeg.y), 0, cos(anglesDeg.y), 0,
        0, 0, 0, 1,
    };

    mat4x4 rotationZ = {
        cos(anglesDeg.z), sin(anglesDeg.z), 0, 0,
        -sin(anglesDeg.z), cos(anglesDeg.z), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    return rotationX * rotationY * rotationZ;
}

mat4x4  mat4x4::GetScaleMatrix(Vector3D scale)
{
    return {
        scale.x, 0, 0, 0,
        0, scale.y, 0, 0,
        0, 0, scale.z, 0,
        0, 0, 0, 1,
    };
}

mat4x4 mat4x4::Get_TRS_Matrix(Vector3D position, Vector3D rotationAngles, Vector3D scale)
{
    return GetTranslationMatrix(position) * GetRotationMatrix(rotationAngles) * GetScaleMatrix(scale);
}

mat4x4 mat4x4::operator*(const mat4x4& other) const
{
    mat4x4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += data[i * 4 + k] * other.data[k * 4 + j];
            }
            result.data[i * 4 + j] = sum;
        }
    }
    return result;
}