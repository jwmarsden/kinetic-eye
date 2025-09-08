#pragma once

#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <unordered_set>

namespace k2c {

    // from: https://stackoverflow.com/a/57595105
    template <typename T, typename... Rest> void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        (hashCombine(seed, rest), ...);
    };

}