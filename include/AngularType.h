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

namespace Space2D {
	static inline long double Pi = 3.1415926535897932;
	static inline long double epsilon = 1e-6;

	template<typename T, typename Tag, typename Ratio>
	class AngularType

	{
	public:

		using Angular = AngularType<T, Tag, Ratio>;

		constexpr AngularType() noexcept : value(0) {}
		constexpr explicit AngularType(const T& value) noexcept : value(value) {}
		constexpr explicit AngularType(const Angular& other) noexcept 
			: value(other.value) {}
		constexpr explicit AngularType(const long double& value) noexcept 
			: value(static_cast<T>(value)) {}

		T& get() noexcept { return value; }
		const T& get() const noexcept { return value; }

		template <typename OtherRatio>
		operator AngularType<T, Tag, OtherRatio>() const {
			return  AngularType<T, Tag, OtherRatio>(get() * Ratio::num / Ratio::den * OtherRatio::den / OtherRatio::num);
		}

		auto operator<=>(const Angular&) const noexcept = default;
		bool operator== (const Angular& other) const noexcept {
			return std::abs(value - other.value) < epsilon;
		}

		explicit operator T() const {
			return value;
		}

#ifndef S2D_ANGULAR_OPERATOR
#define S2D_ANGULAR_OPERATOR(op) \
    constexpr inline Angular& operator##op##=(const Angular& rhs) noexcept {\
        value op##= (T)rhs;\
        return *this;\
	}\
	constexpr inline Angular operator##op(const Angular& rhs) const noexcept { \
	    return Angular(value op (T)rhs);\
	}\
	constexpr inline Angular& operator##op##=(const T& rhs) noexcept {\
        value op##= (T)rhs;\
        return *this;\
	}\
	constexpr inline Angular& operator##op(const T& rhs) const noexcept { \
	    return Angular(value op (T)rhs);\
	}\
    constexpr inline friend Angular& operator##op(const T& lhs, const Angular& rhs) noexcept { \
		return Angular(lhs op rhs.value);\
	}
#endif

		S2D_ANGULAR_OPERATOR(-)
		S2D_ANGULAR_OPERATOR(+)
		S2D_ANGULAR_OPERATOR(*)
		S2D_ANGULAR_OPERATOR(/)
		S2D_ANGULAR_OPERATOR(%)


		constexpr Angular operator-() const noexcept {
			return Angular(-value);
		}

		constexpr Angular operator+() const noexcept {
			return Angular(+value);
		}

	private:
		T value;
	};

	template<typename Ratio>
	using AngType = AngularType<float, struct Ang, Ratio>;

	using Radians = AngType<std::ratio<1, 1>>;

	//pi radians = 180 degrees
	using Degrees = AngType<std::ratio<31415926535897932, 1800000000000000000>>;

	//2pi radians = 100%
	using Percent = AngType<std::ratio<31415926535897932, 5000000000000000>>;

	inline Radians operator"" _rad(const long double d) noexcept {
		return Radians(d);
	}

	inline Radians operator"" _pirad(const long double d) noexcept {
		return Radians(d * 3.1415926535897932);
	}

	inline Degrees operator"" _deg(const long double d) noexcept {
		return Degrees(d);
	}
	inline Percent operator"" _pcent(const long double d) noexcept {
		return Percent(static_cast<long double>(d / 100.0));
	}

	inline Radians operator"" _rad(const unsigned long long d) noexcept {
		return Radians(static_cast<long double>(d));
	}

	inline Radians operator"" _pirad(const unsigned long long d) noexcept {
		return Radians(static_cast<long double>(d) * 3.1415926535897932);
	}

	inline Degrees operator"" _deg(const unsigned long long d) noexcept {
		return Degrees(static_cast<long double>(d));
	}

	inline Percent operator"" _pcent(const unsigned long long d) noexcept {
		return Percent(static_cast<long double>(d / 100.0));
	}

	inline std::ostream& operator << (std::ostream& os, const Radians& it) {
		double pie_ratio = (it / 180_deg).get();
		if (pie_ratio == 1) {
			os << "pi radians";
			return os;
		}

		if (pie_ratio == 0) {
			os << "0 radians";
			return os;
		}

		long precisison = 1000;

		long gcdnum = std::gcd((long)round(pie_ratio * precisison), precisison);

		double numerator = (pie_ratio * precisison) / (double)gcdnum;
		long denominator = precisison / gcdnum;

		if (abs(numerator - 1) < 1e-6) {
			os << "pi" << "/" << denominator << " radians";
			return os;
		}

		if (denominator == 1) {
			os << numerator << "pi radians";
			return os;
		}

		if (gcdnum == 1) {
			os << pie_ratio << "pi radians";
			return os;
		}

		os << numerator << "pi/" << denominator << " radians";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const Degrees& it) {
		os << it.get() << " degrees";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const Percent& it) {
		os << it.get() * 100.0 << "%";
		return os;
	}

}
