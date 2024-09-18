#pragma once
#define _USE_MATH_DEFINES

#include "vector.h"

#include <vector>
#include <iostream>
#include <math.h>

class mat2x2 {
public:
    float data[4]; 

    mat2x2() {
        data[0] = 0;  data[1] = 0;
        data[2] = 0;  data[3] = 0;
    };

    mat2x2(float e00, float e01, 
           float e10, float e11)
    {
        data[0] = e00;  data[1] = e01;  
        data[2] = e10;  data[3] = e11;  
    }
    void initMatrixWithRowVectors(Vector2D row[]);
    void printMatrix();
    float TraceMatrix() const;
    void DiagonalMatrix();
    void OppositeMatrix();
    void TransposeMatrix();
    void AddMatrices(const mat2x2& other);
    void MultiplyByScalar(float scalar);
    void MultiplyMatrices(const mat2x2& other);
    float determinant() const;
    mat2x2 IdentitiesMatrix();
    void gaussJordan();
    mat2x2 inverse() const;
    void GLMCompatible();

    mat2x2 rotationX(float angle); 
    mat2x2 rotationY(float angle);
    mat2x2 rotationZ(float angle);
    mat2x2 rotationXYZ(const mat2x2& m1, const mat2x2& m2, const mat2x2& m3); 

    mat2x2 operator*(const mat2x2& other) const; 
    Vector2D operator*(const Vector2D& v) const;
};

class mat3x3 {
public:
    float data[9];

    mat3x3() {
        data[0] = 0;  data[1] = 0;  data[2] = 0;
        data[3] = 0;  data[4] = 0;  data[5] = 0;
        data[6] = 0;  data[7] = 0;  data[8] = 0;
    };

    mat3x3(float e00, float e01, float e02,
           float e10, float e11, float e12,
           float e20, float e21, float e22)
    {
        data[0] = e00;  data[1] = e01;  data[2] = e02; 
        data[3] = e10;  data[4] = e11;  data[5] = e12; 
        data[6] = e20;  data[7] = e21;  data[8] = e22; 
    }

    void initMatrixWithRowVectors(Vector3D row[3]);
    void printMatrix();
    float TraceMatrix() const;
    void DiagonalMatrix();
    void OppositeMatrix();
    void TransposeMatrix();
    void AddMatrices(const mat3x3& other);
    void MultiplyByScalar(float scalar);
    void MultiplyMatrices(const mat3x3& other);
    float determinant() const;
    mat3x3 IdentitiesMatrix();
    void gaussJordan();
    mat3x3 inverse() const; 
    void GLMCompatible();

    mat3x3 rotationX(float angle);
    mat3x3 rotationY(float angle);
    mat3x3 rotationZ(float angle);
    mat3x3 rotationXYZ(const mat3x3& m1, const mat3x3& m2, const mat3x3& m3);
    mat3x3 rotationAroundAxis(const Vector3D& axis, float angle);

    mat3x3 operator*(const mat3x3& other) const;
    Vector3D operator*(const Vector3D& v) const;
};

class mat4x4 {
public:
    float data[16];

    mat4x4() {
        data[0] = 0;  data[1] = 0;  data[2] = 0;  data[3] = 0;
        data[4] = 0;  data[5] = 0;  data[6] = 0;  data[7] = 0;
        data[8] = 0;  data[9] = 0;  data[10] = 0; data[11] = 0;
        data[12] = 0; data[13] = 0; data[14] = 0; data[15] = 0;
    };

    mat4x4(float e00, float e01, float e02, float e03,
        float e10, float e11, float e12, float e13,
        float e20, float e21, float e22, float e23,
        float e30, float e31, float e32, float e33) 
    {
        data[0] = e00;  data[1] = e01;  data[2] = e02;  data[3] = e03;
        data[4] = e10;  data[5] = e11;  data[6] = e12;  data[7] = e13;
        data[8] = e20;  data[9] = e21;  data[10] = e22; data[11] = e23;
        data[12] = e30; data[13] = e31; data[14] = e32; data[15] = e33;
    }

    void initMatrixWithRowVectors(Vector4D row[4]);
    void printMatrix();
    float TraceMatrix() const;
    void DiagonalMatrix();
    void OppositeMatrix();
    void TransposeMatrix();
    void AddMatrices(const mat4x4& other);
    void MultiplyByScalar(float scalar);
    void MultiplyMatrices(const mat4x4& other);
    float determinant() const;
    float Determinant3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i) const; 
    mat4x4 IdentitiesMatrix();
    void gaussJordan();
    mat4x4 getCofactor() const;
    mat4x4 getInverse() const;
    void GLMCompatible();

    //TRS
    mat4x4 GetTranslationMatrix(Vector3D position);
    mat4x4 GetRotationMatrix(Vector3D anglesDeg);
    mat4x4 GetScaleMatrix(Vector3D scale);
    mat4x4 Get_TRS_Matrix(Vector3D position, Vector3D rotationAngles, Vector3D scale);
    void PerspectiveMatrix(float fovY, float aspect, float near, float far);
    void OrthographicMatrix(float top, float bottom, float right, float left, float near, float far);

    mat4x4 operator*(const mat4x4& other) const;
    Vector4D operator*(const Vector4D& v) const;
    

    //Mouse Movement
    Vector3D transformDirection(const mat4x4& matrix, const Vector3D& direction);
    mat4x4 rotateY(float angle); 
    mat4x4 rotateX(float angle);
};

class matMxN {
private:
    std::vector<std::vector<float>> matrix;

public:
    matMxN() {}

    void initIndividualMatrix(int rows, int columns, std::vector<float> elements);
    void initMatrixWithRowVectors(std::vector<std::vector<float>> rowVectors);
    void printMatrix() const;
    float TraceMatrix() const;
    void OppositeMatrix();
    void DiagonalMatrix();
    void TransposeMatrix();
    matMxN AddMatrices(const matMxN& other) const;
    void MultiplyByScalar(float scalar);
    matMxN MultiplyMatrices(const matMxN& other) const;
    float determinant() const;
    float subDeterminant(const std::vector<std::vector<float>>& subMatrix) const;
    matMxN IdentitiesMatrix(int size);
    matMxN gaussJordan();
    matMxN getCofactor(const matMxN& other);
    matMxN getInverse(const matMxN& other);
    void GLMCompatible();
};