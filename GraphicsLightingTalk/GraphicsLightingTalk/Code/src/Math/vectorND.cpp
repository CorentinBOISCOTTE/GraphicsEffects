#include "Math/vector.h"

void VectorND::print() const {
    std::cout << "(";
    for (const auto& element : elements) {
        std::cout << element << ", ";
    }
    std::cout << ")" << std::endl;
}

void VectorND::inverseVector() {
    for (auto& element : elements) {
        element = -element;
    }
}

VectorND VectorND::additionVector(VectorND v1, VectorND v2) {
    if (v1.elements.size() != v2.elements.size()) {
        throw std::invalid_argument("Vectors must have the same dimension.");
    }

    VectorND result;
    for (size_t i = 0; i < v1.elements.size(); ++i) {
        result.elements.push_back(v1.elements[i] + v2.elements[i]);
    }
    return result;
}

VectorND VectorND::addScalar(float scalar) const {
    VectorND result;
    for (size_t i = 0; i < elements.size(); ++i) {
        result.elements.push_back(elements[i] + scalar);
    }
    return result;
}

VectorND  VectorND::multiplyVectors(VectorND other) const {
    if (elements.size() != other.elements.size()) {
        throw std::invalid_argument("Vectors must have the same dimension.");
    }

    VectorND result;
    for (std::size_t i = 0; i < elements.size(); ++i) {
        result.elements.push_back(elements[i] * other.elements[i]);
    }
    return result;
}

VectorND VectorND::multiplyByScalar(float scalar) const {
    VectorND result = *this; 
    for (auto& element : result.elements) {
        element *= scalar;
    }
    return result;
}

VectorND  VectorND::operator+(const VectorND& other) const {
    VectorND result;
    if (elements.size() == other.elements.size()) {
        for (size_t i = 0; i < elements.size(); ++i) {
            result.elements.push_back(elements[i] + other.elements[i]);
        }
    }
    else {
        std::cerr << "Cannot add vectors with different dimensions." << std::endl;
    }
    return result;
}

VectorND VectorND::operator+(float scalar) const {
    return addScalar(scalar); 
}


VectorND  VectorND::operator-(const VectorND& other) const {
    VectorND result;
    if (elements.size() == other.elements.size()) {
        for (size_t i = 0; i < elements.size(); ++i) {
            result.elements.push_back(elements[i] - other.elements[i]);
        }
    }
    else {
        std::cerr << " Cannot subtract vectors with different dimensions." << std::endl;
    }
    return result;
}

VectorND  VectorND::operator*(const VectorND& other) const {
    VectorND result;
    if (elements.size() == other.elements.size()) {
        for (size_t i = 0; i < elements.size(); ++i) {
            result.elements.push_back(elements[i] * other.elements[i]);
        }
    }
    else {
        std::cerr << " Cannot multiply vectors with different dimensions." << std::endl;
    }
    return result;
}

VectorND  VectorND::operator/(float scalar) const {
    if (scalar != 0.0) {
        VectorND result;
        for (const auto& element : elements) {
            result.elements.push_back(element / scalar);
        }
        return result;
    }
    else {
        std::cerr << " Division by zero." << std::endl;
        return *this;
    }
}

VectorND& VectorND::operator+=(const VectorND& other) {
    if (elements.size() != other.elements.size()) {
        throw std::invalid_argument("Les vecteurs n'ont pas la même taille.");
    }

    for (size_t i = 0; i < elements.size(); ++i) {
        elements[i] += other.elements[i];
    }

    return *this;
}

VectorND& VectorND::operator-=(const VectorND& other) {
    if (elements.size() != other.elements.size()) {
        throw std::invalid_argument("Les vecteurs n'ont pas la même taille.");
    }

    for (size_t i = 0; i < elements.size(); ++i) {
        elements[i] -= other.elements[i];
    }

    return *this;
}

VectorND& VectorND::operator*=(float scalar) {
    for (auto& element : elements) {
        element *= scalar;
    }
    return *this;
}

VectorND& VectorND::operator*=(const VectorND& other) {
    if (elements.size() != other.elements.size()) {
        throw std::invalid_argument("Les vecteurs n'ont pas la même taille.");
    }

    for (size_t i = 0; i < elements.size(); ++i) {
        elements[i] *= other.elements[i];
    }

    return *this;
}

VectorND& VectorND::operator/=(float scalar) {
    if (scalar != 0.0) {
        for (auto& element : elements) {
            element /= scalar;
        }
    }
    return *this;
}

VectorND VectorND::operator*(float scalar) const
{
    return multiplyByScalar(scalar);
}

VectorND  VectorND::midpoint(const VectorND& other) const {
    VectorND result;
    if (elements.size() == other.elements.size()) {
        for (size_t i = 0; i < elements.size(); ++i) {
            result.elements.push_back((elements[i] + other.elements[i]) / 2.0);
        }
    }
    else {
        std::cerr << "Vectors Don't have the same size." << std::endl;
    }
    return result;
}

float  VectorND::distanceBetweenPoints(const VectorND& point1, const VectorND& point2) {
    float result = 0.0;
    if (point1.elements.size() == point2.elements.size()) {
        for (size_t i = 0; i < point1.elements.size(); ++i) {
            float tempValue = point1.elements[i] - point2.elements[i];
            result += tempValue * tempValue;
        }
        result = std::sqrt(result);
    }
    else {
        std::cerr << "Vectors don't have the same size." << std::endl;
    }

    return result;
}

float  VectorND::normSquared() const {
    float result = 0.0;
    for (const auto& element : elements) {
        float tempValue = element * element;
        result += tempValue;
    }
    return result;
}


float  VectorND::norm() const {
    float sum = 0.0;
    for (const auto& element : elements) {
        sum += element;
    }
    return std::sqrt(sum);
}

float VectorND::dotProduct(const VectorND& other) const {
    float result = 0.0;
    if (elements.size() == other.elements.size()) {
        for (size_t i = 0; i < elements.size(); ++i) {
            result += elements[i] * other.elements[i];
        }
    }
    else {
        std::cerr << "Vectors Don't have the same size." << std::endl;
    }
    return result;
}