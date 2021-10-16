#include <iostream>
#include <vector>

const int MODULUS = 1'000'000'007;

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

LabyrinthData readLabyrinthData(std::istream& in);
int64_t getNumberOfPaths(const LabyrinthData& labyrinthData);
std::ostream& writeNumberOfPaths(std::ostream& out, const int64_t value);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    const LabyrinthData labyrinthData = readLabyrinthData(std::cin);
    const int64_t numberOfPaths = getNumberOfPaths(labyrinthData);
    writeNumberOfPaths(std::cout, numberOfPaths);

    return 0;
}
