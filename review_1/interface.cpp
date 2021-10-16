#include <iostream>
#include <vector>

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

struct VectorPower {
    std::vector<std::vector<int64_t>> base;
    int power;
};

VectorPower readMatrixPower(std::istream& in);
int64_t getFirstRowSum(const VectorPower& vectorPower);
std::ostream& writeFirstRowSum(std::ostream& out, const int64_t result);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    const VectorPower squareMatrixPower = readMatrixPower(std::cin);
    const int64_t answer = getFirstRowSum(squareMatrixPower);
    writeFirstRowSum(std::cout, answer);

    return 0;
}
