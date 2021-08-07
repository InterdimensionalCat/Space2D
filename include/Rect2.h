#pragma once
#include "AngularType.h"
#include "S2DIterator.h"


#ifndef S2D_RECT_2D_OPERATOR
#define S2D_VEC_2D_OPERATOR
#define S2D_VEC_2D_OP_EQ(op, typ2d) \
    constexpr inline Rect2& operator##op(const typ2d<T>& rhs) noexcept {\
    min op rhs;\
    max op rhs;\
    return (*this);\
	}

#define S2D_VEC_2D_OP(op, typ2d) \
	constexpr inline Rect2 operator##op(const typ2d<T>& rhs) const noexcept { \
    return Rect2(min op rhs, max op rhs);\
	}
#endif

namespace Space2D {

    /**
     * @brief Enumeration specifiying each face of a Rect2
    */
    enum class RectFace {
        Up,
        Down,
        Left,
        Right
    };


    template<typename T>
    class Vec2;
    template<typename T>
    class NormVec2;
    template<typename T>
    class Dim2;
    template<typename T>
    class Point2;


    /**
	 * @brief Class encapsulating a 2 Dimensional Axis Aligned Rectangle representation
     * @details unlike some other Rectangle representations, Rect2 defines an AABB as a maximum and minimum
     * point (where minimum is top left)
	 * @tparam T Underlying data type of the coordinates for each point
	*/
    template<typename T>
    class Rect2 
    {

    public:

        /**
         * @brief Constructs a Rect2 with min (0,0) and max (1,1)
         * 
        */
        Rect2() noexcept : min(), max(1, 1) {}

        /**
         * @brief Constructs a Rec2 with min (x0, y0) and max (x1, y1)
         * @param x0 The minimum x coordinate
         * @param y0 The minimum y coordinate
         * @param x1 The maximum x coordinate
         * @param y1 The maximum y coordinate
        */
        constexpr explicit Rect2(const T& x0, const T& y0, const T& x1, const T& y1) noexcept
            : min(Point2<T>(x0, y0)), max(Point2<T>(x1, y1)) {}

        /**
         * @brief Constructs a Rec2 with the supplied min and max points
         * @param min The minimum point
         * @param max The maximum point
        */
        constexpr explicit Rect2(const Point2<T>& min, const Point2<T>& max) noexcept
            : min(min), max(max) {}

        /**
         * @brief Constructs a Rec2 with the supplied top left corner and size
         * @param min the minimum point
         * @param dim the size of the rect
        */
        constexpr explicit Rect2(const Point2<T>& min, const Dim2<T>& dim) noexcept
            : min(min), max(Point2<T>(min.x + dim.x, min.y + dim.y)) {}


        /**
         * @brief Constructs a Rec2 as a sub area of another Rec2
         * @details Constructs a Rec2 as a sub area of another Rec2 such that
         * 
         *         _____________
         *        |             |
         *        |x0,y0__      |
         *        |  |   |      |
         *        |  |___x1, y1 |
         *        |_____________|
         * 
         * 
         * 
         * where (x0, y0) is baseRect.min + minOffset
         * and (x1, y1) is baseRect.max - maxOffset, and
         * the new rectangle is described by (x0, y0) and (x1, y1)
         * 
         * @param baseRect the base Rect2 to construct from
         * @param minOffset the offset of the minimum point from the baseRect
         * @param maxOffset the offset of the maximum point from the baseRect,
         * note that positive values will result in a negative change in the maximum point, as
         * the maxOffset describes how much the max point should shrink
        */
        constexpr explicit Rect2(const Rect2& baseRect,
            const Vec2<T>& minOffset, const Vec2<T>& maxOffset) noexcept
            : min(baseRect.min + minOffset), max(baseRect.max - maxOffset) {}

