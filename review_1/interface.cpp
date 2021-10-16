#include <iostream>
#include <vector>

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
    // назначить элемент по индексам
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

// инициализация матрицы смежности графа
SquareMatrixPower read(std::istream& in);
// посчитать ответ
SquareMatrix solve(const SquareMatrixPower& squareMatrixPower);
// вывод количества путей заданной длины из 0 вершины
std::ostream& write(std::ostream& out, const SquareMatrix& matrix);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    const SquareMatrixPower squareMatrixPower = read(std::cin);
    const SquareMatrix resultMatrix = solve(squareMatrixPower);
    write(std::cout, resultMatrix);

    return 0;
}

