#include "Math/matrice.h"

void mat3x3::initMatrixWithRowVectors(Vector3D row[3])
{
    for (int i = 0; i < 3; ++i) {
        data[i * 3] = row[i].x;
        data[i * 3 + 1] = row[i].y;
        data[i * 3 + 2] = row[i].z;
    }
}

void mat3x3::printMatrix()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << data[i * 3 + j] << " ";
        }
        std::cout << std::endl;
    }
}

float mat3x3::TraceMatrix() const
{
    float result = 0;
    for (int i = 0; i < 3; ++i) {
        result += data[i * 3 + i];
    }
    return result;
}

void mat3x3::DiagonalMatrix()
{
    float result = 0;
    for (int i = 0; i < 3; ++i) {
        std::cout << data[i * 3 + i] << " ";
    }
    std::cout << std::endl;
}

void mat3x3::OppositeMatrix()
{
    for (int i = 0; i < 9; ++i) {
        data[i] = -data[i];
    }
}

void mat3x3::TransposeMatrix()
{
    float temp;
    for (int i = 0; i < 3; ++i) {
        for (int j = i + 1; j < 3; ++j) {
            temp = data[i * 3 + j];
            data[i * 3 + j] = data[j * 3 + i];
            data[j * 3 + i] = temp;
        }
    }
}

void mat3x3::AddMatrices(const mat3x3& other)
{
    for (int i = 0; i < 9; ++i) {
        data[i] += other.data[i];
    }
}

void mat3x3::MultiplyByScalar(float scalar)
{
    for (int i = 0; i < 9; ++i) {
        data[i] *= scalar;
    }
}

void mat3x3::MultiplyMatrices(const mat3x3& other)
{
    float result[9];

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            float sum = 0;
            for (int k = 0; k < 3; ++k) {
                sum += data[i * 3 + k] * other.data[k * 3 + j];
            }
            result[i * 3 + j] = sum;
        }
    }

    for (int i = 0; i < 9; ++i) {
        data[i] = result[i];
    }
}

float mat3x3::determinant() const {
    float det = data[0] * (data[4] * data[8] - data[5] * data[7])
        - data[1] * (data[3] * data[8] - data[5] * data[6])
        + data[2] * (data[3] * data[7] - data[4] * data[6]);
    return det;
}

mat3x3 mat3x3::IdentitiesMatrix()
{
    return mat3x3(1, 0, 0,
                  0, 1, 0, 
                  0, 0, 1
    );
}

void mat3x3::gaussJordan()
{
    float* matrixPtr = data;

    int lead = 0;
    for (int r = 0; r < 3; ++r) {
        if (3 <= lead)
            break;

        int i = r;
        while (matrixPtr[i * 3 + lead] == 0) {
            ++i;
            if (3 == i) {
                i = r;
                ++lead;
                if (3 == lead)
                    return;
            }
        }

        for (int j = 0; j < 3; ++j)
            std::swap(matrixPtr[r * 3 + j], matrixPtr[i * 3 + j]);

        float val = matrixPtr[r * 3 + lead];
        for (int j = 0; j < 3; ++j)
            matrixPtr[r * 3 + j] /= val;

        for (int i = 0; i < 3; ++i) {
            if (i != r) {
                float val = matrixPtr[i * 3 + lead];
                for (int j = 0; j < 3; ++j)
                    matrixPtr[i * 3 + j] -= val * matrixPtr[r * 3 + j];
            }
        }

        ++lead;
    }
}