        /**
         * @brief Constructs a Rec2 as a sub area of another Rec2
         * @details Constructs a Rec2 as a sub area of another Rec2 such that
         * 
         *         _____________
         *        |             |
         *        |x0,y0__      |
         *        |  |   |      |
         *        |  |___x1, y1 |
         *        |_____________|
         * 
         * 
         * 
         * the new rectangle is described by (x0, y0) and (x1, y1), the difference between
         * this constructor and the one above is that this constructor offsets by a percentage of the
         * width and height for each corner, so a min and max offset of 10% in both directions results in a Rec2
         * with the same center as the base rect but with 80% of the size
         * 
         * @param baseRect the base Rect2 to construct from
         * @param minOffset the offset of the minimum point from the baseRect
         * @param maxOffset the offset of the maximum point from the baseRect,
         * note that positive values will result in a negative change in the maximum point, as
         * the maxOffset describes how much the max point should shrink
        */
        constexpr explicit Rect2(const Rect2<T>& baseRect, 
            const Dim2<Percent>& minOffset, const Dim2<Percent>& maxOffset) noexcept :
            min(baseRect.min + baseRect.size() * ((Dim2<T>)minOffset)),
            max(baseRect.max - baseRect.size() * ((Dim2<T>)maxOffset))
        {}

        /**
         * @brief Comparison operators for Rect2, sorted by min then max
         * @param other The Rect2 to compare with
         *
        */
        constexpr auto operator<=>(const Rect2&) const noexcept = default;

        /**
		 * @brief Equality operator for Rect2
		 * @param other The Rect2 to compare with
		 * @return true if the max and min 
         * points of the two Rect2's are equal
		*/
        constexpr bool operator==(const Rect2& other) const noexcept {
            return min == other.min && max == other.max;
        }

        /**
		 * @brief Access the Rect2 points as if it were an array
		 * @param i The index, 0 = min and 1 = max
		 * @return a read only reference to the point
		*/
        constexpr const Point2<T>& operator[] (const size_t i) const {
            if (i > 1) throw std::out_of_range("Rect2 subscript out of range");
            return i == 0 ? min : max;
        }

        /**
		 * @brief Access the Rect2 points as if it were an array
		 * @param i The index, 0 = min and 1 = max
		 * @return a read and write reference to the point
		*/
        constexpr Point2<T>& operator[] (const size_t i) {
            if (i > 1) throw std::out_of_range("Rect2 subscript out of range");
            return i == 0 ? min : max;
        }

        /**
         * @brief Read and write iterator for Rect2
        */
        struct Iterator {

            Iterator(const size_t index, Rect2* val) : index(index), val(val) {}

            Point2<T>& operator*() {
                return (*val)[index];
            }
            bool operator==(const Iterator& other) {
                return index == other.index && val == other.val;
            }
            bool operator!=(const Iterator& other) {
                return !(*this == other);
            }
            Iterator& operator++() {
                ++index;
                return *this;
            }

            Iterator& operator++(int) {
                index++;
                return *this;
            }

        private:
            size_t index;
            Rect2* val;
        };

        /**
         * @brief Read only iterator for Rect2
        */
        struct CIterator {

            CIterator(const size_t index, const Rect2* val) : index(index), val(val) {}

            const Point2<T>& operator*() const {
                return (*val)[index];
            }

            bool operator==(const CIterator& other) const {
                return index == other.index && val == other.val;
            }
            bool operator!=(const CIterator& other) const {
                return !(*this == other);
            }
            CIterator& operator++() {
                ++index;
                return *this;
            }

            CIterator& operator++(int) {
                index++;
                return *this;
            }

        private:
            size_t index;
            const Rect2* val;
        };

        /**
         * @brief Beginning of the Iterator range
         * @return A const iterator at the beginning of the Rect2
        */
        const CIterator cbegin() const {
            return CIterator(0, this);
        }

        /**
         * @brief One past the end of the Iterator range
         * @return A const iterator one past the end of the Rect2
        */
        const CIterator cend() const {
            return CIterator(2, this);
        }

        /**
         * @brief Beginning of the Iterator range
         * @return An iterator at the beginning of the Rect2
        */
        Iterator begin() {
            return Iterator(0, this);
        }

        /**
         * @brief One past the end of the Iterator range
         * @return An iterator one past the end of the Rect2
        */
        Iterator end() {
            return Iterator(2, this);
        }

