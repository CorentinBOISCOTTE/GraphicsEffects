#include "Math/vector.h"

Vector3D::Vector3D()
{
    x, y, z = 0;
}

void Vector3D::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
}

Vector3D Vector3D::inverseVector() {
    return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::additionVector(Vector3D v1, Vector3D v2) {
    return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3D Vector3D::addScalar(float scalar) const {
    return Vector3D(x + scalar, y + scalar, z + scalar);
}

Vector3D Vector3D::multiplyVectors(Vector3D v) const {
    return Vector3D(x * v.x, y * v.y, z * v.z);
}

Vector3D Vector3D::multiplyByScalar(float scalar) const {
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

float Vector3D::norm() const {
    return std::sqrt(x + y +  z);
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator+(float scalar) const {
    return addScalar(scalar); 
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator*(float scalar) const {
    return multiplyByScalar(scalar);
}

Vector3D Vector3D::operator*(const Vector3D& other) const
{
    return multiplyVectors(other);
}

Vector3D Vector3D::operator/(float scalar) const {
    if (scalar != 0.0) {
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }
    else {
        std::cerr << " Division by zero." << std::endl;
        return *this;
    }
}

Vector3D& Vector3D::operator+=(const Vector3D& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3D& Vector3D::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3D& Vector3D::operator*=(const Vector3D& other)
{
    x *= other.x; 
    y *= other.y;
    z *= other.z;

    return *this; 
}

Vector3D& Vector3D::operator/=(float scalar) {
    if (scalar != 0.0) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
    }
    else {
        std::cerr << " Division by zero." << std::endl;
    }
    return *this;
}

Vector3D Vector3D::midpoint(const Vector3D& other) const {
    return Vector3D((x + other.x) / 2.0, (y + other.y) / 2.0, (z + other.z) / 2.0);
}

float Vector3D::distanceBetweenPoints(const Vector3D& point1, const Vector3D& point2) {
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;
    float dz = point2.z - point1.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float Vector3D::normSquared() const {
    return std::sqrt(x * x + y * y + z * z);
}

float Vector3D::dotProduct(const Vector3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::crossProduct(const Vector3D& v1, const Vector3D& v2) const {
    float resultX = v1.y * v2.z - v1.z * v2.y;
    float resultY = v1.z * v2.x - v1.x * v2.z;
    float resultZ = v1.x * v2.y - v1.y * v2.x;

    return Vector3D(resultX, resultY, resultZ);
}

float Vector3D::angleBetween(const Vector3D& v1, const Vector3D& v2) {
    float dotProduct = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    float magnitudesProduct = v1.norm() * v2.norm();

    return std::acos(dotProduct / magnitudesProduct);
}

Vector3D Vector3D::rotate3D(float angle, const Vector3D& axis, const Vector3D& center) const {
    float cosTheta = std::cos(angle);
    float sinTheta = std::sin(angle);
    float oneMinusCosTheta = 1.0 - cosTheta;

    float rotationMatrix[3][3] = {
        {cosTheta + axis.x * axis.x * oneMinusCosTheta, axis.x * axis.y * oneMinusCosTheta - axis.z * sinTheta, axis.x * axis.z * oneMinusCosTheta + axis.y * sinTheta},
        {axis.y * axis.x * oneMinusCosTheta + axis.z * sinTheta, cosTheta + axis.y * axis.y * oneMinusCosTheta, axis.y * axis.z * oneMinusCosTheta - axis.x * sinTheta},
        {axis.z * axis.x * oneMinusCosTheta - axis.y * sinTheta, axis.z * axis.y * oneMinusCosTheta + axis.x * sinTheta, cosTheta + axis.z * axis.z * oneMinusCosTheta}
    };

    float translatedX = x - center.x;
    float translatedY = y - center.y;
    float translatedZ = z - center.z;

    float rotatedX = rotationMatrix[0][0] * translatedX + rotationMatrix[0][1] * translatedY + rotationMatrix[0][2] * translatedZ;
    float rotatedY = rotationMatrix[1][0] * translatedX + rotationMatrix[1][1] * translatedY + rotationMatrix[1][2] * translatedZ;
    float rotatedZ = rotationMatrix[2][0] * translatedX + rotationMatrix[2][1] * translatedY + rotationMatrix[2][2] * translatedZ;

    return Vector3D(rotatedX + center.x, rotatedY + center.y, rotatedZ + center.z);
}

Vector3D Vector3D::normalize(const Vector3D& v) {
    
    float len = v.normSquared();

    return { v.x / len, v.y / len, v.z / len };

    return Vector3D();
}
