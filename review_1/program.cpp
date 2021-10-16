#include <iostream>
#include <vector>

constexpr int MOD = 1'000'000'007;

class SquareMatrix{
 public:
     SquareMatrix(const size_t size);
     SquareMatrix(const std::vector<std::vector<int64_t>>& matrix);

     SquareMatrix operator* (const SquareMatrix& other) const;
     SquareMatrix elevateToPower(size_t power) const;

     friend std::ostream& write(std::ostream& out, const SquareMatrix& matrix);

 private:
     std::vector<std::vector<int64_t>> matrix_;
};

struct VectorPower {
    std::vector<std::vector<int64_t>> base;
    int power;
};

SquareMatrix::SquareMatrix(const size_t size) {
    matrix_ = std::vector<std::vector<int64_t>>(size,
        std::vector<int64_t>(size, 0));
}

SquareMatrix::SquareMatrix(const std::vector<std::vector<int64_t>>& matrix) {
    matrix_ = matrix;
}

SquareMatrix SquareMatrix::operator* (const SquareMatrix& other) const {
    size_t matrixSize = matrix_.size();
    size_t otherMatrixSize = other.matrix_.size();

    if (matrixSize != otherMatrixSize) {
        throw std::invalid_argument("matrices sizes mismatch");
    }

    SquareMatrix resultMatrix(matrixSize);

    for (size_t row = 0; row < matrixSize; ++row) {
        for (size_t column = 0; column < matrixSize; ++column) {
            int64_t newElement = 0;
            for (size_t k = 0; k < matrixSize; ++k) {
                newElement = (newElement + (matrix_[row][k] *
                    other.matrix_[k][column]) % MOD) % MOD;
            }
            resultMatrix.matrix_[row][column] = newElement;
        }
    }
    return resultMatrix;
}

SquareMatrix SquareMatrix::elevateToPower(size_t power) const {
    size_t matrixSize = matrix_.size();
    SquareMatrix thisMatrix = *this;
    SquareMatrix resultMatrix(matrixSize);
    for (size_t row = 0; row < matrixSize; ++row) {
        resultMatrix.matrix_[row][row] = 1;
    }

    while (power > 0) {
        if (power % 2 == 1) {
            resultMatrix = thisMatrix * resultMatrix;
        }
        thisMatrix = thisMatrix * thisMatrix;
        power /= 2;
    }
    return resultMatrix;
}

VectorPower read(std::istream& in) {
    int vertices, edges, power;
    in >> vertices >> edges >> power;
    std::vector<std::vector<int64_t>> base (vertices,
        std::vector<int64_t>(vertices, 0));

    for (int edge = 0; edge < edges; ++edge) {
        int indexFirst, indexSecond;
        in >> indexFirst >> indexSecond;
        indexFirst--;
        indexSecond--;
        base[indexFirst][indexSecond]++;
    }

    return {base, power};
}

SquareMatrix solve(const VectorPower& vectorPower) {
    SquareMatrix matrix(vectorPower.base);
    return matrix.elevateToPower(vectorPower.power);
}

std::ostream& write(std::ostream& out, const SquareMatrix& matrix) {
    int64_t answer = 0;

    for (int64_t value : matrix.matrix_[0]) {
        answer += (value % MOD);
    }

    out << answer % MOD << "\n";
    return out;
}
