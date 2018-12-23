#pragma once

#include <array>
#include <gsl/span>
#include <tuple>
#include <type_traits>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "vec.h"


template<size_t m, size_t n, typename Scalar = float>
class Mat {
    static_assert((m > 1) && (n > 1), "Dimensions have to be larger than 1");
    static_assert(std::is_arithmetic_v<Scalar>, "Scalar has to be arithmetic type");
    
    Scalar elements[m*n];

    public:

    // Constructors
    Mat() : elements{} {
        std::fill(elements, elements + m*n, static_cast<Scalar>(0));
    }

    template<typename... Scalars, std::enable_if_t<std::conjunction_v<std::is_arithmetic<Scalars>...>>...>
    Mat(Scalars... scalars) : elements{static_cast<Scalar>(scalars)...} {
        static_assert(sizeof ...(Scalars) <= m*n, "Too many elements");
    }

    template<typename OtherScalar>
    Mat(Mat<m,n,OtherScalar> const& other) {
        for (auto i = 0; i < m*n; i++) {
            (*this)(i) = static_cast<Scalar>(other(i));
        }
    }

    // Print
    std::string print(int width = 7) const {
        std::stringstream ss;
        for (auto i = 0; i < m; i++) {
            for(auto j = 0; j < n; j++) {
                ss << std::right << std::setw(width) << (*this)(i,j);
            }
            ss << std::endl; 
        }
        return ss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, Mat<m,n,Scalar> const& A)
    {
        return os << A.print();
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

    void swap_rows(size_t i, size_t j) {
        using std::swap;
        for (auto k = 0; k < n; k++) {
            swap((*this)(i,k), (*this)(j,k));
        }
    }

    void swap_columns(size_t i, size_t j) {
        using std::swap;
        for (auto k = 0; k < m; k++) {
            swap((*this)(k,i), (*this)(k,j));
        }
    }

    // Matrix operations
    // Multiplication
    template<size_t p, typename OtherScalar>
    auto operator*(Mat<n, p, OtherScalar> const& other) const {
        using ResultScalar = decltype( std::declval<Scalar>() * std::declval<OtherScalar>() );
        Mat<m, p, ResultScalar> result{};

        for (auto i = 0; i < m; i++) {
            for (auto j = 0; j < p; j++) {
                auto sum = ResultScalar(0);
                for (auto k = 0; k < n; k++) {
                    sum+= (*this)(i,k) * other(k,j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    template<typename OtherScalar>
    auto operator*(Vec<n, 1, OtherScalar> const& other) const {
        using ResultScalar = decltype( std::declval<Scalar>() * std::declval<OtherScalar>() );
        Vec<n, 1, ResultScalar> result{other};

        for (auto i = 0; i < m; i++) {
            auto sum = ResultScalar(0);
            for (auto k = 0; k < n; k++) {
                sum+= (*this)(i, k) * other(k);
            }
            result(i) = sum;
        }
        return result;
    }

    template<typename OtherScalar>
    friend auto operator*(Vec<1, m, OtherScalar> const& vector, Mat<m,n,Scalar> const& matrix) {
        using ResultScalar = decltype( std::declval<Scalar>() * std::declval<OtherScalar>() );
        Vec<1, m, ResultScalar> result{vector};
        
        for (auto i = 0; i < m; i++) {
            auto sum = ResultScalar(0);
            for (auto k = 0; k < n; k++) {
                sum+= matrix(i, k) * vector(k);
            }
            result(i) = sum;
        }
        return result;
    }

    // Addition
    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto& operator+=(Mat<m, n, OtherScalar> const& other) {
        for (auto i = 0; i < m*n; i++){
            (*this)(i) += other(i);
        }
        return *this;
    }

    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto operator+(Mat<m, n, OtherScalar> const& other) const {
        using ResultScalar = decltype( std::declval<Scalar>() + std::declval<OtherScalar>() );
        Mat<m, n, ResultScalar> result{*this};

        return result+=other;
    }

    // Subtraction
    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto& operator-=(Mat<m, n, OtherScalar> const& other) {
        for (auto i = 0; i < m*n; i++){
            (*this)(i) -= other(i);
        }
        return *this;
    }

    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto operator-(Mat<m, n, OtherScalar> const& other) const {
        using ResultScalar = decltype( std::declval<Scalar>() + std::declval<OtherScalar>() );
        Mat<m, n, ResultScalar> result{*this};

        return result-=other;
    }


    // Scalar operations
    // Addition
    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto& operator+=(OtherScalar scalar) {
        for (auto& e : elements) {
            e += scalar;
        }
        return *this;
    }

    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto operator+(OtherScalar scalar) const {
        using ResultScalar = decltype( std::declval<Scalar>() + std::declval<OtherScalar>() );
        Mat<m,n,ResultScalar> result{*this};

        return result+=scalar;
    }

    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    friend auto operator+(OtherScalar scalar, Mat<m,n,Scalar> const& matrix) {
        return matrix+scalar;
    }
    
    // Subtraction
    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto& operator-=(OtherScalar scalar) {
        return *this += (-scalar);
    }

    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto operator-(OtherScalar scalar) const {
        return *this + (-scalar);
    }

    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    friend auto operator-(OtherScalar scalar, Mat<m,n,Scalar> const& matrix) {
        return matrix-scalar;
    }

    // Multiplication
    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto& operator*=(OtherScalar scalar) {
        for(auto& e : elements) {
            e *= scalar;
        }
        return *this;
    }

    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto operator*(OtherScalar scalar) const {
        using ResultScalar = decltype( std::declval<Scalar>() * std::declval<OtherScalar>() );
        Mat<m,n,ResultScalar> result{*this};
        return result *= scalar;
    }

    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    friend auto operator*(OtherScalar scalar, Mat<m,n,Scalar> const& matrix) {
        return matrix*scalar;
    }

    // Division
    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto& operator/=(OtherScalar scalar) {
        for(auto& e : elements) {
            e /= scalar;
        }
        return *this;
    }

    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    auto operator/(OtherScalar scalar) const {
        using ResultScalar = decltype( std::declval<Scalar>() / std::declval<OtherScalar>() );
        Mat<m,n,ResultScalar> result{*this};
        return result /= scalar;
    }

    template<typename OtherScalar, std::enable_if_t<std::is_arithmetic_v<OtherScalar>>...>
    friend auto operator/(OtherScalar scalar, Mat<m,n,Scalar> const& matrix) {
        using ResultScalar = decltype( std::declval<Scalar>() * std::declval<OtherScalar>() );
        Mat<m,n,ResultScalar> result{};

        for (auto i = 0; i < m*n; i++) {
            result(i) = scalar / matrix(i);
        }

        return result;
    }
};
