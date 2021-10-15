#include <iostream>
#include <vector>

class Matrix{
public:
    Matrix(const size_t row_count, const size_t column_count) :
            matrix_(std::vector(row_count,
                                std::vector<int64_t>(column_count, 0))){}
    // получить ряд матрицы
    const std::vector<int64_t>& get_row(const size_t row_number) const;
    // получить элемент по индексам
    const int64_t get_element(const size_t row_number,
                              const size_t column_number) const;
    // назначить новое значение элементу по индексам
    void set_element(const size_t row_number,
                     const size_t column_number, const int64_t value);

    // получить глубину матрицы
    const size_t get_x_size() const;
    // получить ширину матрицы
    const size_t get_y_size() const;

    Matrix operator* (const Matrix& other) const;
    // возвести матрицу в степень
    Matrix elevate_to_power(size_t power) const;

private:
    std::vector<std::vector<int64_t>> matrix_;
};

struct MatrixPower{
    Matrix matrix;
    int power;
};

// инициализация матрицы смежности графа
MatrixPower read(std::istream& in);
// посчитать ответ
Matrix solve(const MatrixPower& matrix_power);
// вывод количества путей заданной длины из 0 вершины
std::ostream& write(std::ostream& out, const Matrix& matrix);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    const MatrixPower base_matrix = read(std::cin);
    const Matrix result_matrix = solve(base_matrix);
    write(std::cout, result_matrix);
    
    return 0;
}

