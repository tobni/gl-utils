#pragma once
#include "mat.h"

template<size_t m, size_t n, typename Scalar, std::enable_if_t<std::is_floating_point_v<Scalar>>...>
auto decompose_PLU(Mat<m,n,Scalar> const& A) {

    Mat<m,n,Scalar> U{A};
    Mat<m,m> L{0}, P{0};

    for(auto i = 0; i < m; i++) {
        P(i,i) = Scalar(1);
    }
    
    for(auto i=0; i < m-1; i++) {

        Scalar pivot = 0;
        size_t pivot_row = i;
        auto constexpr p =  (m < n) ? m : n;
        for(auto j=i; j < p; j++) {
            Scalar element = (U(j,i) < 0) ? -U(j,i) : U(j,i);

            if (element > pivot) {
                pivot = element;
                pivot_row = j;
            }

        }

        P.swap_rows(pivot_row, i);
        U.swap_rows(pivot_row, i);
        L.swap_rows(pivot_row, i);
        for (auto j = i + 1; j < m; j++) {
            Scalar s = (i >= n) ? 0 : -U(j,i)/U(i,i);
            L(j,i) = -s;

            for(auto k = 0; k < n; k++) {
                U(j,k) += s * U(i,k);
            }
        }

    }
    for(auto i = 0; i < m; i++) {
        L(i,i) = Scalar(1);
    }

    P = P.transpose();
    
    return std::make_tuple(P, L, U);
}

template<size_t m, typename Scalar, std::enable_if_t<std::is_floating_point_v<Scalar>>...>
auto inverse(Mat<m,m,Scalar> const& A) {

    auto [P, L, U] = decompose_PLU(A);
    return P*L*U;
}