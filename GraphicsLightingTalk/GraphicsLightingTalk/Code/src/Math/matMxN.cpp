#include "Math/matrice.h"

void matMxN::initIndividualMatrix(int rows, int columns, std::vector<float> elements) {
    matrix.resize(rows, std::vector<float>(columns));

    if (elements.size() != rows * columns) {
        std::cerr << "Incorrect number of elements for matrix size." << std::endl;
        return;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            matrix[i][j] = elements[i * columns + j];
        }
    }
}

void matMxN::initMatrixWithRowVectors(std::vector<std::vector<float>> rowVectors) {
    matrix = rowVectors;
}

void matMxN::printMatrix() const {
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            std::cout << matrix[i][j] << " "; 
        }
        std::cout << std::endl;
    }
}


float matMxN::TraceMatrix() const {
    if (matrix.size() != matrix[0].size()) {
        std::cerr << "The matrix is not square." << std::endl;
        return 0;
    }

    int trace = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
        trace += matrix[i][i];
    }

    return trace;
}


void matMxN::OppositeMatrix() {
    for (auto& row : matrix) {
        for (auto& value : row) {
            value = -value;
        }
    }
}

void matMxN::DiagonalMatrix() {
    if (matrix.size() != matrix[0].size()) {
        std::cerr << "The matrix is not square." << std::endl;
        return;
    }

    size_t size = matrix.size();

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (i == j) {
                std::cout << matrix[i][j] << " ";
            }
        }
    }
    std::cout << std::endl;
}

void matMxN::TransposeMatrix() {
    std::vector<std::vector<float>> transposed(matrix[0].size(), std::vector<float>(matrix.size()));
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[0].size(); ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }

    matrix = transposed;
}

matMxN matMxN::AddMatrices(const matMxN& other) const {
    if (matrix.size() != other.matrix.size() || matrix[0].size() != other.matrix[0].size()) {
        std::cerr << "Matrix dimensions do not match." << std::endl;
        return matMxN();
    }

    matMxN sum;
    sum.matrix.resize(matrix.size(), std::vector<float>(matrix[0].size()));

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[0].size(); ++j) {
            sum.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
        }
    }

    return sum;
}

void matMxN::MultiplyByScalar(float scalar) {
    for (auto& row : matrix) {
        for (auto& value : row) {
            value *= scalar;
        }
    }
}

matMxN matMxN::MultiplyMatrices(const matMxN& other) const {
    if (matrix[0].size() != other.matrix.size()) {
        std::cerr << "Matrix dimensions do not match for multiplication." << std::endl;
        return matMxN();
    }

    matMxN product;
    product.matrix.resize(matrix.size(), std::vector<float>(other.matrix[0].size(), 0));

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < other.matrix[0].size(); ++j) {
            for (size_t k = 0; k < matrix[0].size(); ++k) {
                product.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
            }
        }
    }

    return product;
}

float matMxN::determinant() const {
    int size = static_cast<int>(matrix.size());
    float det = 0;

    if (size == 1) {
        return matrix[0][0];
    }
    else {
        int sign = 1;
        for (int i = 0; i < size; ++i) {
            std::vector<std::vector<float>> subMatrix(size - 1, std::vector<float>(size - 1));
            for (int j = 1; j < size; ++j) {
                for (int k = 0; k < size; ++k) {
                    if (k < i) {
                        subMatrix[j - 1][k] = matrix[j][k];
                    }
                    else if (k > i) {
                        subMatrix[j - 1][k - 1] = matrix[j][k];
                    }
                }
            }
            det += sign * matrix[0][i] * subDeterminant(subMatrix);
            sign = -sign;
        }
    }
    return det;
}