        /**
         * @brief computes the area of the Rect2
         * @return the computed area
        */
        constexpr T area() const noexcept {
            return width() * height();
        }

        /**
         * @brief computes the center of the Rect2
         * @return the computer center
        */
        constexpr Point2<T> center() const noexcept {
            return Point2<T>((max.x + min.x) * (T)0.5, (max.y + min.y) * (T)0.5);
        }

        /**
         * @brief determines if a point is within the Rect2 or not
         * @details determines if a point is within the Rect2 or not, 
         * returns false if the point lies on one of the faces or corners of the Rect2
         * @param query the Point2 to check
         * @return true if the query Point2 is within the Rect2
        */
        constexpr bool contains(const Point2<T>& query) const noexcept {
            return (query.x > min.x) &&
                (query.x < max.x) &&
                (query.y > min.y) &&
                (query.y < max.y);
        }

        /**
         * @brief determines if a point is within the Rect2 or not
         * @details determines if a point is within the Rect2 or not, 
         * @param qx the x coordinate of the query Point2
         * @param qy the y coordinate of the query Point2
         * @return true if the query Point2 is within the Rect2
        */
        constexpr bool contains(const T& qx, const T& qy) const noexcept {
            return contains(Point2<T>(qx, qy));
        }

        /**
         * @brief Computes the width and height of the Rect2
         * @return the width and height of the Rect2
        */
        constexpr Dim2<T> size() const noexcept {
            return Dim2<T>(min, max);
        }

        /**
         * @brief Computes the width of the Rect2
         * @return the width of the Rect2
        */
        constexpr T width() const noexcept {
            return max.x - min.x;
        }

        /**
         * @brief Computes the height of the Rect2
         * @return the height of the Rect2
        */
        constexpr T height() const noexcept {
            return max.y - min.y;
        }

        /**
         * @brief Moves the Rect2 to a new min point
         * @details Moves the Rect2 to a new min point such that
         * the size of the Rect2 is the same
         * @param newMin 
        */
        constexpr void moveMinTo(const Point2<T>& newMin) noexcept {
            auto size = Rect2::size();
            min = newMin;
            max = newMin + Vec2<T>(size.x, size.y);
        }

        /**
         * @brief Moves the Rect2 to a new min point
         * @details Moves the Rect2 to a new min point such that
         * the size of the Rect2 is the same
         * @param mx the x coordinate of the new min Point2
         * @param my the y coordinate of the new min Point2
        */
        constexpr void moveMinTo(const T& mx, const T& my) noexcept {
            moveMinTo(Point2<T>(mx, my));
        }

        /**
         * @brief Moves the Rect2 to a new center
         * @details Moves the Rect2 to a new center point such that
         * the size of the Rect2 is the same
         * @param newCenter 
        */
        constexpr void moveCenterTo(const Point2<T>& newCenter) noexcept {
            auto size = Rect2::size();
            min = newCenter - (size / 2.0f);
            max = newCenter + (size / 2.0f);
        }

        /**
         * @brief Moves the Rect2 to a new center
         * @details Moves the Rect2 to a new center point such that
         * the size of the Rect2 is the same
         * @param cx the x coordinate of the new center Point2
        */
        constexpr void moveCenterTo(const T& cx, const T& cy) noexcept {
            moveCenterTo(Point2<T>(cx, cy));
        }

        /**
         * @brief Get the NormVec2 of the supplied face of the Rect2
         * @details Get the NormVec2 of the supplied face of the Rect2, although this is
         * fixed since Rect2 is an Axis Aligned Bounding Box (AABB) it is provided for convenience
         * @param type The face to get the NormVec2 of
         * @return The NormVec2 of the supplied face
        */
        constexpr NormVec2<T> getFaceNormal(const RectFace type) const {
            switch (type) {
            case RectFace::Up:
                return getNormal_0_neg1();
            case RectFace::Down:
                return getNormal_0_1();
            case RectFace::Left:
                return getNormal_neg1_0();
            case RectFace::Right:
                return getNormal_1_0();
            default:
                return getNormal_0_1();
            }

        }


