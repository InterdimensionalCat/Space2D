#pragma once
#include <ratio>
#include <iostream>
#include <numeric>
#include <cmath>

/*
  Defines the 3 "angular types"; degrees, radians, and percent
  This allowes for an angle value to be expressed in any of these 3
  ways, and each type implicity converts to the others so that you
  dont have to worry about which one a particular function takes

  NOTE: percent is defined OpenGL style: 1.0 == 100 percent
  since this is more helpful mathematically (and is what I am used to)
  however, 1.0 percent will print as "100%" for clarity
*/

#ifndef S2D_PIXEL_TO_METER
#define S2D_PIXEL_TO_METER 64
#endif

namespace Space2D {

	template<typename T, typename Tag, typename Ratio>
	class LinearType

	{
	public:

		using Linear = LinearType<T, Tag, Ratio>;

		constexpr LinearType() noexcept : value(0) {}
		constexpr LinearType(const T& value) noexcept : value(value) {}
		constexpr LinearType(const Linear& other) noexcept
			: value(other.value) {}
		constexpr explicit LinearType(const long double& value) noexcept
			: value(static_cast<T>(value)) {}
		constexpr explicit LinearType(const double& value) noexcept
			: value(static_cast<T>(value)) {}
		constexpr explicit LinearType(const unsigned long long & value) noexcept
			: value(static_cast<T>(value)) {}

		T& get() noexcept { return value; }
		const T& get() const noexcept { return value; }

		template <typename OtherRatio>
		operator LinearType<T, Tag, OtherRatio>() const {

			return  LinearType<T, Tag, OtherRatio>(get() * Ratio::num / Ratio::den * OtherRatio::den / OtherRatio::num);
		}

		auto operator<=>(const Linear&) const noexcept = default;
		bool operator== (const Linear& other) const noexcept {
			return std::abs(value - other.value) < epsilon;
		}

		explicit operator double() const {
			return (double)value;
		}

		explicit operator float() const {
			return (float)value;
		}

#ifndef S2D_LINEAR_OPERATOR
#define S2D_LINEAR_OPERATOR(op) \
    constexpr inline Linear& operator##op##=(const Linear& rhs) noexcept {\
        value op##= (T)rhs;\
        return *this;\
	}\
	constexpr inline Linear operator##op(const Linear& rhs) const noexcept { \
	    return Linear(value op (T)rhs);\
	}\
	constexpr inline Linear& operator##op##=(const T& rhs) noexcept {\
        value op##= (T)rhs;\
        return *this;\
	}\
	constexpr inline Linear operator##op(const T& rhs) const noexcept { \
	    return Linear(value op (T)rhs);\
	}
#endif

		S2D_LINEAR_OPERATOR(-)
		S2D_LINEAR_OPERATOR(+)
		S2D_LINEAR_OPERATOR(*)
		S2D_LINEAR_OPERATOR(/ )
		S2D_LINEAR_OPERATOR(%)


			constexpr Linear operator-() const noexcept {
			return Linear(-value);
		}

		constexpr Linear operator+() const noexcept {
			return Linear(+value);
		}

	private:
		T value;
	};

	template<typename Ratio>
	using LinType = LinearType<float, struct Lin, Ratio>;

	using Pixels = LinType<std::ratio<1, 1>>;

	//1 pixel = _PIXEL_TO_METER meters
	//using Meters = LinType<std::ratio<64, 1>>;

	using Meters = LinType<std::ratio< S2D_PIXEL_TO_METER , 1>>;

	namespace literals {

	}

	static float toMeters(const float pixels) {
		return pixels / S2D_PIXEL_TO_METER;
	}

	static float toPixels(const float meters) {
		return meters * S2D_PIXEL_TO_METER;
	}

}


inline Space2D::Pixels operator"" _px(const long double d) noexcept {
	return Space2D::Pixels(d);
}

inline Space2D::Pixels operator"" _px(const unsigned long long d) noexcept {
	return Space2D::Pixels(d);
}

inline Space2D::Meters operator"" _mtr(const long double d) noexcept {
	return Space2D::Meters(d);
}

inline Space2D::Meters operator"" _mtr(const unsigned long long d) noexcept {
	return Space2D::Meters(d);
}


inline std::ostream& operator << (std::ostream& os, const Space2D::Pixels& it) {
	os << it.get() << "_px";
	return os;
}

inline std::ostream& operator << (std::ostream& os, const Space2D::Meters& it) {
	os << it.get() << "_mtr";
	return os;
}
