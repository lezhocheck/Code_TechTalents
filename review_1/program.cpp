#include <iostream>
#include <vector>

constexpr int MOD = 1'000'000'007;

const std::vector<int64_t>& Matrix::get_row(size_t i) const {
    return _matrix.at(i);
}

int64_t& Matrix::at(size_t i, size_t j) {
    return _matrix.at(i).at(j);
}

size_t Matrix::x_size() const {
    return _matrix.size();
}

size_t Matrix::y_size() const {
    return _matrix[0].size();
}

Matrix Matrix::operator* (const Matrix& other) const {
    size_t n = x_size();
    size_t m = y_size();
    size_t other_n = other.x_size();
    size_t other_m = other.y_size();

    if (m != other_n) {
        throw std::invalid_argument("Matrices sizes mismatch");
    }

    Matrix res(n, other_m);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < other_m; ++j) {
            int64_t s = 0;
            for (size_t k = 0; k < n; ++k) {
                s = (s + (_matrix[i][k] * other._matrix[k][j]) % MOD) % MOD;
            }
            res.at(i, j) = s;
        }
    }
    return res;
}

Matrix Matrix::bin_pow(size_t k) const {
    size_t n = x_size();
    Matrix temp = *this;
    Matrix res(n, n);

    for (size_t i = 0; i < n; ++i) {
        res.at(i, i) = 1;
    }

    while (k > 0) {
        if (k % 2 == 1) {
            res = temp * res;
        }
        temp = temp * temp;
        k /= 2;
    }
    return res;
}

Matrix create_adj(std::istream& in, int vertices, int edges) {
    Matrix res(vertices, vertices);

    for (int i = 0; i < edges; ++i) {
        int a, b;
        in >> a >> b;
        a--;
        b--;
        res.at(a, b)++;
    }

    return res;
}

std::ostream& write_res(std::ostream& out, const Matrix& m) {
    int64_t ans = 0;

    for (int64_t i : m.get_row(0)){
        ans += (i % MOD);
    }

    out << ans % MOD;
    return out;
}
