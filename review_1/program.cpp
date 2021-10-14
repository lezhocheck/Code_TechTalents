#include <iostream>
#include <vector>

constexpr int MOD = 1'000'000'007;

class Matrix{
 public:
     using vi = std::vector<int64_t>;
     Matrix(const size_t n, const size_t m) :
             matrix_(std::vector(n, vi(m, 0))){}

     const vi& GetRow(const size_t row_number) const;
     const int64_t GetElement(const size_t row_number,
                              const size_t column_number) const;
     void SetElement(const size_t row_number,
                     const size_t column_number, const int64_t value);

     const size_t GetXSize() const;
     const size_t GetYSize() const;

     Matrix operator* (const Matrix& other) const;
     Matrix ElevateToPower(size_t power) const;

 private:
     std::vector<vi> matrix_;
};

const std::vector<int64_t>& Matrix::GetRow(const size_t row_number) const {
    return matrix_.at(row_number);
}

const int64_t Matrix::GetElement(const size_t row_number,
                                 const size_t column_number) const {
    return matrix_.at(row_number).at(column_number);
}

void Matrix::SetElement(const size_t row_number,
                        const size_t column_number, const int64_t value){
    matrix_.at(row_number).at(column_number) = value;
}

const size_t Matrix::GetXSize() const {
    return matrix_.size();
}

const size_t Matrix::GetYSize() const {
    return matrix_[0].size();
}

Matrix Matrix::operator* (const Matrix& other) const {
    size_t x_size = GetXSize();
    size_t y_size = GetYSize();
    size_t other_x_size = other.GetXSize();
    size_t other_y_size = other.GetYSize();

    if (y_size != other_x_size) {
        throw std::invalid_argument("Matrices sizes mismatch");
    }

    Matrix result_matrix(x_size, other_y_size);

    for (size_t row = 0; row < x_size; ++row) {
        for (size_t column = 0; column < other_y_size; ++column) {
            int64_t new_element = 0;
            for (size_t k = 0; k < x_size; ++k) {
                new_element = (new_element + (matrix_[row][k] *
                    other.matrix_[k][column]) % MOD) % MOD;
            }
            result_matrix.SetElement(row, column, new_element);
        }
    }
    return result_matrix;
}

Matrix Matrix::ElevateToPower(size_t power) const {
    size_t x_size = GetXSize();
    Matrix this_matrix = *this;
    Matrix result_matrix(x_size, x_size);

    for (size_t row = 0; row < x_size; ++row) {
        result_matrix.SetElement(row, row, 1);
    }

    while (power > 0) {
        if (power % 2 == 1) {
            result_matrix = this_matrix * result_matrix;
        }
        this_matrix = this_matrix * this_matrix;
        power /= 2;
    }
    return result_matrix;
}

Matrix InitializeMatrix(std::istream& in, int* power) {
    int vertices, edges;
    in >> vertices >> edges >> *power;
    Matrix result_matrix(vertices, vertices);

    for (int edge = 0; edge < edges; ++edge) {
        int index_first, index_second;
        in >> index_first >> index_second;
        index_first--;
        index_second--;
        result_matrix.SetElement(index_first, index_second,
            result_matrix.GetElement(index_first, index_second) + 1);
    }

    return result_matrix;
}

Matrix Solve(const Matrix& matrix, int power) {
    return matrix.ElevateToPower(power);
}

std::ostream& WriteResult(std::ostream& out, const Matrix& matrix) {
    int64_t answer = 0;

    for (int64_t value : matrix.GetRow(0)){
        answer += (value % MOD);
    }

    out << answer % MOD << "\n";
    return out;
}
