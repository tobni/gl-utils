#pragma once

#include <array>
#include <gsl/span>
#include <tuple>
#include <type_traits>
#include <algorithm>


template<size_t m, size_t n, typename Scalar = float>
class Mat {
    static_assert((m > 0) && (n > 0), "Dimensions have to be larger than 0");
    static_assert(std::is_arithmetic<Scalar>::value, "Scalar has to be arithmetic type");
    
    Scalar elements[m*n];

    public:

    Mat() : elements{} {
        std::fill(elements, elements + m*n, static_cast<Scalar>(0));
    }

    template<typename... Scalars>
    Mat(Scalars... scalars) : elements{static_cast<Scalar>(scalars)...} {
        static_assert(sizeof ...(Scalars) <= m*n, "Too many elements");
    }

    // Element access
    Scalar& operator()(uint i, uint j) {
        assert(i < m);
        assert(j < n);
        return elements[i*n + j];
    }

    Scalar& operator()(uint i) {
        assert(i < m*n);
        return elements[i];
    }

    Scalar const& operator()(uint i, uint j) const {
        assert(i < m);
        assert(j < n);
        return elements[i*n + j];
    }

    Scalar const& operator()(uint i) const { 
        assert(i < m*n);
        return elements[i];
    }
    
    std::tuple<size_t, size_t> constexpr size() {
        return {m, n};
    }

    auto transpose() const {
        Mat<n,m,Scalar> result{};
        for (auto i = 0; i < m; i++) {
            for(auto j = 0; j < n; j++) {
                result(j,i) = (*this)(i,j);
            }
        }
        return result;
    }

    auto t() {
        return transpose();
    }

    auto decompose_PLU() const {
        static_assert(m == n, "PLU-decomposition only implemented for square matrices");
        Mat<m,n,Scalar> L{0}, U{*this}, P{0};

        // Find pivot
        for(auto i = 0; i < n; i++) {
            P(i,i) = Scalar(1);
        }
        return std::make_tuple(P, L, U);
    }
    
    // Matrix operations
    // Multiplication
    template<size_t p, typename OtherScalar>
    auto operator*(Mat<n, p, OtherScalar> const& other) const {
        using ResultScalar = decltype( std::declval<Scalar>() * std::declval<OtherScalar>() );
        Mat<m, p, ResultScalar> result{};

        for (auto i = 0; i < m; i++){
            for (auto j = 0; j < p; j++) {
                auto sum = ResultScalar(0);
                for (auto k = 0; k < n; k++) {
                    sum+= (*this)(i,k) * other(k,j);
                }
                result(i, j) = sum;
            }

        }
        if constexpr ((p == 1) && (m == 1)) {
            return result(0,0);
        } else {
            return result;
        }
    }
    // Addition
    template<typename OtherScalar>
    auto& operator+=(Mat<m, n, OtherScalar> const& other) {
        for (auto i = 0; i < m*n; i++){
            (*this)(i) += other(i);
        }
        return *this;
    }

    template<typename OtherScalar>
    auto operator+(Mat<m, n, OtherScalar> const& other) const {
        using ResultScalar = decltype( std::declval<Scalar>() + std::declval<OtherScalar>() );
        Mat<m, n, ResultScalar> result{*this};

        return result+=other;
    }

    // Subtraction
    template<typename OtherScalar>
    auto& operator-=(Mat<m, n, OtherScalar> const& other) {
        for (auto i = 0; i < m*n; i++){
            (*this)(i) -= other(i);
        }
        return *this;
    }

    template<typename OtherScalar>
    auto operator-(Mat<m, n, OtherScalar> const& other) const {
        using ResultScalar = decltype( std::declval<Scalar>() + std::declval<OtherScalar>() );
        Mat<m, n, ResultScalar> result{*this};

        return result-=other;
    }


    // Scalar operations
    // Addition
    template<typename OtherScalar, typename = typename std::enable_if<std::is_arithmetic<OtherScalar>::value>::type>
    auto& operator+=(OtherScalar scalar) {
        for (auto& e : elements) {
            e += scalar;
        }
        return *this;
    }

    template<typename OtherScalar, typename = typename std::enable_if<std::is_arithmetic<OtherScalar>::value>::type>
    auto operator+(OtherScalar scalar) {
        using ResultScalar = decltype( std::declval<Scalar>() + std::declval<OtherScalar>() );
        Mat<m,n,ResultScalar> result{*this};

        return result+=scalar;
    }
    
    // Subtraction
    template<typename OtherScalar, typename = typename std::enable_if<std::is_arithmetic<OtherScalar>::value>::type>
    auto& operator-=(OtherScalar scalar) {
        return *this += (-scalar);
    }

    template<typename OtherScalar, typename = typename std::enable_if<std::is_arithmetic<OtherScalar>::value>::type>
    auto operator-(OtherScalar scalar) {
        return *this + (-scalar);
    }

    // Multiplication
    template<typename OtherScalar, typename = typename std::enable_if<std::is_arithmetic<OtherScalar>::value>::type>
    auto& operator*=(OtherScalar scalar) {
        for(auto& e : elements) {
            e *= scalar;
        }
        return *this;
    }

    template<typename OtherScalar, typename = typename std::enable_if<std::is_arithmetic<OtherScalar>::value>::type>
    auto operator*(OtherScalar scalar) {
        using ResultScalar = decltype( std::declval<Scalar>() * std::declval<OtherScalar>() );
        Mat<m,n,ResultScalar> result{*this};
        return result *= scalar;
    }

    // Division
    template<typename OtherScalar, typename = typename std::enable_if<std::is_arithmetic<OtherScalar>::value>::type>
    auto& operator/=(OtherScalar scalar) {
        for(auto& e : elements) {
            e /= scalar;
        }
        return *this;
    }

    template<typename OtherScalar, typename = typename std::enable_if<std::is_arithmetic<OtherScalar>::value>::type>
    auto operator/(OtherScalar scalar) {
        using ResultScalar = decltype( std::declval<Scalar>() / std::declval<OtherScalar>() );
        Mat<m,n,ResultScalar> result{*this};
        return result /= scalar;
    }


};