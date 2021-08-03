#pragma once
#include <vector>
#include <array>
#include <algorithm>
#include "S2DMath.h"
#include "AngularType.h"

namespace Space2D {

    template<typename T>
    class Vec2;
    template<typename T>
    class NormVec2;
    template<typename T>
    class Dim2;
    template<typename T>
    class Point2;
    template<typename T>
    class Rect2;
    template<typename T>
    class Poly2;

    template<typename T>
    class Mat3
    {

        //matrix form:
        //[a b tx]
        //[c d ty]
        //[0 0 1 ]

        //[0 3 6]
        //[1 4 7]
        //[2 5 8]

    private:
        std::array<T, 9> matrix;

#define _a  Mat3::matrix[0]
#define _b  Mat3::matrix[3]
#define _tx Mat3::matrix[6]

#define _c  Mat3::matrix[1]
#define _d  Mat3::matrix[4]
#define _ty Mat3::matrix[7]

#define _a20 Mat3::matrix[2]
#define _a21 Mat3::matrix[5]
#define _a22 Mat3::matrix[8]


    public:

        constexpr Mat3() noexcept : matrix{ 
            (T)1.0, (T)0.0, (T)0.0, 
            (T)0.0, (T)1.0, (T)0.0, 
            (T)0.0, (T)0.0, (T)1.0 
        } {}

        constexpr explicit Mat3(const T& fillValue) noexcept : matrix{
    fillValue, fillValue, fillValue,
    fillValue, fillValue, fillValue,
    fillValue, fillValue, fillValue
        } {}


        constexpr explicit Mat3(
            const float& a00, const float& a01, const float& a02,
            const float& a10, const float& a11, const float& a12,
            const float& a20 = 0, const float& a21 = 0, const float& a22 = 1
        ) noexcept : matrix{ 
            a00, a10, a20, 
            a01, a11, a21, 
            a02, a12, a22 } 
        {}

        //all transformations are applied on points
        constexpr Point2<T> transformPoint(const Point2<T>& p) const noexcept {
            return Point2<T>(
                _a * p.x + _b * p.y + _tx,
                _c * p.x + _d * p.y + _ty
            );
        }

        //all transformations excluding translations are applied on vectors
        constexpr Vec2<T> transformVec(const Vec2<T>& v) const noexcept {
            return Vec2<T>(
                _a * v.x + _b * v.y,
                _c * v.x + _d * v.y
            );
        }

        //all transformations excluding translations are applied on vectors,
        //but their magnitude will remain 1 no matter what
        constexpr NormVec2<T> transformNormVec(const NormVec2<T>& v) const noexcept {
            return NormVec2<T>(
                _a * v.x + _b * v.y,
                _c * v.x + _d * v.y
            );
        }

        //all transformations excluding translations are applied on Dimensions
        constexpr Dim2<T> transformDim(const Dim2<T>& d) const noexcept {
            return Dim2<T>(
                _a * d.x + _b * d.y,
                _c * d.x + _d * d.y
            );
        }

        //all transformations are applied on Rectangles
        //however they maintain their axis-aligned properties,
        //as a new rect is created with the two individually transformed points
        constexpr Rect2<T> transformRect(const Rect2<T>& r) {
            return Rect2<T>(
                transformPoint(r.min),
                transformPoint(r.max)
            );
        }

        //all transformations are applied to Polygons
        //as each point is transformed individually
        constexpr Poly2<T> transformPoly(const Poly2<T>& p) const noexcept {
            std::vector<Point2<T>> points;
            std::for_each(p.begin(), p.end(), [&points](const auto& point) {
                    points.push_back(transformPoint(point));
                }
            );
            return Poly2<T>(points);
        }

        constexpr Mat3 inverse() const noexcept {
            T determinant = 
                  _a  * (_d * _a22 - _ty * _a21)
                - _b  * (_c * _a22 - _ty * _a20)
                + _tx * (_c * _a21 - _d  * _a20);


            if (determinant != 0.0f)
            {
                //[0 3 6]
                //[1 4 7]
                //[2 5 8]

                //[0 1 2]
                //[3 4 5]
                //[6 7 8]

                return Mat3(
                    (_d * _a22 - _a21 * _ty) / determinant,
                   -(_b * _a22 - _a21 * _tx) / determinant,
                    (_b * _ty  - _d   * _tx) / determinant,

                   -(_c * _a22 - _a20 * _ty) / determinant,
                    (_a * _a22 - _a20 * _tx) / determinant,
                   -(_a * _ty  - _c   * _tx) / determinant,

                    (_c * _a21 - _a20 * _d) / determinant,
                   -(_a * _a21 - _a20 * _b) / determinant,
                    (_a * _d   - _c   * _b) / determinant
                );
            }
            else
            {
                return Mat3();
            }
        }

