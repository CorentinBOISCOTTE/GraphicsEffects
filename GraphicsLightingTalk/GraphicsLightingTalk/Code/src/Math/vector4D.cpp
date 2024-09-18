#include "Math/vector.h"

Vector4D::Vector4D()
{
    x, y, z, w = 0;
}

void Vector4D::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ", " << w << ")" << std::endl;
}

Vector4D  Vector4D::inverseVector() {
    return Vector4D(-x, -y, -z, -w);
}

Vector4D  Vector4D::additionVector(Vector4D v1, Vector4D v2) {
    return Vector4D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v1.w);
}

Vector4D  Vector4D::addScalar(float scalar) const {
    return Vector4D(x + scalar, y + scalar, z + scalar, w + scalar);
}

Vector4D  Vector4D::multiplyVectors(Vector4D v) const {
    return Vector4D(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4D  Vector4D::multiplyByScalar(float scalar) const {
    return Vector4D(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vector4D  Vector4D::operator+(const Vector4D& other) const {
    return Vector4D(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4D Vector4D::operator+(float scalar) const {
    return addScalar(scalar); 
}

Vector4D  Vector4D::operator-(const Vector4D& other) const {
    return Vector4D(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4D  Vector4D::operator*(float scalar) const {
    return multiplyByScalar(scalar);
}

Vector4D Vector4D::operator*(const Vector4D& other) const
{
    return multiplyVectors(other);
}

Vector4D  Vector4D::operator/(float scalar) const {
    if (scalar != 0.0) {
        return Vector4D(x / scalar, y / scalar, z / scalar, w / scalar);
    }
    else {
        std::cerr << " Division by zero." << std::endl;
        return *this;
    }
}

Vector4D& Vector4D::operator+=(const Vector4D& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Vector4D& Vector4D::operator-=(const Vector4D& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

Vector4D& Vector4D::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

Vector4D& Vector4D::operator*=(const Vector4D& other)
{
    x *= other.x; 
    y *= other.y; 
    z *= other.z; 
    w *= other.w; 
    return *this; 
}

Vector4D& Vector4D::operator/=(float scalar) {
    if (scalar != 0.0) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
    }
    else {
        std::cerr << " Division by zero." << std::endl;
    }
    return *this;
}

float  Vector4D::norm() const {
    return std::sqrt(x + y + z + w);
}

Vector4D  Vector4D::midpoint(const Vector4D& other) const {
    return Vector4D((x + other.x) / 2.0, (y + other.y) / 2.0, (z + other.z) / 2.0, (w + other.w) / 2.0);
}

Vector3D Vector4D::ToVector3D() const
{
    Vector3D v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

float Vector4D::distanceBetweenPoints(const Vector4D& point1, const Vector4D& point2) {
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;
    float dz = point2.z - point1.z;
    float dw = point2.w - point1.w;
    return std::sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

float  Vector4D::normSquared() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

float  Vector4D::dotProduct(const Vector4D& other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}