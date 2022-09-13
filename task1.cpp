#include <array>
#include <iostream>
#include <variant>

constexpr int get_cofactor_coef(int i, int j){
    return (i + j) % 2 == 0 ? 1 : -1;
}


template<std::size_t N>
constexpr std::array<std::array<int, N - 1>, N - 1>
get_submatrix_of_a(const std::array<std::array<int, N>, N>& a, int i, int j) {
    std::array<std::array<int, N - 1>, N - 1> r{};  // initialize in constexpr
    for (int ii = 0; ii != N - 1; ++ii)
        for (int jj = 0; jj != N - 1; ++jj)
            (&std::get<0>(((&std::get<0>(r))[ii])))[jj] = a[ii + (ii >= i ? 1 : 0)][jj + (jj >= j ? 1 : 0)];
    return r;
}


// Laplace expansion by 0th collumn
template <int N>
constexpr int det(const std::array<std::array<int, N>, N>& a) {
    int res = 0;
    for (int i = 0; i < N; i++) {
        res += get_cofactor_coef(i, 0) * a[i][0] * 
               det<N-1>(get_submatrix_of_a<N>(a, i, 0));
    }
    return res;
}


template <>
constexpr int det<1>(const std::array<std::array<int, 1>, 1>& a) {
    return a[0][0];
}


int main() {
    constexpr std::array<std::array<int, 3>, 3> A = {{
        {0, 1, 2},
        {1, 2, 3},
        {2, 3, 7},
    }};
    constexpr int res = det<3>(A);
    std::cout << "Result: " << res << std::endl;
}