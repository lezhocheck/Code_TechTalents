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

LabyrinthData readMatrixPower(std::istream& in);
int64_t getFirstRowSum(const LabyrinthData& vectorPower);
std::ostream& writeFirstRowSum(std::ostream& out, const int64_t firstRowSum);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    const LabyrinthData squareMatrixPower = readMatrixPower(std::cin);
    const int64_t firstRowSum = getFirstRowSum(squareMatrixPower);
    writeFirstRowSum(std::cout, firstRowSum);

    return 0;
}
