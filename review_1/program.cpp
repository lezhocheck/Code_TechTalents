#include <iostream>
#include <vector>

const int MODULUS = 1'000'000'007;
const int SHIFT = -1;

class SquareMatrix{
 public:
     explicit SquareMatrix(const size_t size);
     explicit SquareMatrix(const std::vector<std::vector<int64_t>>& matrix);

     SquareMatrix operator* (const SquareMatrix& secondMatrix) const;
     SquareMatrix elevateToPower(size_t power) const;

    const std::vector<int64_t>& getRow(const size_t rowNumber) const;

 private:
     std::vector<std::vector<int64_t>> matrix_;
};

struct LabyrinthData {
    std::vector<std::vector<int64_t>> labyrinth;
    int pathLength;
};

const std::vector<int64_t>& SquareMatrix::getRow(const size_t rowNumber) const {
    if(rowNumber >= matrix_.size()){
        throw std::out_of_range("Row was out of range");
    }

    return matrix_.at(rowNumber);
}

SquareMatrix::SquareMatrix(const size_t size) {
    matrix_ = std::vector<std::vector<int64_t>>(size,
        std::vector<int64_t>(size, 0));
}

SquareMatrix::SquareMatrix(const std::vector<std::vector<int64_t>>& matrix) {
    matrix_ = matrix;
}

SquareMatrix SquareMatrix::operator* (const SquareMatrix& secondMatrix) const {
    size_t firstMatrixSize = matrix_.size();
    size_t secondMatrixSize = secondMatrix.matrix_.size();

    if (firstMatrixSize != secondMatrixSize) {
        throw std::invalid_argument("matrices sizes mismatch");
    }

    SquareMatrix resultMatrix(firstMatrixSize);

    for (size_t row = 0; row < firstMatrixSize; ++row) {
        for (size_t column = 0; column < firstMatrixSize; ++column) {
            int64_t newElement = 0;
            for (size_t k = 0; k < firstMatrixSize; ++k) {
                newElement = (newElement + (matrix_[row][k] *
                    secondMatrix.matrix_[k][column]) % MODULUS) % MODULUS;
            }
            resultMatrix.matrix_[row][column] = newElement;
        }
    }
    return resultMatrix;
}

SquareMatrix SquareMatrix::elevateToPower(size_t power) const {
    size_t matrixSize = matrix_.size();
    SquareMatrix matrix = *this;
    SquareMatrix resultMatrix(matrixSize);
    for (size_t row = 0; row < matrixSize; ++row) {
        resultMatrix.matrix_[row][row] = 1;
    }

    while (power > 0) {
        if (power % 2 == 1) {
            resultMatrix = matrix * resultMatrix;
        }
        matrix = matrix * matrix;
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
        indexFirst += SHIFT;
        indexSecond += SHIFT;
        base[indexFirst][indexSecond]++;
    }

    return {base, power};
}

int64_t getNumberOfPaths(const LabyrinthData& labyrinthData) {
    SquareMatrix matrix(labyrinthData.labyrinth);
    SquareMatrix powered = matrix.elevateToPower(labyrinthData.pathLength);
    int64_t numberOfPaths = 0;
    std::vector<int64_t> firstRow = powered.getRow(0);
    for (int64_t value : firstRow) {
        numberOfPaths += (value % MODULUS);
    }

    return (numberOfPaths % MODULUS);
}

std::ostream& writeNumberOfPaths(std::ostream& out, const int64_t value) {
    out << value << "\n";
    return out;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    const LabyrinthData squareMatrixPower = readLabyrinthData(std::cin);
    const int64_t numberOfPaths = getNumberOfPaths(squareMatrixPower);
    writeNumberOfPaths(std::cout, numberOfPaths);

    return 0;
}
