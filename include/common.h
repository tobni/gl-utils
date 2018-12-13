#pragma once
#include <map>
#include <utility>
#include <memory>

// Thin wrapper for owning pointer
template<class T> 
using owner = T*;

// Reference counted wrapper of shared_pointer, too verbose imo
template<class T>
using RefCnt = std::shared_pointer<T>;


// RefMap, a map with a reference_wrapped key value.
template<typename T>
struct Less{

    bool operator()(T const& lhs, T const& rhs) const 
    {
        return lhs<rhs;
    }
};
template<typename T, typename U>
using RefMap = std::map<std::reference_wrapper<T>, U, Less<T>>;
