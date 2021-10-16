#include <iostream>
#include <vector>

const int MODULUS = 1'000'000'007;

class SquareMatrix{
 public:
     explicit SquareMatrix(const size_t size);
     explicit SquareMatrix(const std::vector<std::vector<int64_t>>& matrix);

     SquareMatrix operator* (const SquareMatrix& other) const;
     SquareMatrix elevateToPower(size_t power) const;

    const std::vector<int64_t>& getRow(const size_t rowNumber) const;

 private:
     std::vector<std::vector<int64_t>> matrix_;
};

struct LabyrinthData {
    std::vector<std::vector<int64_t>> base;
    int power;
};

const std::vector<int64_t>& SquareMatrix::getRow(const size_t rowNumber) const{
    return matrix_.at(rowNumber);
}

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
                    other.matrix_[k][column]) % MODULUS) % MODULUS;
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

LabyrinthData readLabyrinthData(std::istream& in) {
    int vertices, edges, power;
    in >> vertices >> edges >> power;
    std::vector<std::vector<int64_t>> base(vertices,
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

int64_t getNumberOfPathsFromStart(const LabyrinthData& vectorPower) {
    SquareMatrix matrix(vectorPower.base);
    SquareMatrix powered = matrix.elevateToPower(vectorPower.power);
    int64_t answer = 0;

    for (int64_t value : powered.getRow(0)) {
        answer += (value % MODULUS);
    }

    return (answer % MODULUS);
}

std::ostream& writeNumberOfPathsFromStart(std::ostream& out, const int64_t value) {
    out << value << "\n";
    return out;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    const LabyrinthData squareMatrixPower = readLabyrinthData(std::cin);
    const int64_t firstRowSum = getNumberOfPathsFromStart(squareMatrixPower);
    writeNumberOfPathsFromStart(std::cout, firstRowSum);

    return 0;
}