        /**
         * @brief Get the two points that make up the supplied face of the Rect2
         * @param type The face to get the points of
         * @return The points of the supplied face
         * @todo Make a class representing both lines and line segments to make Rect2 and
         * Poly2 faces more robust and functional
        */
        constexpr std::array<Point2<T>, 2> getFacePoints(const RectFace type) const noexcept {

            switch (type) {
            case RectFace::Up:
                //min + max.x, min.y
                return std::array<Point2<T>, 2>({ min, Point2<T>(max.x, min.y) });
            case RectFace::Down:
                //min.x, max.y + max
                return std::array<Point2<T>, 2>({ Point2<T>(min.x, max.y), max });
            case RectFace::Left:
                //min + min.x, max.y
                return std::array<Point2<T>, 2>({ min, Point2<T>(min.x, max.y) });
            case RectFace::Right:
                //max.x, min.y + max
                return std::array<Point2<T>, 2>({ Point2<T>(max.x, min.y), max });
            default:
                return std::array<Point2<T>, 2>({ Point2<T>(min.x, max.y), max });
            }
        }



        /**
         * @brief Get a Vec2 of the points in the supplied face in the COUNTERCLOCKWISE DIRECTION
         * @param type The face to get the points of
         * @return The Vec2 representing the supplied face
        */
        constexpr Vec2<T> getFaceVec(const RectFace type) const noexcept {
            //TODO: FINISH THIS
            //maybe make a face class??
            //make a getFaceVec function

            switch (type) {
            case RectFace::Up:
                //min + max.x, min.y
                return Vec2<T>(Point2<T>(max.x, min.y), min);
            case RectFace::Down:
                //min.x, max.y + max
                return Vec2<T>(Point2<T>(min.x, max.y), max);
            case RectFace::Left:
                //min + min.x, max.y
                return Vec2<T>(min, Point2<T>(min.x, max.y));
            case RectFace::Right:
                //max.x, min.y + max
                return Vec2<T>(max, Point2<T>(max.x, min.y));
            default:
                return Vec2<T>(Point2<T>(min.x, max.y), max);
            }
        }

        constexpr NormVec2<T> getNormal_1_0() const noexcept {
            return NormVec2<T>(static_cast<T>(1), static_cast<T>(0));
        }

        constexpr NormVec2<T> getNormal_neg1_0() const noexcept {
            return NormVec2<T>(static_cast<T>(-1), static_cast<T>(0));
        }

        constexpr NormVec2<T> getNormal_0_1() const noexcept {
            return NormVec2<T>(static_cast<T>(0), static_cast<T>(1));
        }

        constexpr NormVec2<T> getNormal_0_neg1() const noexcept {
            return NormVec2<T>(static_cast<T>(0), static_cast<T>(-1));
        }

        /**
         * @brief Implicit conversion to other Rect2 types
         * @tparam Other the underlying data type of the Rect2 to convert to
        */
        template<typename Other>
        explicit operator Rect2<Other>() const {
            return Rect2<Other>((Point2<Other>)min, (Point2<Other>)max);
        }

#ifdef _SFML_ENABLED


        /**
		 * @brief Implicit conversion function to an sf::Rect2
		 * @tparam SFMLType the template param for the desired sf::Rect2
		 * @return The sf::Rect2 equivalent of this Rect2
		*/
		template<typename SFMLType>
		constexpr operator sf::Rect<SFMLType>() const noexcept {
			return toSFMLRect<SFMLType>();
		}

        /**
         * @brief Explicit conversion function to an sf::Rect2
         * @tparam SFMLType the template param for the desired sf::Rect2
         * @return The sf::Rect2 equivalent of this Rect2
        */
        template <typename SFMLType = float>
        sf::Rect<SFMLType> toSFMLRect() const noexcept {
            sf::Vector2<SFMLType> v1 = sf::Vector2<SFMLType>(static_cast<SFMLType>(min.x), static_cast<SFMLType>(min.y));
            sf::Vector2<SFMLType> v2 = sf::Vector2<SFMLType>(static_cast<SFMLType>(width()), static_cast<SFMLType>(height()));
            return sf::Rect<SFMLType>(v1, v2);
        }
        