float matMxN::subDeterminant(const std::vector<std::vector<float>>& subMatrix) const {
    int size = static_cast<int>(subMatrix.size());
    float det = 0;

    if (size == 1) {
        return subMatrix[0][0];
    }
    else {
        int sign = 1;
        for (int i = 0; i < size; ++i) {
            std::vector<std::vector<float>> subSubMatrix(size - 1, std::vector<float>(size - 1));
            for (int j = 1; j < size; ++j) {
                for (int k = 0; k < size; ++k) {
                    if (k < i) {
                        subSubMatrix[j - 1][k] = subMatrix[j][k];
                    }
                    else if (k > i) {
                        subSubMatrix[j - 1][k - 1] = subMatrix[j][k];
                    }
                }
            }
            det += sign * subMatrix[0][i] * subDeterminant(subSubMatrix);
            sign = -sign;
        }
    }
    return det;
}

matMxN matMxN::IdentitiesMatrix(int size) {
    matMxN product;
    product.matrix.resize(size, std::vector<float>(size, 0));

    for (int i = 0; i < size; ++i) {
        product.matrix[i][i] = 1;
    }

    return product;
}
 
matMxN matMxN::gaussJordan() {
    matMxN result;
    int rows = static_cast<int>(matrix.size());
    if (rows == 0) {
        return result;
    }
    int cols = static_cast<int>(matrix[0].size());
    result.matrix = matrix;

    int lead = 0;
    for (int r = 0; r < rows; ++r) {
        if (cols <= lead)
            break;

        int i = r;
        while (result.matrix[i][lead] == 0) {
            ++i;
            if (rows == i) {
                i = r;
                ++lead;
                if (cols == lead)
                    return result; 
            }
        }

        std::swap(result.matrix[i], result.matrix[r]);

        float val = result.matrix[r][lead];
        for (int j = 0; j < cols; ++j)
            result.matrix[r][j] /= val;

        for (int i = 0; i < rows; ++i) {
            if (i != r) {
                float val = result.matrix[i][lead];
                for (int j = 0; j < cols; ++j)
                    result.matrix[i][j] -= val * result.matrix[r][j];
            }
        }

        ++lead;
    }

    return result;
}

matMxN matMxN::getCofactor(const matMxN& other) {

    if (other.matrix.size() != other.matrix[0].size()) {
        std::cout << "can't be inversed" << std::endl;
        exit(1); 
    }

    matMxN solution;
    solution.matrix.resize(other.matrix.size(), std::vector<float>(other.matrix[0].size(), 0));

    matMxN subVect; 
    subVect.matrix.resize(other.matrix.size() - 1, std::vector<float>(other.matrix[0].size() - 1));

    for (std::size_t i = 0; i < other.matrix.size(); i++) {
        for (std::size_t j = 0; j < other.matrix[0].size(); j++) {

            int p = 0;
            for (size_t x = 0; x < other.matrix.size(); x++) {
                if (x == i) {
                    continue;
                }
                int q = 0;

                for (size_t y = 0; y < other.matrix.size(); y++) {
                    if (y == j) {
                        continue;
                    }

                    subVect.matrix[p][q] = other.matrix[x][y];
                    q++;
                }
                p++;
            }
            solution.matrix[i][j] = pow(-1, i + j) * subVect.determinant();
        }
    }
    return solution;
}

matMxN matMxN::getInverse(const matMxN& other) {
    if (other.determinant() == 0) {
        std::cout << "can't be inversed" << std::endl; 
        exit(1);
    }

    float d = 1.0 / other.determinant();

    std::cout << d << std::endl; 
    matMxN solution; 
    solution.matrix.resize(other.matrix.size(), std::vector<float>(other.matrix[0].size(), 0));

    for (size_t i = 0; i < other.matrix.size(); i++) {
        for (size_t j = 0; j < other.matrix.size(); j++) {
            solution.matrix[i][j] = other.matrix[i][j];
        }
    }

    solution = getCofactor(solution);
    solution.TransposeMatrix();

    for (size_t i = 0; i < other.matrix.size(); i++) {
        for (size_t j = 0; j < other.matrix.size(); j++) {
            solution.matrix[i][j] *= d;
        }
    }

    return solution; 
}

void matMxN::GLMCompatible()
{
    TransposeMatrix();
}
