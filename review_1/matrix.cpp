#include <iostream>
#include <vector>

class Matrix{
 public:
     using vi = std::vector<int64_t>;
     Matrix(size_t n, size_t m) :
             _matrix(std::vector(n, vi(m, 0))){}

     const vi& get_row(size_t i) const;
     // обратиться к элементу с индексами i, j
     int64_t& at(size_t i, size_t j);
     // получить глубину матрицы
     size_t x_size() const;
     // получить ширину матрицы
     size_t y_size() const;

     Matrix operator* (const Matrix& other) const;
     Matrix bin_pow(size_t k) const;

 private:
     std::vector<vi> _matrix;
};

// инициализация матрицы смежности графа
Matrix create_adj(std::istream& in, int vertices, int edges);
// вывод количества путей заданной длины из 0 вершины
std::ostream& write_res(std::ostream& out, const Matrix& m);

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int N, M, K;
    std::cin >> N >> M >> K;
    Matrix base = create_adj(std::cin, N, M);
    Matrix res = base.bin_pow(K);
    write_res(std::cout, res);
    return 0;
}
