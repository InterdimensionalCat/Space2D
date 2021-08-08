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

    /**
	 * @brief Class encapsulating a 3x3 Matrix, intended to be used for Linear/Affine transformations
     * @details  Class encapsulating a 3x3 Matrix, intended to be used for Linear/Affine transformations,
     * uses a size 9 single array to represent the matrix, here is the array mapping:
     *
     *       [a b tx]
     *       [c d ty]
     *       [0 0 1 ]
     *       
     *       [0 3 6]
     *       [1 4 7]
     *       [2 5 8]
     * 
	 * @tparam T Underlying data type of the coordinates
	*/
    template<typename T>
    class Mat3
    {



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

        /**
         * @brief Constructs an identity matrix
        */
        constexpr Mat3() noexcept : matrix{ 
            (T)1.0, (T)0.0, (T)0.0, 
            (T)0.0, (T)1.0, (T)0.0, 
            (T)0.0, (T)0.0, (T)1.0 
        } {}

        /**
         * @brief Constructs a matrix with all positions filled with the specified value
         * @param fillValue the value to fill the matrix with
         * @return 
        */
        constexpr explicit Mat3(const T& fillValue) noexcept : matrix{
    fillValue, fillValue, fillValue,
    fillValue, fillValue, fillValue,
    fillValue, fillValue, fillValue
        } {}


        /**
         * @brief Constructs a matrix where each element is specified manually
         * @param a00 value 00
         * @param a01 value 01
         * @param a02 value 02
         * @param a10 value 10
         * @param a11 value 11
         * @param a12 value 12
         * @param a20 value 20, normally not used for linear transformations
         * @param a21 value 21, normally not used for linear transformations
         * @param a22 value 22, normally not used for linear transformations
         * @return 
        */
        constexpr explicit Mat3(
            const T& a00, const T& a01, const T& a02,
            const T& a10, const T& a11, const T& a12,
            const T& a20 = 0, const T& a21 = 0, const T& a22 = 1
        ) noexcept : matrix{ 
            a00, a10, a20, 
            a01, a11, a21, 
            a02, a12, a22 } 
        {}

        /**
         * @brief Transforms the supplied Point2
         * @details all transformations are applied to points
         * @param p the Point2 to transform
         * @return the transformed Point2
        */
        constexpr Point2<T> transform(const Point2<T>& p) const noexcept {

            auto px = _a * p.x + _b * p.y + _tx;
            auto py = _c * p.x + _d * p.y + _ty;

            return Point2<T>(
                _a * p.x + _b * p.y + _tx,
                _c * p.x + _d * p.y + _ty
            );
        }

        /**
         * @brief Transforms the supplied Vec2
         * @details transformations excluding translations are applied to vectors
         * @param v the Vec2 to transform
         * @return the transformed Vec2
        */
        constexpr Vec2<T> transform(const Vec2<T>& v) const noexcept {
            return Vec2<T>(
                _a * v.x + _b * v.y,
                _c * v.x + _d * v.y
            );
        }

        /**
         * @brief Transforms the supplied NormVec2
         * @details transformations excluding translations are applied to normal vectors,
         * but the normal vector invariants will still hold, which can affect scaling in
         * strange ways
         * @param v the NormVec2 to transform
         * @return the transfomred NormVec2
        */
        constexpr NormVec2<T> transform(const NormVec2<T>& v) const noexcept {
            return NormVec2<T>(
                _a * v.x + _b * v.y,
                _c * v.x + _d * v.y
            );
        }

        /**
         * @brief Transformes the supplied Dim2
         * @details all transformations excluding translations are applied to Dimensions
         * @param d the Dim2 to transform
         * @return the transformed Dim2
        */
        constexpr Dim2<T> transform(const Dim2<T>& d) const noexcept {
            return Dim2<T>(
                _a * d.x + _b * d.y,
                _c * d.x + _d * d.y
            );
        }

        /**
         * @brief Transforms the supplied Rect2
         * @details all transformations are applied to Rectangles,
         * but they maintained their axis aligned properties
         * @param r The Rect2 to transform
         * @return the transformed Rect2
        */
        constexpr Rect2<T> transform(const Rect2<T>& r) {
            return Rect2<T>(
                transform(r.min),
                transform(r.max)
            );
        }

        /**
         * @brief Transforms the supplied Poly2
         * @details all transformations are applied to Polygons
         * @param p the Poly2 to transform
         * @return the transformed Poly2
        */
        constexpr Poly2<T> transform(const Poly2<T>& p) const noexcept {
            std::vector<Point2<T>> points;
            std::for_each(p.cbegin(), p.cend(), [this, &points](const auto& point) {
                    points.push_back(transform(point));
                }
            );
            return Poly2<T>(points);
        }


        /**
         * @brief computes the inverse matrix
         * @return the inverted matrix
        */
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

        /**
         * @brief multiplies two matrices
         * @param rhs the other Mat3 to multiply with
         * @return the multiplied Mat3
        */
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

        /**
         * @brief multipliy-eq's two matrices
         * @param rhs the other Mat3 to multiply with
         * @return a reference to the Mat3 for chaining
        */
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

        /**
         * @brief scalar multiplies the matrix
         * @param rhs the scalar to multiply with
         * @return the multiplied Mat3
        */
        constexpr Mat3 operator*(const T& rhs) const noexcept {

            return Mat3(  _a * rhs,   _b * rhs,  _tx * rhs, 
                          _c * rhs,   _d * rhs,  _ty * rhs,
                        _a20 * rhs, _a21 * rhs, _a22 * rhs
            );
        }

        /**
         * @brief scalar multipliy-eq's the matrix
         * @param rhs the scalar to multiply with
         * @return a reference to the Mat3 for chaining
        */
        constexpr Mat3& operator*=(const T& rhs) noexcept {

            (*this) =  Mat3(  _a * rhs,   _b * rhs,  _tx * rhs, 
                              _c * rhs,   _d * rhs,  _ty * rhs,
                            _a20 * rhs, _a21 * rhs, _a22 * rhs
            );
            return *this;
        }

        /**
         * @brief translate the Matrix
         * @param translationVec the Vec2 to translate by
         * @return the transformed Matrix
        */
        constexpr Mat3& translate(const Vec2<T>& translationVec) noexcept {
            auto transVecInv = this->inverse().transform(translationVec);

            return ((*this) *= Mat3(
                1, 0, transVecInv.x,
                0, 1, transVecInv.y,
                0, 0, 1
            ));
        }

        /**
         * @brief rotate the Matrix
         * @param rad the radian value of the rotation
         * @param center optionally set the center of the transformation
         * @return the transformed matrix
        */
        constexpr Mat3& rotate(const Radians rad, const Point2<T>& center = Point2<T>()) noexcept {
            T cosval = (T)cos(rad);
            T sinval = (T)sin(rad);

            return ((*this) *= Mat3(
                 cosval, -sinval, center.x * (1 - cosval) - center.y * sinval,
                 sinval,  cosval, center.y * (1 - cosval) + center.x * sinval
            ));
        }

        /**
         * @brief scale the matrix
         * @param sx the x scale factor
         * @param sy the y scale factor
         * @param center optionally set the center of the transformation
         * @return the transformed matrix
        */
        constexpr Mat3& scale(const T sx, const T sy, const Point2<T>& center = Point2<T>()) noexcept {
            return ((*this) *= Mat3(
                sx,  0, center.x * (1 - sx),
                 0, sy, center.y * (1 - sy)
            ));
        }


        /**
         * @brief shear the matrix
         * @details ex:
         * 
         *           _________           _________
         *           |        |         \         \
         *           |        |     ->   \         \
         *           |        |     ->    \         \
         *           |________|            \_________\  
         * 
         * @param sx the x scale factor
         * @param sy the y scale factor
         * @param center optionally set the center of the transformation
         * @return the transformed matrix
        */
        constexpr Mat3& shear(const T sx, const T sy, Point2<T> center = Point2<T>()) noexcept {
            center *= -1;

            return((*this) *= Mat3(
                 1, sx, -(center.y * sx),
                sy,  1,   center.x * sy
            ));
        }

        /**
         * @brief reflect the matrix across the x axis
         * @return the transformed matrix
        */
        constexpr Mat3& reflX() noexcept {
            return linRefl(Vec2<T>(1, 0));
        }

        /**
         * @brief reflect the matrix across the y axis
         * @return the transformed matrix
        */
        constexpr Mat3& reflY() noexcept {
            return linRefl(Vec2<T>(0, 1));
        }

        /**
         * @brief reflect the matrix across an arbitrary axis
         * @param lineSlope the slope of the transformation axis
         * @param intercept optionally set an intercept point of the axis
         * @return the transformed matrix
        */
        constexpr Mat3& linRefl(const Vec2<T>& lineSlope, const Point2<T>& intercept = Point2<T>()) noexcept {
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
            ) * (1 / lineSlope.magSquared()));


            return *this;
        }

        /**
         * @brief orthagonally project the matrix onto an arbitrary axis
         * @param lineSlope the slope of the transformation axis
         * @param intercept optionally set an intercept point of the axis
         * @return the transformed matrix
        */
        constexpr Mat3& orthProj(const Vec2<T>& lineSlope, const Point2<T>& intercept = Point2<T>()) noexcept {
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

        /**
         * @brief returns a const reference to the raw matrix array
         * @return the refernce to the array
        */
        inline constexpr const std::array<T, 9>& getMatrix() const noexcept {
            return matrix;
        }

        /**
         * @brief returns a reference to the raw matrix array
         * @return the refernce to the array
        */
        inline constexpr std::array<T, 9>& getMatrix() noexcept {
            return matrix;
        }

#ifdef _SFML_ENABLED

        /**
         * @brief convert this Mat3 to an sf::Transform
         * @return the converted sf::Transform
        */
        constexpr sf::Transform toSFMLMatrix() const noexcept {
            return sf::Transform(
                static_cast<float>(matrix[0]), static_cast<float>(matrix[3]), static_cast<float>(matrix[6]),
                static_cast<float>(matrix[1]), static_cast<float>(matrix[4]), static_cast<float>(matrix[7]),
                static_cast<float>(matrix[2]), static_cast<float>(matrix[5]), static_cast<float>(matrix[8])
            );
        }
#endif

        /**
         * @brief Prints the Mat3
         * @param os Input stream
         * @param it The Mat2 to print
         * @return a reference to the stream for << chaining
        */
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

        /**
		 * @brief Equality operator for Mat3
		 * @param other The Mat3 to compare with
		 * @return true if the two Mat3's are equal
		*/
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

        /**
         * @brief Comparison operators for Mat3, sorted by matrix array order
         * @param other The Mat3 to compare with
        */
        constexpr auto operator<=>(const Mat3<T>& other) const noexcept {
            size_t i = 0;
            size_t maxind = 9;
            for (; i < matrix.size(); i++) {
                if (matrix[i] != other.matrix[i]) {
                    return matrix[i] <=> other.matrix[i];
                }
            }

            return matrix[i] <=> other.matrix[i];

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