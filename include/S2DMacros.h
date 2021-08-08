#pragma once

namespace Space2D {

#ifndef S2D_OPS
#define S2D_OPS
#ifndef S2D_OP_MINUS_PLUS
#define S2D_OP_MINUS_PLUS(lhstyp, rhstyp) \
template<typename T>\
constexpr lhstyp<T>& operator-=(lhstyp<T>& lhs, const rhstyp<T>& rhs) noexcept {\
    lhs.x -= rhs.x;\
    lhs.y -= rhs.y;\
    return lhs;\
}\
template<typename T>\
constexpr lhstyp<T>& operator+=(lhstyp<T>& lhs, const rhstyp<T>& rhs) noexcept {\
    lhs.x += rhs.x;\
    lhs.y += rhs.y;\
    return lhs;\
}\
template<typename T>\
constexpr lhstyp<T> operator-(const lhstyp<T>& lhs, const rhstyp<T>& rhs) noexcept {\
    return lhstyp<T>(lhs.x - rhs.x, lhs.y - rhs.y);\
}\
template<typename T>\
constexpr lhstyp<T> operator+(const lhstyp<T>& lhs, const rhstyp<T>& rhs) noexcept {\
    return lhstyp<T>(lhs.x + rhs.x, lhs.y + rhs.y);\
}
#endif

#ifndef S2D_OP_MINUS_PLUS_RET_VEC
#define S2D_OP_MINUS_PLUS_RET_VEC(lhstyp, rhstyp) \
template<typename T>\
Vec2<T> operator-(const lhstyp<T>& lhs, const rhstyp<T>& rhs) noexcept {\
    return lhstyp<T>(lhs.x - rhs.x, lhs.y - rhs.y);\
}\
template<typename T>\
Vec2<T> operator+(const lhstyp<T>& lhs, const rhstyp<T>& rhs) noexcept {\
    return lhstyp<T>(lhs.x + rhs.x, lhs.y + rhs.y);\
}
#endif

#ifndef S2D_OP_DOT
#define S2D_OP_DOT(lhstyp, rhstyp) \
template<typename T>\
T Dot(const lhstyp<T>& lhs, const rhstyp<T>& rhs) noexcept {\
    return lhs.x * rhs.x + lhs.y * rhs.y;\
}\
template<typename T>\
bool perp(const lhstyp<T>& lhs, const rhstyp<T>& rhs) noexcept {\
	return std::abs(Dot(lhs, rhs)) < 1e-6;\
}
#endif

#ifndef S2D_OP_SCALE
#define S2D_OP_SCALE(lhstyp) \
    template<typename T>\
    lhstyp<T>& operator*=(lhstyp<T>& lhs, const T& factor) noexcept {\
        lhs.x *= factor;\
        lhs.y *= factor;\
        return lhs;\
    }\
    template<typename T>\
    lhstyp<T>& operator*(const lhstyp<T>& lhs, const T& factor) noexcept {\
	    return lhstyp<T>(lhs.x * factor, lhs.y * factor);\
    }\
    template<typename T>\
    lhstyp<T> operator/=(lhstyp<T>& lhs, const T& factor) {\
        lhs.x /= factor;\
        lhs.y /= factor;\
        return lhs;\
    }\
    template<typename T>\
    lhstyp<T> operator/(const lhstyp<T>& lhs, const T& factor) {\
	    return lhstyp<T>(lhs.x / factor, lhs.y / factor);\
    }
#endif

#ifndef S2D_OP_SCALE_RET_VEC
#define S2D_OP_SCALE_RET_VEC(lhstyp) \
    template<typename T>\
    Vec2<T>& operator*(const lhstyp<T>& lhs, const T& factor) noexcept {\
	    return Vec2<T>(lhs.x * factor, lhs.y * factor);\
    }\
    template<typename T>\
    Vec2<T> operator/(const lhstyp<T>& lhs, const T& factor) {\
	    return Vec2<T>(lhs.x / factor, lhs.y / factor);\
    }
#endif

#ifndef S2D_OP_CAST
#define S2D_OP_CAST(lhstyp, rhstyp) \
template <typename T>\
operator lhstyp<T>(rhstyp<T>& rhs) {\
    return lhstyp<T>(rhs.x, rhs.y);\
}
#endif

#else
#ifndef S2D_OP_MINUS_PLUS
#define S2D_OP_MINUS_PLUS(rhstyp)
#endif

#ifndef S2D_OP_DOT
#define S2D_OP_DOT(rhstyp)
#endif

#ifndef S2D_OP_SCALE
#define S2D_OP_SCALE(rhstyp)
#endif

#ifndef S2D_OP_PERP
#define S2D_OP_PERP(rhstyp)
#endif

#ifndef S2D_OP_ARR
#define S2D_OP_ARR
#endif

#endif

#ifndef S2D_OPS_ITR
#define S2D_OPS_ITR(lhst, rhst, op) \
    template<typename T>\
    constexpr inline lhst<T>& operator##op##=(lhst<T>& lhs, const rhst<T>& rhs) noexcept {\
        size_t rhstNum = rhs.numVals();\
        size_t lhstNum = lhs.numVals();\
        size_t count = 0;\
        for (size_t i = 0; i < lhstNum; i++) {\
            for(size_t j = 0; j < rhstNum; j++) {\
                lhs[i] op##= rhs[j];\
            }\
        }\
        return *this;\
	}\
    template<typename T>\
	constexpr inline lhst<T> operator##op(const lhst<T>& lhs, const rhst<T>& rhs) noexcept { \
        lhst<T> retval;\
        size_t rhstNum = rhs.numVals();\
        size_t lhstNum = lhs.numVals();\
        size_t count = 0;\
        for (size_t i = 0; i < lhstNum; i++) {\
            for(size_t j = 0; j < rhstNum; j++) {\
                retval[i] = lhs[i] op rhs[j];\
            }\
        }\
        return retval;\
	}
