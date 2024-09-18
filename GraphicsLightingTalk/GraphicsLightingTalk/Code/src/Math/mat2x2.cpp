#include "Math/matrice.h"

void mat2x2::initMatrixWithRowVectors(Vector2D row[]) {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (j == 0) {
                data[i * 2 + j] = row[i].x;
            }
            else {
                data[i * 2 + j] = row[i].y;
            }
        }
    }
}

void  mat2x2::printMatrix() {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            std::cout << data[i * 2 + j] << " ";
        }
        std::cout << std::endl;
    }
}
float mat2x2::TraceMatrix() const {
    float result = 0;
    for (int i = 0; i < 2; ++i) {
        result += data[i * 2 + i];
    }
    return result;
}
void mat2x2::DiagonalMatrix() {
    float result = 2;
    for (int i = 0; i < 2; ++i) {
        std::cout << data[i * 2 + i] << " ";
    }
    std::cout << std::endl;
}
void mat2x2::OppositeMatrix() {
    for (int i = 0; i < 4; ++i) {
        data[i] = -data[i];
    }
}
void mat2x2::TransposeMatrix() {
    float temp;
    for (int i = 0; i < 2; ++i) {
        for (int j = i + 1; j < 2; ++j) {
            temp = data[i * 2 + j];
            data[i * 2 + j] = data[j * 2 + i];
            data[j * 2 + i] = temp;
        }
    }
}
void mat2x2::AddMatrices(const mat2x2& other) {
    for (int i = 0; i < 4; ++i) {
        data[i] += other.data[i];
    }
}
void mat2x2::MultiplyByScalar(float scalar) {
    for (int i = 0; i < 4; ++i) {
        data[i] *= scalar;
    }
}
void mat2x2::MultiplyMatrices(const mat2x2& other) {
    float result[4];

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            float sum = 0;
            for (int k = 0; k < 2; ++k) {
                sum += data[i * 2 + k] * other.data[k * 2 + j];
            }
            result[i * 2 + j] = sum;
        }
    }

    for (int i = 0; i < 4; ++i) {
        data[i] = result[i];
    }
}

mat2x2 mat2x2::IdentitiesMatrix() {
    return mat2x2(1, 0,
                  0, 1
    );
}

void mat2x2::gaussJordan() {
    float* matrixPtr = data;

    int lead = 0;
    for (int r = 0; r < 2; ++r) {
        if (3 <= lead)
            break;

        int i = r;
        while (matrixPtr[i * 2 + lead] == 0) {
            ++i;
            if (2 == i) {
                i = r;
                ++lead;
                if (2 == lead)
                    return;
            }
        }

        for (int j = 0; j < 2; ++j)
            std::swap(matrixPtr[r * 2 + j], matrixPtr[i * 2 + j]);

        float val = matrixPtr[r * 2 + lead];
        for (int j = 0; j < 2; ++j)
            matrixPtr[r * 2 + j] /= val;

        for (int i = 0; i < 2; ++i) {
            if (i != r) {
                float val = matrixPtr[i * 2 + lead];
                for (int j = 0; j < 2; ++j)
                    matrixPtr[i * 2 + j] -= val * matrixPtr[r * 2 + j];
            }
        }

        ++lead;
    }
}

mat2x2 mat2x2::inverse() const {
    float det = determinant();
    if (det == 0) {
        return mat2x2();
    }

    float invDet = 1.0 / det;
    mat2x2 invMat;
    invMat.data[0] = data[3] * invDet;
    invMat.data[1] = -data[1] * invDet;
    invMat.data[2] = -data[2] * invDet;
    invMat.data[3] = data[0] * invDet;

    return invMat;
}

void mat2x2::GLMCompatible()
{
    TransposeMatrix();
}

mat2x2 mat2x2::operator*(const mat2x2& other) const
{
    mat2x2 result;
    result.data[0] = data[0] * other.data[0] + data[1] * other.data[2];
    result.data[1] = data[0] * other.data[1] + data[1] * other.data[3];
    result.data[2] = data[2] * other.data[0] + data[3] * other.data[2];
    result.data[3] = data[2] * other.data[1] + data[3] * other.data[3];
    return result;
}

float mat2x2::determinant() const {
    float det = 0;

    det = data[0] * data[3] - data[2] * data[1]; 

    return det;
}

mat2x2 mat2x2::rotationX(float angle) {
    float cosA = std::cos(angle);

    return mat2x2(1, 0,
        0, cosA);
}

mat2x2 mat2x2::rotationY(float angle) {
    float cosA = std::cos(angle);

    return mat2x2(cosA, 0,
        0, 1);
}

mat2x2 mat2x2::rotationZ(float angle) {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    return mat2x2(cosA, -sinA,
        sinA, cosA);
}

mat2x2 mat2x2::rotationXYZ(const mat2x2& m1, const mat2x2& m2, const mat2x2& m3)
{
    mat2x2 result; 
    result = m1 * m2 * m3; 
    return result;
}


Vector2D mat2x2::operator*(const Vector2D& v) const
{
    Vector2D vR;

    vR.x = (data[0] * v.x) * (data[1] * v.y);
    vR.x = (data[2] * v.x) * (data[3] * v.y);

    return vR;
}