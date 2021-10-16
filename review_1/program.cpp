#include <iostream>
#include <vector>

constexpr int MOD = 1'000'000'007;

class SquareMatrix{
 public:
     SquareMatrix(const size_t size) {
         matrix_ = std::vector(size, std::vector<int64_t>(size, 0));
     }
     // получить ряд матрицы
     const std::vector<int64_t>& getRow(const size_t rowNumber) const;
     // получить элемент по индексам
     const int64_t getElement(const size_t rowNumber,
                              const size_t columnNumber) const;
     // назначить новое значение элементу по индексам
     void setElement(const size_t rowNumber,
                     const size_t columnNumber, const int64_t value);
     SquareMatrix operator* (const SquareMatrix& other) const;
     // возвести матрицу в степень
     SquareMatrix elevateToPower(size_t power) const;

 private:
     std::vector<std::vector<int64_t>> matrix_;
};

struct SquareMatrixPower {
    SquareMatrix matrix;
    int power;
};

const std::vector<int64_t>& SquareMatrix::getRow(const size_t rowNumber) const {
    return matrix_.at(rowNumber);
}

const int64_t SquareMatrix::getElement(const size_t rowNumber,
                                       const size_t columnNumber) const {
    return matrix_.at(rowNumber).at(columnNumber);
}

void SquareMatrix::setElement(const size_t rowNumber,
                              const size_t columnNumber, const int64_t value){
    matrix_.at(rowNumber).at(columnNumber) = value;
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
            resultMatrix.setElement(row, column, newElement);
        }
    }
    return resultMatrix;
}

SquareMatrix SquareMatrix::elevateToPower(size_t power) const {
    size_t matrixSize = matrix_.size();
    SquareMatrix thisMatrix = *this;
    SquareMatrix resultMatrix(matrixSize);
    for (size_t row = 0; row < matrixSize; ++row) {
        resultMatrix.setElement(row, row, 1);
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

SquareMatrixPower read(std::istream& in) {
    int vertices, edges, power;
    in >> vertices >> edges >> power;
    SquareMatrix resultMatrix(vertices);

    for (int edge = 0; edge < edges; ++edge) {
        int indexFirst, indexSecond;
        in >> indexFirst >> indexSecond;
        indexFirst--;
        indexSecond--;
        resultMatrix.setElement(indexFirst, indexSecond,
            resultMatrix.getElement(indexFirst, indexSecond) + 1);
    }

    return {resultMatrix, power};
}

SquareMatrix solve(const SquareMatrixPower& squareMatrixPower) {
    return squareMatrixPower.matrix.elevateToPower(squareMatrixPower.power);
}

std::ostream& write(std::ostream& out, const SquareMatrix& squareMatrix) {
    int64_t answer = 0;

    for (int64_t value : squareMatrix.getRow(0)){
        answer += (value % MOD);
    }

    out << answer % MOD << "\n";
    return out;
}
