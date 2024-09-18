#include "Math/matrice.h"

void mat4x4::initMatrixWithRowVectors(Vector4D row[4]) {
    for (int i = 0; i < 4; ++i) {
        for (int i = 0; i < 4; ++i) {
            data[i * 4] = row[i].x;
            data[i * 4 + 1] = row[i].y;
            data[i * 4 + 2] = row[i].z;
            data[i * 4 + 3] = row[i].w;
        }
    }
}

void mat4x4::printMatrix() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << data[i * 4 + j] << " ";
        }
        std::cout << std::endl; 
    }
}

float mat4x4::TraceMatrix() const
{
    float result = 0; 
    for (int i = 0; i < 4; ++i) {
        result += data[i * 4 + i]; 
    }
    return result;
}

void mat4x4::DiagonalMatrix()
{
    float result = 0;
    for (int i = 0; i < 4; ++i) {
        std::cout << data[i * 4 + i] << " "; 
    }
    std::cout << std::endl; 
}

void mat4x4::OppositeMatrix()
{
    for (int i = 0; i < 16; ++i) {
        data[i] = -data[i]; 
    }
}

void mat4x4::TransposeMatrix() {
    float temp;
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            temp = data[i * 4 + j];
            data[i * 4 + j] = data[j * 4 + i];
            data[j * 4 + i] = temp;
        }
    }
}

void mat4x4::AddMatrices(const mat4x4& other)
{
    for (int i = 0; i < 16; ++i) {
        data[i] += other.data[i]; 
    }
}

void mat4x4::MultiplyByScalar(float scalar)
{
    for (int i = 0; i < 16; ++i) {
        data[i] *= scalar;
    }
}

void mat4x4::MultiplyMatrices(const mat4x4& other) {
    float result[16]; 

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            float sum = 0;
            for (int k = 0; k < 4; ++k) {
                sum += data[i * 4 + k] * other.data[k * 4 + j];
            }
            result[i * 4 + j] = sum; 
        }
    }

    for (int i = 0; i < 16; ++i) {
        data[i] = result[i];
    }
}

float mat4x4::determinant() const {
    float det = 0;

    det += data[0] * Determinant3x3(data[5], data[6], data[7], data[9], data[10], data[11], data[13], data[14], data[15]);
    det -= data[1] * Determinant3x3(data[4], data[6], data[7], data[8], data[10], data[11], data[12], data[14], data[15]);
    det += data[2] * Determinant3x3(data[4], data[5], data[7], data[8], data[9], data[11], data[12], data[13], data[15]);
    det -= data[3] * Determinant3x3(data[4], data[5], data[6], data[8], data[9], data[10], data[12], data[13], data[14]);

    return det;
}

float mat4x4::Determinant3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i) const {
    return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

mat4x4 mat4x4::IdentitiesMatrix()
{
    return mat4x4( 1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1
    );
}

void mat4x4::gaussJordan() {
    float* matrixPtr = data;

    int lead = 0;
    for (int r = 0; r < 4; ++r) {
        if (4 <= lead)
            break;

        int i = r;
        while (matrixPtr[i * 4 + lead] == 0) {
            ++i;
            if (4 == i) {
                i = r;
                ++lead;
                if (4 == lead)
                    return;
            }
        }

        for (int j = 0; j < 4; ++j)
            std::swap(matrixPtr[r * 4 + j], matrixPtr[i * 4 + j]);

        float val = matrixPtr[r * 4 + lead];
        for (int j = 0; j < 4; ++j)
            matrixPtr[r * 4 + j] /= val;

        for (int i = 0; i < 4; ++i) {
            if (i != r) {
                float val = matrixPtr[i * 4 + lead];
                for (int j = 0; j < 4; ++j)
                    matrixPtr[i * 4 + j] -= val * matrixPtr[r * 4 + j];
            }
        }

        ++lead;
    }
}

mat4x4 mat4x4::getCofactor() const {
    mat4x4 solution;

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            mat3x3 subMatrix;
            int p = 0, q = 0;
            for (size_t x = 0; x < 4; ++x) {
                if (x == i) continue;
                q = 0;
                for (size_t y = 0; y < 4; ++y) {
                    if (y == j) continue;
                    subMatrix.data[p * 3 + q] = data[x * 4 + y];
                    ++q;
                }
                ++p;
            }
            float cofactorSign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            solution.data[i * 4 + j] = cofactorSign * subMatrix.determinant();
        }
    }

    return solution;
}

