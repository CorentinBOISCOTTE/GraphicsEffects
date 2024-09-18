#include "Math/vector.h"

Vector2D::Vector2D()
{
    x, y = 0;
}

void Vector2D::print() const {
    std::cout << "(" << x << ", " << y << ")" << std::endl;
}

Vector2D Vector2D::inverseVector() {
    return Vector2D(-x, -y);
}

Vector2D Vector2D::additionVector(Vector2D v1, Vector2D v2) {
    return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

Vector2D Vector2D::addScalar(float scalar) const {
    return Vector2D(x + scalar, y + scalar);
}

Vector2D Vector2D::multiplyVectors(Vector2D v) const {
    return Vector2D(x * v.x, y * v.y);
}

Vector2D Vector2D::multiplyByScalar(float scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator+(float scalar) const {
    return addScalar(scalar); 
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator*(float scalar) const {
    return multiplyByScalar(scalar);
}

Vector2D Vector2D::operator*(const Vector2D& other) const
{
    return multiplyVectors(other);   
}

Vector2D Vector2D::operator/(float scalar) const {
    if (scalar != 0.0) {
        return Vector2D(x / scalar, y / scalar);
    }
    else {
        std::cout << " Division by zero." << std::endl;
        return *this;
    }
}

Vector2D& Vector2D::operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2D& Vector2D::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2D& Vector2D::operator*=(const Vector2D& other)
{
    x *= other.x; 
    y *= other.y; 
    return *this; 
}

Vector2D& Vector2D::operator/=(float scalar) {
    if (scalar != 0.0) {
        x /= scalar;
        y /= scalar;
    }
    else {
        std::cout << "can't divide by zero" << std::endl;
    }
    return *this;
}

Vector2D Vector2D::midpoint(const Vector2D& other) const {
    return Vector2D((x + other.x) / 2.0, (y + other.y) / 2.0);
}

float Vector2D::distanceBetweenPoints(const Vector2D& point1, const Vector2D& point2) {
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;
    return std::sqrt(dx * dx + dy * dy);
}

float Vector2D::normSquared() const {
    return std::sqrt(x * x + y * y);
}

float Vector2D::norm() const {
    return std::sqrt(x + y);
}

float Vector2D::dotProduct(const Vector2D& v1, const Vector2D& v2) const {
    return v1.x * v2.x + v1.y * v2.y;
}

float Vector2D::angleBetween(const Vector2D& v1, const Vector2D& v2) {
    float dotProduct = v1.x * v2.x + v1.y * v2.y;
    float magnitudesProduct = v1.norm() * v2.norm();

    return std::acos(dotProduct / magnitudesProduct);
}

Vector2D Vector2D::rotate2D(float angle, const Vector2D& center) const {
    float cosTheta = std::cos(angle);
    float sinTheta = std::sin(angle);

    float translatedX = x - center.x;
    float translatedY = y - center.y;

    float rotatedX = translatedX * cosTheta - translatedY * sinTheta;
    float rotatedY = translatedX * sinTheta + translatedY * cosTheta;

    return Vector2D(rotatedX + center.x, rotatedY + center.y);
}