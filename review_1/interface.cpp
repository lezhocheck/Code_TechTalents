#include <iostream>
#include <vector>

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

VectorPower read(std::istream& in);
SquareMatrix solve(const VectorPower& vectorPower);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    const VectorPower squareMatrixPower = read(std::cin);
    const SquareMatrix resultMatrix = solve(squareMatrixPower);
    write(std::cout, resultMatrix);

    return 0;
}