mat3x3 mat3x3::inverse() const {
    float det = determinant();
    if (det == 0) {
        return mat3x3();
    }

    float invDet = 1.0 / det;
    mat3x3 invMat;
    invMat.data[0] = (data[4] * data[8] - data[5] * data[7]) * invDet;
    invMat.data[1] = (data[2] * data[7] - data[1] * data[8]) * invDet;
    invMat.data[2] = (data[1] * data[5] - data[2] * data[4]) * invDet;
    invMat.data[3] = (data[5] * data[6] - data[3] * data[8]) * invDet;
    invMat.data[4] = (data[0] * data[8] - data[2] * data[6]) * invDet;
    invMat.data[5] = (data[2] * data[3] - data[0] * data[5]) * invDet;
    invMat.data[6] = (data[3] * data[7] - data[4] * data[6]) * invDet;
    invMat.data[7] = (data[1] * data[6] - data[0] * data[7]) * invDet;
    invMat.data[8] = (data[0] * data[4] - data[1] * data[3]) * invDet;

    return invMat;
}

void mat3x3::GLMCompatible()
{
    TransposeMatrix();
}

mat3x3 mat3x3::rotationX(float angle)
{
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    return mat3x3(1, 0, 0,
        0, cosA, -sinA,
        0, sinA, cosA);

}

mat3x3 mat3x3::rotationY(float angle)
{
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    return mat3x3(cosA, 0, sinA,
        0, 1, 0,
        -sinA, 0, cosA);
}

mat3x3 mat3x3::rotationZ(float angle)
{
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    return mat3x3(cosA, -sinA, 0,
        sinA, cosA, 0,
        0, 0, 1);
}

mat3x3 mat3x3::rotationXYZ(const mat3x3& m1, const mat3x3& m2, const mat3x3& m3)
{
    mat3x3 result;
    result = m1 * m2 * m3;
    return result;
}

mat3x3 mat3x3::rotationAroundAxis(const Vector3D& axis, float angle) {
    float cosTheta = std::cos(angle);
    float sinTheta = std::sin(angle);
    float oneMinusCosTheta = 1.0 - cosTheta;

    float axisX = axis.x;
    float axisY = axis.y;
    float axisZ = axis.z;

    mat3x3 rotationMatrix = {
        cosTheta + axisX * axisX * oneMinusCosTheta, axisX * axisY * oneMinusCosTheta - axisZ * sinTheta, axisX * axisZ * oneMinusCosTheta + axisY * sinTheta,
        axisY * axisX * oneMinusCosTheta + axisZ * sinTheta, cosTheta + axisY * axisY * oneMinusCosTheta, axisY * axisZ * oneMinusCosTheta - axisX * sinTheta,
        axisZ * axisX * oneMinusCosTheta - axisY * sinTheta, axisZ * axisY * oneMinusCosTheta + axisX * sinTheta, cosTheta + axisZ * axisZ * oneMinusCosTheta
    };

    return mat3x3(rotationMatrix);
}

mat3x3 mat3x3::operator*(const mat3x3& other) const
{
    mat3x3 result;
    result.data[0] = data[0] * other.data[0] + data[1] * other.data[3] + data[2] * other.data[6];
    result.data[1] = data[0] * other.data[1] + data[1] * other.data[4] + data[2] * other.data[7];
    result.data[2] = data[0] * other.data[2] + data[1] * other.data[5] + data[2] * other.data[8];

    result.data[3] = data[3] * other.data[0] + data[4] * other.data[3] + data[5] * other.data[6];
    result.data[4] = data[3] * other.data[1] + data[4] * other.data[4] + data[5] * other.data[7];
    result.data[5] = data[3] * other.data[2] + data[4] * other.data[5] + data[5] * other.data[8];

    result.data[6] = data[6] * other.data[0] + data[7] * other.data[3] + data[8] * other.data[6];
    result.data[7] = data[6] * other.data[1] + data[7] * other.data[4] + data[8] * other.data[7];
    result.data[8] = data[6] * other.data[2] + data[7] * other.data[5] + data[8] * other.data[8];

    return result;
}

Vector3D mat3x3::operator*(const Vector3D& v) const
{
    Vector3D vR;

    vR.x = (data[0] * v.x) + (data[1] * v.y) + (data[2] * v.z);
    vR.y = (data[3] * v.x) + (data[4] * v.y) + (data[5] * v.z);
    vR.z = (data[6] * v.x) + (data[7] * v.y) + (data[8] * v.z);

    return vR;

}