#endif

    //S2D_OP_MINUS_PLUS(Point2, Vec2)
    //S2D_OP_MINUS_PLUS(Point2, NormVec2)
	//S2D_OP_MINUS_PLUS(Point2, Point2)
	//S2D_OP_MINUS_PLUS(Point2, Dim2)

	//S2D_OP_MINUS_PLUS(Vec2, Vec2)
	//S2D_OP_MINUS_PLUS(Vec2, NormVec2)

	//S2D_OP_MINUS_PLUS_RET_VEC(NormVec2, Vec2)
	//S2D_OP_MINUS_PLUS_RET_VEC(NormVec2, NormVec2)

	//S2D_OP_MINUS_PLUS(Dim2, Vec2)
	//S2D_OP_MINUS_PLUS(Dim2, NormVec2)
	//S2D_OP_MINUS_PLUS(Dim2, Dim2)

	//S2D_OP_SCALE(Point2)
	//S2D_OP_SCALE(Vec2)
	//S2D_OP_SCALE_RET_VEC(NormVec2)
	//S2D_OP_SCALE(Dim2)

	//S2D_OP_DOT(Vec2, Vec2)
	//S2D_OP_DOT(Vec2, NormVec2)
	//S2D_OP_DOT(NormVec2, NormVec2)
	//S2D_OP_DOT(NormVec2, Vec2)

    //S2D_OPS_ITR(Rect2, Vec2, +)
    //S2D_OPS_ITR(Rect2, Vec2, -)
    //S2D_OPS_ITR(Rect2, NormVec2, +)
    //S2D_OPS_ITR(Rect2, NormVec2, -)

    //S2D_OPS_ITR(Poly2, Vec2, +)
    //S2D_OPS_ITR(Poly2, Vec2, -)
    //S2D_OPS_ITR(Poly2, NormVec2, +)
    //S2D_OPS_ITR(Poly2, NormVec2, -)


#undef S2D_OP_MINUS_PLUS
#undef S2D_OP_MINUS_PLUS_RET_VEC

#undef S2D_OP_DOT
#undef S2D_OP_SCALE
#undef S2D_OP_SCALE_RET_VEC
#undef S2D_OP_PERP
#undef S2D_OP_ARR



}