        //
        //however, section that is the "interior color" will never get clipped due to a large border size


        /**
         * @brief Creates a sf::RectangleShape (a drawable rectangle)
         * @param borderSize the size of the sf::RectangleShape border
         * border size strictly adds to the size, so the RectangleShape 
         * will only retain the same dimensions as the s2d::Rect if border is 0.0f,
         * however the interior area(and interior color) will 
         * always be the same dimensions as the original Rect2
         * @param interiorColor the interior color of the sf::RectangleShape
         * @param borderColor the border color of the sf::RectangleShape
         * @return the sf::RectangleShape
        */
        sf::RectangleShape makeDrawableSFMLRect(
            const float borderSize = 0.0f, 
            const sf::Color interiorColor = sf::Color(0,0,0,0), 
            const sf::Color borderColor = sf::Color(0, 0, 0, 0)
        ) const noexcept {
            sf::RectangleShape newshape;
            newshape.setOutlineThickness(borderSize);
            newshape.setOutlineColor(borderColor);
            newshape.setFillColor(interiorColor);
            sf::Vector2f v1 = sf::Vector2f(static_cast<float>(min.x), static_cast<float>(min.y));
            sf::Vector2f v2 = sf::Vector2f(static_cast<float>(width()), static_cast<float>(height()));
            newshape.setPosition(v1);
            newshape.setSize(v2);
            return newshape;
        }
#endif

        /**
		 * @brief Prints the Rect2
		 * @param os Input stream
		 * @param it The Rect2 to print
		 * @return a reference to the stream for << chaining
		*/
        friend std::ostream& operator<<(std::ostream& os, const Rect2<T>& it) {

            std::string typname = typeid(T).name();
            std::string::size_type i = typname.find("struct");
            if (i != std::string::npos) {
                typname.erase(i, 7);
            }
            i = typname.find("class");
            if (i != std::string::npos) {
                typname.erase(i, 6);
            }

            os  << "Rect2<" << typname << ">(min: (" << it.min.x <<", " <<it.min.y << "), max: (" << it.max.x << ", " << it.max.y << "))";
            return os;
        }

        /**
		 * @brief addition-eq for Rect2 with Vec2
		 * @param rhs the addition Vec2
		*/
        S2D_VEC_2D_OP_EQ(+=, Vec2);

         /**
		 * @brief subtract-eq for Rect2 with Vec2
		 * @param rhs the subtraction Vec2
		*/
        S2D_VEC_2D_OP_EQ(-=, Vec2);

        /**
		 * @brief addition-eq for Rect2 with NormVec2
		 * @param rhs the subtraction NormVec2
		*/
        S2D_VEC_2D_OP_EQ(+=, NormVec2);

        /**
		 * @brief subtract-eq for Rect2 with NormVec2
		 * @param rhs the subtraction NormVec2
		*/
        S2D_VEC_2D_OP_EQ(-=, NormVec2);

        /**
		 * @brief addition for Rect2 with Vec2
		 * @param rhs the addition Vec2
		*/
        S2D_VEC_2D_OP(+, Vec2);

         /**
		 * @brief subtract for Rect2 with Vec2
		 * @param rhs the subtraction Vec2
		*/
        S2D_VEC_2D_OP(-, Vec2);

        /**
		 * @brief addition for Rect2 with NormVec2
		 * @param rhs the subtraction NormVec2
		*/
        S2D_VEC_2D_OP(+, NormVec2);

        /**
		 * @brief subtract for Rect2 with NormVec2
		 * @param rhs the subtraction NormVec2
		*/
        S2D_VEC_2D_OP(-, NormVec2);

        /**
         * @brief The min point of the Rect2
        */
        Point2<T> min;

        /**
         * @brief The max point of the Rect2
        */
        Point2<T> max;
    };
}