mat4x4 mat4x4::getInverse() const {
    float det = determinant();
    if (det == 0) {
        std::cout << "Matrix is singular, cannot invert." << std::endl;
        exit(1);
    }

    mat4x4 cofactorMatrix = getCofactor();
    cofactorMatrix.TransposeMatrix(); 

    float invDet = 1.0 / det;
    mat4x4 inverseMatrix;
    for (size_t i = 0; i < 16; ++i) {
        inverseMatrix.data[i] = invDet * cofactorMatrix.data[i];
    }

    return inverseMatrix;
}

void mat4x4::GLMCompatible()
{
    TransposeMatrix();
}

void mat4x4::PerspectiveMatrix(float fovY, float aspect, float near, float far)
{
    mat4x4 perspective(
        1.f / (aspect * tanf(fovY / 2.f)), 0.f, 0.f, 0.f,
        0.f, 1.f / (tanf(fovY / 2.f)), 0.f, 0.f,
        0.f, 0.f, -1.f * (far + near) / (far - near), -1.f * (2.f * far * near) / (far - near),
        0.f, 0.f, -1.f, 0.f
    );

    for (int i = 0; i < 16; i++)
    {
        data[i] = perspective.data[i];
    }
}

void mat4x4::OrthographicMatrix(float top, float bottom, float right, float left, float far, float near)
{
    mat4x4 orthographic(
        2.f / (right - left), 0.f, 0.f, -1.f * ((right + left) / (right - left)),
        0.f, 2.f / (top - bottom), 0.f, -1.f * ((top + bottom) / (top - bottom)),
        0.f, 0.f, -2.f / (far - near), -1.f * ((far + near) / (far - near)),
        0.f, 0.f, 0.f, 1.f
    );

    for (int i = 0; i < 16; i++)
    {
        data[i] = orthographic.data[i];
    }
}

Vector4D mat4x4::operator*(const Vector4D& v) const
{
    Vector4D vR;

    vR.x = (data[0] * v.x) + (data[1] * v.y) + (data[2] * v.z) + (data[3] * v.w);
    vR.y = (data[4] * v.x) + (data[5] * v.y) + (data[6] * v.z) + (data[7] * v.w);
    vR.z = (data[8] * v.x) + (data[9] * v.y) + (data[10] * v.z) + (data[11] * v.w);
    vR.w = (data[12] * v.x) + (data[13] * v.y) + (data[14] * v.z) + (data[15] * v.w);

    return vR;
}

mat4x4 mat4x4::rotateY(float angle) {
    float radAngle = angle * M_PI / 180;

    float radians = radAngle; 
    float cosine = std::cos(radians);
    float sine = std::sin(radians);

    return mat4x4{
        cosine, 0.0f, sine, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sine, 0.0f, cosine, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

mat4x4 mat4x4::rotateX(float angle) {
    float radAngle = angle * M_PI / 180;

    float radians = radAngle;
    float cosine = std::cos(radians);
    float sine = std::sin(radians);

    return mat4x4{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosine, -sine, 0.0f,
        0.0f, sine, cosine, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Vector3D mat4x4::transformDirection(const mat4x4& matrix, const Vector3D& direction)
{
    Vector3D result;
    result.x = matrix.data[0] * direction.x + matrix.data[4] * direction.y + matrix.data[8] * direction.z;
    result.y = matrix.data[1] * direction.x + matrix.data[5] * direction.y + matrix.data[9] * direction.z;
    result.z = matrix.data[2] * direction.x + matrix.data[6] * direction.y + matrix.data[10] * direction.z;
    return result;
}
