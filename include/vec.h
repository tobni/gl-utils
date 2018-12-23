#pragma once

#include <type_traits>
#include <algorithm>
#include <array>

template<size_t m, size_t n, typename Scalar>
class Vec {
    static_assert(m == 1 || n == 1, "A vector is 1xn or mx1");
    static_assert(std::is_arithmetic_v<Scalar>, "Scalar has to be arithmetic type");
    std::array<Scalar, m*n > elements;

    public:
    
    template<typename... Scalars, std::enable_if_t<std::conjunction_v<std::is_arithmetic<Scalars>...>>...>
    Vec(Scalars... scalars) : elements{static_cast<Scalar>(scalars)...} {
        static_assert(sizeof ...(Scalars) <= m*n, "Too many elements");
    }

    template<typename OtherScalar>
    Vec(Vec<m,n,OtherScalar> const& other) {
        for (auto i = 0; i < m*n; i++) {
            (*this)(i) = static_cast<Scalar>(other(i));
        }
    }


    // Print
    std::string print(int width = 7) const {
        std::stringstream ss;
        char const* separator = "";
        for (auto i = 0; i < m*n; i++) {
            ss << separator << elements[i];

            if constexpr( m == 1 ) {
                separator = std::endl;
            } else {
                separator = " ";
            }
        }
        return ss.str();
    }

    template<typename... Swizzle, std::enable_if_t<std::conjunction_v<std::is_integral<Swizzle>...>>...>
    auto operator() (size_t i, Swizzle... swizzle) const {
        if constexpr (sizeof...(swizzle) == 0) {
            assert(i < m*n);
            return elements[i];
        } else {
            size_t constexpr size = sizeof...(swizzle) + 1;
            std::array<size_t, sizeof...(swizzle)> index{static_cast<size_t>(swizzle)...};
            
            Vec<size, 1, Scalar> result{};
            
            result(0) = (*this)(i);

            auto k = 1;
            for(auto j : index) {
                result(k) = (*this)(j); 
                k++;
            }

            if constexpr (m==1) {
                auto result_t = result.transpose();
                return result_t;
            } else {
                return result;
            }
        }
    }

    auto& operator() (size_t i) {
        assert(i < m*n);
        return elements[i];
    }

    auto transpose() const {
        Vec<n,m,Scalar> result{};
        for (auto i = 0; i < m*n; i++) {
            result(i) = elements[i];
        }
        return result;
    }

};