        constexpr Mat3 operator*(const Mat3& rhs) const noexcept {

            //[0 3 6]
            //[1 4 7]
            //[2 5 8]

            return Mat3(
                (matrix[0] * rhs.matrix[0] + matrix[3] * rhs.matrix[1] + matrix[6] * rhs.matrix[2]),
                (matrix[0] * rhs.matrix[3] + matrix[3] * rhs.matrix[4] + matrix[6] * rhs.matrix[5]),
                (matrix[0] * rhs.matrix[6] + matrix[3] * rhs.matrix[7] + matrix[6] * rhs.matrix[8]),

                (matrix[1] * rhs.matrix[0] + matrix[4] * rhs.matrix[1] + matrix[7] * rhs.matrix[2]),
                (matrix[1] * rhs.matrix[3] + matrix[4] * rhs.matrix[4] + matrix[7] * rhs.matrix[5]),
                (matrix[1] * rhs.matrix[6] + matrix[4] * rhs.matrix[7] + matrix[7] * rhs.matrix[8]),

                (matrix[2] * rhs.matrix[0] + matrix[5] * rhs.matrix[1] + matrix[8] * rhs.matrix[2]),
                (matrix[2] * rhs.matrix[3] + matrix[5] * rhs.matrix[4] + matrix[8] * rhs.matrix[5]),
                (matrix[2] * rhs.matrix[6] + matrix[5] * rhs.matrix[7] + matrix[8] * rhs.matrix[8])
            );
        }

        constexpr Mat3& operator*=(const Mat3& rhs) noexcept {
            //[0 3 6]
            //[1 4 7]
            //[2 5 8]

            (*this) = Mat3(
                (matrix[0] * rhs.matrix[0] + matrix[3] * rhs.matrix[1] + matrix[6] * rhs.matrix[2]),
                (matrix[0] * rhs.matrix[3] + matrix[3] * rhs.matrix[4] + matrix[6] * rhs.matrix[5]),
                (matrix[0] * rhs.matrix[6] + matrix[3] * rhs.matrix[7] + matrix[6] * rhs.matrix[8]),

                (matrix[1] * rhs.matrix[0] + matrix[4] * rhs.matrix[1] + matrix[7] * rhs.matrix[2]),
                (matrix[1] * rhs.matrix[3] + matrix[4] * rhs.matrix[4] + matrix[7] * rhs.matrix[5]),
                (matrix[1] * rhs.matrix[6] + matrix[4] * rhs.matrix[7] + matrix[7] * rhs.matrix[8]),

                (matrix[2] * rhs.matrix[0] + matrix[5] * rhs.matrix[1] + matrix[8] * rhs.matrix[2]),
                (matrix[2] * rhs.matrix[3] + matrix[5] * rhs.matrix[4] + matrix[8] * rhs.matrix[5]),
                (matrix[2] * rhs.matrix[6] + matrix[5] * rhs.matrix[7] + matrix[8] * rhs.matrix[8])
            );
            return *this;
        }

        constexpr Mat3& translate(const Vec2<T>& translationVec) noexcept {
            auto transVecInv = this->inverse().transformVec(translationVec);

            return ((*this) *= Mat3(
                1, 0, transVecInv.x,
                0, 1, transVecInv.y,
                0, 0, 1
            ));
        }

        constexpr Mat3& rotate(const Radians rad) noexcept {
            T cosval = (T)cos<Radians>(rad);
            T sinval = (T)sin<Radians>(rad);

            return ((*this) *= Mat3(
                 cosval, sinval, 0,
                -sinval, cosval, 0
            ));
        }

        constexpr Mat3& rotate(const Radians rad, const Point2<T>& center) noexcept {
            T cosval = (T)cos<Radians>(rad);
            T sinval = (T)sin<Radians>(rad);

            return ((*this) *= Mat3(
                 cosval, sinval, center.x * (1 - cosval) - center.y * sinval,
                -sinval, cosval, center.y * (1 - cosval) + center.x * sinval
            ));
        }

        constexpr Mat3& scale(const Percent sx, const Percent sy) noexcept {
            return scale((T)sx, (T)sy);
        }

        constexpr Mat3& scale(const T sx, const T sy) noexcept {
            return ((*this) *= Mat3(
                sx,  0,  0,
                 0, sy,  0
            ));
        }

        constexpr Mat3& scale(const Percent sx, const Percent sy, const Point2<T>& center) noexcept {
            return scale((T)sx, (T)sy, center);
        }

        constexpr Mat3& scale(const T sx, const T sy, const Point2<T>& center) noexcept {
            return ((*this) *= Mat3(
                sx,  0, center.x * (1 - sx),
                 0, sy, center.y * (1 - sy)
            ));
        }

        constexpr Mat3& shear(const T sx, const T sy) noexcept {
            return((*this) *= Mat3(
                1, sx, 0,
                sy, 1, 0
            ));
        }

        constexpr Mat3& shear(const T sx, const T sy, Point2<T> center) noexcept {
            center *= -1;

            return((*this) *= Mat3(
                 1, sx, -(center.y * sx),
                sy,  1,   center.x * sy
            ));
        }

