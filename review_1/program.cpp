#include <iostream>
#include <vector>

constexpr int MOD = 1'000'000'007;

const std::vector<int64_t>& Matrix::get_row(const size_t row_number) const {
    return matrix_.at(row_number);
}

const int64_t Matrix::get_element(const size_t row_number,
                                  const size_t column_number) const {
    return matrix_.at(row_number).at(column_number);
}

void Matrix::set_element(const size_t row_number,
                         const size_t column_number, const int64_t value){
    matrix_.at(row_number).at(column_number) = value;
}

const size_t Matrix::get_x_size() const {
    return matrix_.size();
}

const size_t Matrix::get_y_size() const {
    return matrix_[0].size();
}

Matrix Matrix::operator* (const Matrix& other) const {
    size_t x_size = get_x_size();
    size_t y_size = get_y_size();
    size_t other_x_size = other.get_x_size();
    size_t other_y_size = other.get_y_size();

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
            result_matrix.set_element(row, column, new_element);
        }
    }
    return result_matrix;
}

Matrix Matrix::elevate_to_power(size_t power) const {
    size_t x_size = get_x_size();
    Matrix this_matrix = *this;
    Matrix result_matrix(x_size, x_size);

    for (size_t row = 0; row < x_size; ++row) {
        result_matrix.set_element(row, row, 1);
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

MatrixPower read(std::istream& in) {
    int vertices, edges, power;
    in >> vertices >> edges >> power;
    Matrix result_matrix(vertices, vertices);

    for (int edge = 0; edge < edges; ++edge) {
        int index_first, index_second;
        in >> index_first >> index_second;
        index_first--;
        index_second--;
        result_matrix.set_element(index_first, index_second,
            result_matrix.get_element(index_first, index_second) + 1);
    }

    return {result_matrix, power};
}

Matrix solve(const MatrixPower& matrix_power) {
    return matrix_power.matrix.elevate_to_power(matrix_power.power);
}

std::ostream& write(std::ostream& out, const Matrix& matrix) {
    int64_t answer = 0;

    for (int64_t value : matrix.get_row(0)){
        answer += (value % MOD);
    }

    out << answer % MOD << "\n";
    return out;
}
