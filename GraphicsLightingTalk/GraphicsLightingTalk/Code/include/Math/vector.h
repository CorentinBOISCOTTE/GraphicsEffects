#pragma once

#include <iostream>
#include <vector>
#include <cmath>

class Vector2D {
public:
    float x, y;

    Vector2D();
    Vector2D(float _x, float _y) : x(_x), y(_y) {}
    
    void print() const; 
    Vector2D inverseVector(); 
    Vector2D additionVector(Vector2D v1, Vector2D v2);
    Vector2D addScalar(float scalar) const; 
    Vector2D multiplyVectors(Vector2D v) const; 
    Vector2D multiplyByScalar(float scalar) const; 
    Vector2D operator+(const Vector2D& other) const; 
    Vector2D operator+(float scalar) const; 
    Vector2D operator-(const Vector2D& other) const; 
    Vector2D operator*(float scalar) const; 
    Vector2D operator*(const Vector2D& other) const;  
    Vector2D operator/(float scalar) const; 
    Vector2D& operator+=(const Vector2D& other); 
    Vector2D& operator-=(const Vector2D& other); 
    Vector2D& operator*=(float scalar); 
    Vector2D& operator*=(const Vector2D& other);
    Vector2D& operator/=(float scalar); 
    Vector2D midpoint(const Vector2D& other) const; 
    static float distanceBetweenPoints(const Vector2D& point1, const Vector2D& point2); 
    float normSquared() const;
    float norm() const;
    float dotProduct(const Vector2D& v1, const Vector2D& v2) const;
    float angleBetween(const Vector2D& v1, const Vector2D& v2); 
    Vector2D rotate2D(float angle, const Vector2D& center) const; 
};

class Vector3D {
public:
    float x, y, z;

    Vector3D();
    Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    void print() const;
    Vector3D inverseVector();
    Vector3D additionVector(Vector3D v1, Vector3D v2);
    Vector3D addScalar(float scalar) const;
    Vector3D multiplyVectors(Vector3D v) const;
    Vector3D multiplyByScalar(float scalar) const;
    float norm() const;
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator+(float scalar) const; 
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(float scalar) const;
    Vector3D operator*(const Vector3D& other) const; 
    Vector3D operator/(float scalar) const;
    Vector3D& operator+=(const Vector3D& other);
    Vector3D& operator-=(const Vector3D& other);
    Vector3D& operator*=(float scalar);
    Vector3D& operator*=(const Vector3D& other); 
    Vector3D& operator/=(float scalar);
    Vector3D midpoint(const Vector3D& other) const;
    float distanceBetweenPoints(const Vector3D& point1, const Vector3D& point2);
    float normSquared() const;
    float dotProduct(const Vector3D& other) const;
    Vector3D crossProduct(const Vector3D& v1, const Vector3D& v2) const;
    float angleBetween(const Vector3D& v1, const Vector3D& v2);
    Vector3D rotate3D(float angle, const Vector3D& axis, const Vector3D& center) const; 
    Vector3D normalize(const Vector3D& v);
};

class Vector4D {
public:
    float x, y, z, w;

    Vector4D();
    Vector4D(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    void print() const;
    Vector4D inverseVector();
    Vector4D additionVector(Vector4D v1, Vector4D v2);
    Vector4D addScalar(float scalar) const;
    Vector4D multiplyVectors(Vector4D v) const;
    Vector4D multiplyByScalar(float scalar) const;
    float norm() const;
    Vector4D operator+(const Vector4D& other) const;
    Vector4D operator+(float scalar) const;
    Vector4D operator-(const Vector4D& other) const;
    Vector4D operator*(float scalar) const;
    Vector4D operator*(const Vector4D& other) const;
    Vector4D operator/(float scalar) const;
    Vector4D& operator+=(const Vector4D& other);
    Vector4D& operator-=(const Vector4D& other);
    Vector4D& operator*=(float scalar);
    Vector4D& operator*=(const Vector4D& other); 
    Vector4D& operator/=(float scalar);
    Vector4D midpoint(const Vector4D& other) const;
    Vector3D ToVector3D() const;
    float distanceBetweenPoints(const Vector4D& point1, const Vector4D& point2);
    float normSquared() const;
    float dotProduct(const Vector4D& other) const;
};

class VectorND {
public:
    std::vector<float> elements;

    VectorND() {}

    VectorND(std::initializer_list<float> args) : elements(args) {}

    void print() const;
    void inverseVector();
    VectorND additionVector(VectorND v1, VectorND v2);
    VectorND addScalar(float scalar) const;
    VectorND multiplyVectors(VectorND v) const;
    VectorND multiplyByScalar(float scalar) const;
    VectorND operator+(const VectorND& other) const;
    VectorND operator+(float scalar) const;
    VectorND operator-(const VectorND& other) const;
    VectorND operator*(float scalar) const; 
    VectorND operator*(const VectorND& other) const; 
    VectorND operator/(float scalar) const;
    VectorND& operator+=(const VectorND& other);
    VectorND& operator-=(const VectorND& other);
    VectorND& operator*=(float scalar);
    VectorND& operator*=(const VectorND& other);
    VectorND& operator/=(float scalar);
    VectorND midpoint(const VectorND& other) const;
    float distanceBetweenPoints(const VectorND& point1, const VectorND& point2);
    float normSquared() const;
    float norm() const;
    float dotProduct(const VectorND& other) const;
};