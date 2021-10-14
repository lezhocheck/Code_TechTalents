#include <iostream>
#include <vector>

class Matrix{
public:
    using vi = std::vector<int64_t>;
    Matrix(const size_t n, const size_t m) :
            matrix_(std::vector(n, vi(m, 0))){}

    // получить ряд матрицы
    const vi& GetRow(const size_t row_number) const;
    // получить элемент по индексам
    const int64_t GetElement(const size_t row_number,
                             const size_t column_number) const;
    // назначить новое значение элементу по индексам
    void SetElement(const size_t row_number,
                    const size_t column_number, const int64_t value);
    // получить глубину матрицы
    const size_t GetXSize() const;
    // получить ширину матрицы
    const size_t GetYSize() const;

    Matrix operator* (const Matrix& other) const;
    // пвозвести матрицу в степень
    Matrix ElevateToPower(size_t power) const;

private:
    std::vector<vi> matrix_;
};

// инициализация матрицы смежности графа
Matrix InitializeMatrix(std::istream& in, int* power);
// посчитать ответ
Matrix Solve(const Matrix& matrix, int power);
// вывод количества путей заданной длины из 0 вершины
std::ostream& WriteResult(std::ostream& out, const Matrix& matrix);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int power;
    Matrix base_matrix = InitializeMatrix(std::cin, &power);
    Matrix result_matrix = Solve(base_matrix, power);
    WriteResult(std::cout, result_matrix);
    return 0;
}

