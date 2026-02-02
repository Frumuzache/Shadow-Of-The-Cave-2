#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <type_traits>
#include <iostream>

/**
 * @brief Template functions for common math operations
 * Used throughout the game for calculations
 */
namespace MathHelper {

/**
 * @brief Clamp a value between min and max
 *
 * Template Function Instantiation 1: clamp<float>
 * Template Function Instantiation 2: clamp<int>
 *
 * @tparam T Numeric type (int, float, double)
 * @param value The value to clamp
 * @param min Minimum value
 * @param max Maximum value
 * @return Clamped value
 */
template<typename T>
T clamp(T value, T min, T max) {
    static_assert(std::is_arithmetic<T>::value, "clamp requires arithmetic type");

    if (value < min) return min;
    if (value > max) return max;
    return value;
}

/**
 * @brief Calculate distance between two points
 *
 * Template Function Instantiation 3: distance<float>
 *
 * @tparam T Vector component type (float, double)
 * @param a First point
 * @param b Second point
 * @return Distance between points
 */
template<typename T>
T distance(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    static_assert(std::is_floating_point<T>::value, "distance requires floating point type");

    T dx = b.x - a.x;
    T dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}

/**
 * @brief Normalize a vector (make it unit length)
 *
 * Template Function Instantiation 4: normalize<float>
 *
 * @tparam T Vector component type (float, double)
 * @param vec Vector to normalize
 * @return Normalized vector (length 1) or zero vector if input was zero
 */
template<typename T>
sf::Vector2<T> normalize(const sf::Vector2<T>& vec) {
    static_assert(std::is_floating_point<T>::value, "normalize requires floating point type");

    T length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
    if (length == 0) return sf::Vector2<T>(0, 0);

    return sf::Vector2<T>(vec.x / length, vec.y / length);
}

/**
 * @brief Linear interpolation between two values
 *
 * Template Function Instantiation 5: lerp<float>
 *
 * @tparam T Numeric type
 * @param a Start value
 * @param b End value
 * @param t Interpolation factor (0.0 to 1.0)
 * @return Interpolated value
 */
template<typename T>
T lerp(T a, T b, float t) {
    static_assert(std::is_arithmetic<T>::value, "lerp requires arithmetic type");

    return a + (b - a) * t;
}

/**
 * @brief Get maximum of two values
 *
 * @tparam T Comparable type
 * @param a First value
 * @param b Second value
 * @return Maximum value
 */
template<typename T>
const T& max(const T& a, const T& b) {
    return (a > b) ? a : b;
}

/**
 * @brief Get minimum of two values
 *
 * @tparam T Comparable type
 * @param a First value
 * @param b Second value
 * @return Minimum value
 */
template<typename T>
const T& min(const T& a, const T& b) {
    return (a < b) ? a : b;
}

} // namespace MathHelper

#endif // MATH_HELPER_H
