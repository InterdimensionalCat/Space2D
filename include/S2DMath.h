#pragma once
#include <cmath>

namespace Space2D {
	template<typename T>
	constexpr inline T lerp(const T a, const T b, const T t) noexcept {
		return a + t * (b - a);
	}

	template<typename T>
	using not_angular = std::enable_if<
		!std::is_same<T, Radians>::value &&
		!std::is_same<T, Degrees>::value &&
		!std::is_same<T, Percent>::value
	>;
	//using not_angular = std::enable_if<!std::is_same<T, Radians>::value>>;// && !std::is_same<T, Degrees> && !std::is_same<T, Percent>>;

#ifndef S2D_STDMATH_FN
#define S2D_STDMATH_FN(fn)\
	template<typename T>\
	inline T fn(const T a) noexcept {\
		return (T)(std::##fn((double)a));\
	}
#endif


	S2D_STDMATH_FN(sqrt)
	S2D_STDMATH_FN(abs)
	//template<typename T>
	//constexpr inline T cos(const T a) noexcept {
	//	return static_cast<T>(std::cos(static_cast<double>(a)));
	//}

	//template<typename T>
	//constexpr inline T sin(const T a) noexcept {
	//	return static_cast<T>(std::sin(static_cast<double>(a)));
	//}

	inline float cos(const Radians a) noexcept {
		return std::cos(a.get());
	}

	inline float sin(const Radians a) noexcept {
		return std::sin(a.get());
	}
}

//standard linear interpolation
constexpr inline float lerp(const float a, const float b, const float t) noexcept {
	return std::lerp(a, b, t);
}

#ifdef _SFML_ENABLED
#include "SFML\Graphics.hpp"
// "2D linear interpolation"; just interprets each component of the vector individually
inline sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, const float t) noexcept {
	return sf::Vector2f(a.x + t * (b.x - a.x), a.y + t * (b.y - a.y));
}

//prints sf::Vector2f type
inline std::ostream& operator<<(std::ostream& os, const sf::Vector2f& rhs) {
	os << "sf::Vector2f(" << rhs.x << "," << rhs.y << ")";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const sf::Vector2i& rhs) {
	os << "sf::Vector2i(" << rhs.x << "," << rhs.y << ")";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const sf::Vector2u& rhs) {
	os << "sf::Vector2u(" << rhs.x << "," << rhs.y << ")";
	return os;
}
#endif