        constexpr Mat3& reflX() noexcept {
            return linearReflection(Vec2<T>(1, 0));
        }

        constexpr Mat3& reflY() noexcept {
            return linearReflection(Vec2<T>(0, 1));
        }

        constexpr Mat3& linRefl(const Vec2<T>& lineSlope) noexcept {
            auto a = -lineSlope.y;
            auto b =  lineSlope.x;

            (*this) *= (Mat3(
                b * b - a * a, -2 * a * b, 0,
                -2 * a * b, a * a - b * b, 0
            ) * (1 / lineSlope.MagSquared()));


            return *this;
        }

        constexpr Mat3& linRefl(const Vec2<T>& lineSlope, const Point2<T>& intercept) noexcept {
            auto a = -lineSlope.y;
            auto b =  lineSlope.x;
            T c; //the y intercept
            if (intercept.y == 0) {
                c = intercept.x;
            }
            else {
                auto m = a / b;
                auto x1 = intercept.x;
                auto y1 = intercept.y;
                c = (-y1 / m) + x1;
            }


            (*this) *= (Mat3(
                b * b - a * a, -2 * a * b, -2 * a * c,
                -2 * a * b, a * a - b * b, -2 * b * c
            ) * (1 / lineSlope.MagSquared()));


            return *this;
        }

        constexpr Mat3& orthProj(const Vec2<T>& lineSlope) noexcept {

            NormVec2<T> a = lineSlope.Normalize();

            (*this) *= (Mat3(
                a.x * a.x, a.x * a.y, 0,
                a.x * a.y, a.y * a.y, 0
            ));
            return *this;
        
        }

        constexpr Mat3& orthProj(const Vec2<T>& lineSlope, const Point2<T>& intercept) noexcept {
            NormVec2<T> v = lineSlope.Normalize();

            auto a = -lineSlope.y;
            auto b = lineSlope.x;
            T c; //the y intercept
            if (intercept.y == 0) {
                c = intercept.x;
            }
            else {
                auto m = a / b;
                auto x1 = intercept.x;
                auto y1 = intercept.y;
                c = (-y1 / m) + x1;
            }

            auto d = abs<T>(a * intercept.x + b * intercept.y + c) / sqrt<T>(a * a + b * b);

            auto transVal = lineSlope.unitNormal() * d;

            if (c > 0) {
                transVal *= -1;
            }

            (*this) *= (Mat3(
                v.x * v.x, v.x * v.y, transVal.x,
                v.x * v.y, v.y * v.y, transVal.y
            ));
            return *this;
        }

        inline constexpr const std::array<T, 9>& getMatrix() const noexcept {
            return matrix;
        }

        inline constexpr std::array<T, 9>& getMatrix() noexcept {
            return matrix;
        }

#ifdef _SFML_ENABLED

        constexpr sf::Transform toSFMLMatrix() const noexcept {
            return sf::Transform(
                static_cast<float>(matrix[0]), static_cast<float>(matrix[3]), static_cast<float>(matrix[6]),
                static_cast<float>(matrix[1]), static_cast<float>(matrix[4]), static_cast<float>(matrix[7]),
                static_cast<float>(matrix[2]), static_cast<float>(matrix[5]), static_cast<float>(matrix[8])
                );
        }
#endif

        friend std::ostream& operator << (std::ostream& os, const Mat3& it) {
            auto mat = it.getMatrix();

            std::string typname = typeid(T).name();
            std::string::size_type i = typname.find("struct");
            if (i != std::string::npos) {
                typname.erase(i, 7);
            }
            i = typname.find("class");
            if (i != std::string::npos) {
                typname.erase(i, 6);
            }

            os << "Mat3<" << typname << ">()\n";
            os << "[ " << mat[0] << ", " << mat[3] << ", " << mat[6] << "]\n";
            os << "[ " << mat[1] << ", " << mat[4] << ", " << mat[7] << "]\n";
            os << "[ " << mat[2] << ", " << mat[5] << ", " << mat[8] << "]\n";
            return os;
        }

        constexpr bool operator== (const Mat3<T>& other) const noexcept {
            return
                matrix[0] == other.matrix[0] &&
                matrix[1] == other.matrix[1] &&
                matrix[2] == other.matrix[2] &&
                matrix[3] == other.matrix[3] &&
                matrix[4] == other.matrix[4] &&
                matrix[5] == other.matrix[5] &&
                matrix[6] == other.matrix[6] &&
                matrix[7] == other.matrix[7] &&
                matrix[8] == other.matrix[8];
        }

        constexpr auto operator<=>(const Mat3<T>& other) const noexcept {
            size_t ind = 0;
            size_t maxind = 9;
            while (ind < maxind - 1 && matrix[ind] != other.matrix[ind]) {
                ind++;
            }
            return matrix[ind] <=> other.matrix[ind];
        }

#undef _a
#undef _b
#undef _tx

#undef _c
#undef _d
#undef _ty

#undef _a20
#undef _a21
#undef _a22
    };
}