#pragma once
#include <initializer_list>
#include "Space2D.h"
#include <vector>
#include <algorithm>
#include "S2DMath.h"
#include "S2DIterator.h"

#ifndef S2D_POLY_2D_OPERATOR
#define S2D_POLY_2D_OPERATOR
#define S2D_POLY_2D_OP_EQ(op, typ2d) \
    constexpr inline Poly2& operator##op(const typ2d<T>& rhs) noexcept {\
	std::for_each(begin(), end(), [&rhs](auto& a) {\
		    a op rhs;\
		}\
	);\
    return (*this);\
	}

#define S2D_POLY_2D_OP(op, typ2d) \
	constexpr inline Poly2 operator##op(const typ2d<T>& rhs) const noexcept { \
        std::vector<Point2<T>> points;\
        points.reserve(size());\
	    std::for_each(cbegin(), cend(), [&rhs, &points](auto& a) {\
                 points.push_back(a op rhs);\
		    }\
	    );\
        return Poly2(points);\
	}
#endif

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
    class Mat3;

    /**
     * @brief Class encapsulating a 2 Dimensional convex polygon representation
     * @details The polygon is checked for concavity at construction time, but can
     * be deformed into concavity through manually changing points
     * concavity is also checked when attempting to compute the centroid or the area, as
     * these functions require the polygon to be concave to work properly, they will throw if
     * the polygon is unexpectedly concave
     * @tparam T the underlying coordinate type of the Poly2
    */
    template<typename T>
    class Poly2
    {

    public:

        /**
         * @brief Constructs a Poly2 eqivalant to a 
         * Rec2 with min point (0,0) and max point (1,1)
        */
        constexpr Poly2() : points{ Point2<T>(), Point2<T>(0, 1), Point2<T>(1,1), Point2<T>(1, 0) } {
            if (!isConvex()) {
                throw std::logic_error("Poly2 is not convex");
            }
        }

        /**
         * @brief Constructs a Poly2 directly from a vector of points
         * @param points the points to construct from
        */
        constexpr explicit Poly2(const std::vector<Point2<T>>& points) : points(points) {
            if (!isConvex()) {
                throw std::logic_error("Poly2 is not convex");
            }
        }

        /**
         * @brief Constructs a Poly2 from an initializer list of points 
         * @details Constructs a Poly2 from an initializer list of points,
         * example syntax:
         * 
         *           Poly2({Point2<T>(0,0), Point2<T>(1, 0), Point2<T>(0, 1)});
         * 
         * @param list the list of points to construct from
        */
        constexpr explicit Poly2(const std::initializer_list<Point2<T>>& list) : points(list) {
            if (!isConvex()) {
                throw std::logic_error("Poly2 is not convex");
            }
        }

        /**
         * @brief Constructs a Poly2 from a paired list of initializer list points
         * @details Constructs a Poly2 from a paired list of points, 
         * example syntax:
         * 
         *           Poly2f{{ 0, 0 }, { 0, 1 }, { 1, 1 }};
         * 
         * @param listPaired 
        */
        constexpr explicit Poly2(const std::initializer_list<std::initializer_list<T>>& listPaired) {
            points.resize(listPaired.size());
            size_t i = 0;
            for (auto it = listPaired.begin(); it != listPaired.end(); it++, i++) {
                if(it->size() != 2) throw std::logic_error("Poly2 points list is not paired");
                points.at(i) = Point2<T>(*(it->begin()), *(it->begin() + 1));
            }
            if (!isConvex()) {
                throw std::logic_error("Poly2 is not convex");
            }
        }

        template<bool...> struct bool_pack;
        template<bool... bs>
        using all_true = std::is_same<bool_pack<bs..., true>, bool_pack<true, bs...>>;

        template<class R, class... Ts>
        using are_all_convertible = all_true<std::is_convertible<Ts, R>::value...>;
        /**
         * @brief Constructs a Poly2 from a paired list of arguments
         * @details Constructs a Poly2 from a paired list of arguments 
         * that are convertable to the coordinate type, because of CTAD
         * this can be more pleasant syntax wise than the constructors above
         * example syntax:
         * 
         *           Poly2(0,0,1,0,0,1); //produces the same Poly2 as the example in the constructor above
         * 
         * @tparam ...Args template param for the point arguments
         * @tparam only enabled if the number of template params is even, since each param is paired off
         * @param ...pts 
        */
        template<typename... Args,
            typename = std::enable_if_t<sizeof...(Args) % 2 == 0 && sizeof...(Args) >= 2 && are_all_convertible<T, Args...>::value>>
        constexpr explicit Poly2(Args... pts) {
            std::vector<T> pointvals{ (T)pts ... };
            points.resize(pointvals.size() / 2);
            for (size_t i = 0; i < pointvals.size(); i += 2) {
                points.at(i / 2) = Point2<T>(pointvals.at(i), pointvals.at(i + 1));
            }

            if (!isConvex()) {
                throw std::logic_error("Poly2 is not convex");
            }
        }

        /**
         * @brief Constructs a Poly2 from a set of points ranging (0,0) to (1,1) and a rectangular dimension
         * @details Constructs a Poly2 from a set of points ranging 
         * (0,0) to (1,1) and a rectangular dimension such that each point is 
         * some percentage between the min and max values of the supplied rectangle
         * @param points0_to_1 the list of points (0,0) to (1,1)
         * @param quadDim the rectangular dimension to construct the polygon inside
        */
        constexpr explicit Poly2(const std::vector<Point2<T>>& points0_to_1, const Rect2<T>& quadDim) : points(points0_to_1) {
            auto len = points.size();
            for (size_t i = 0; i < len; i++) {
                auto& p = points.at(i);
                if (abs<T>(p.x) > 1) throw std::logic_error("Points must be between (0,0) and (1,1)");
                if (abs<T>(p.y) > 1) throw std::logic_error("Points must be between (0,0) and (1,1)");
                p.x = lerp<T>(quadDim.min.x, quadDim.max.x, p.x);
                p.y = lerp<T>(quadDim.min.y, quadDim.max.y, p.y);
            }

            if (!isConvex()) {
                throw std::logic_error("Poly2 is not convex");
            }
        }


        /**
         * @brief Constructs a Poly2 eqivalent to the supplied Rect2
         * @param quadDim the rectangle to construct from
         * @todo make all of these conversion like constructors explicit
         * conversion functions
        */
        constexpr explicit Poly2(const Rect2<T>& quadDim) 
            : Poly2({ Point2<T>(0,0), Point2<T>(0,1), Point2<T>(1,1), Point2<T>(1,0) }, 
                quadDim) {

            if (!isConvex()) {
                throw std::logic_error("Poly2 is not convex");
            }

        }

        /**
		 * @brief Access the Poly2 points as if it were an array
		 * @param i The index
		 * @return a read only reference to the coordinate
		*/
        constexpr const Point2<T>& operator[] (const size_t i) const {
            if (i >= size())  throw std::out_of_range("Poly2 subscript out of range");
            return points[i];
        }

        /**
		 * @brief Access the Poly2 points as if it were an array
		 * @param i The index
		 * @return a read and write reference to the coordinate
		*/
        constexpr Point2<T>& operator[] (const size_t i) {
            if(i >= size())  throw std::out_of_range("Poly2 subscript out of range");
            dirty = true;
            return points[i];
        }

        /**
		 * @brief Access the Poly2 points as if it were a vector
		 * @param i The index
		 * @return a read only reference to the coordinate
		*/
        constexpr const Point2<T>& at(const size_t i) const {
            if (i >= size())  throw std::out_of_range("Poly2 subscript out of range");
            return points[i];
        }

        /**
		 * @brief Access the Poly2 points as if it were a vector
		 * @param i The index
		 * @return a read and write reference to the coordinate
		*/
        constexpr Point2<T>& at(const size_t i) {
            if(i >= size())  throw std::out_of_range("Poly2 subscript out of range");
            dirty = true;
            return points[i];
        }


        /**
		 * @brief Comparison operators for Poly2, sorted by size, then by first to last points
		 * @param other The Poly2 to compare with
		*/
        auto operator<=> (const Poly2<T>& other) const noexcept {
            if (size() != other.size()) {
                if (size() < other.size()) {
                    return Point2<T>() <=> other[size()];
                }
                else {
                    return (*this)[other.size()] <=> Point2<T>();
                }
            } else {
                size_t ind = 0;
                for (; ind < size(); ind++) {
                    if ((*this)[ind] != other[ind]) {
                        return (*this)[ind] <=> other[ind];
                    }
                }

                return (*this)[ind] <=> other[ind];
            }
        }

        /**
		 * @brief Equality operator for Poly2
		 * @param other The Poly2 to compare with
		 * @return true if the two Poly2's are equal
		*/
        bool operator==  (const Poly2<T>& other) const noexcept {
            return points == other.points;
        }

        /**
         * @brief Returns the size of the Poly2, or number of points
         * @return the number of points
        */
        const size_t size() const {
            return points.size();
        }

        /**
        * @brief Read and write iterator for Poly2
        */
        struct Iterator {

            Iterator(const size_t index, Poly2* val) : index(index), val(val) {}

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
            Poly2* val;
        };

        /**
         * @brief Read only iterator for Poly2
        */
        struct CIterator {

            CIterator(const size_t index, const Poly2* val) : index(index), val(val) {}

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
            const Poly2* val;
        };

        /**
         * @brief Beginning of the Iterator range
         * @return A const iterator at the beginning of the Poly2
        */
        const CIterator cbegin() const {
            return CIterator(0, this);
        }

        /**
         * @brief One past the end of the Iterator range
         * @return A const iterator one past the end of the Poly2
        */
        const CIterator cend() const {
            return CIterator(size(), this);
        }

        /**
         * @brief Beginning of the Iterator range
         * @return An iterator at the beginning of the Poly2
        */
        Iterator begin() {
            return Iterator(0, this);
        }

        /**
         * @brief One past the end of the Iterator range
         * @return An iterator one past the end of the Poly2
        */
        Iterator end() {
            return Iterator(size(), this);
        }

        /**
         * @brief Computes the centroid, or the unweighted center of mass of the Poly2
         * @details will incorrectly compute if the Poly2 is convex, so concavity is checked for
         * if the Poly2 is dirty here
         * @return the computed Centroid Point2
        */
        constexpr const Point2<T> centroid() const {

            if (dirty) {
                if (!isConvex()) {
                    throw std::logic_error("Poly2 is not convex");
                }
                dirty = false;
            }

            Point2<T> cent;

            auto len = points.size();
            double signedArea = 0;

            for (size_t i = 0; i < len - 1; i++) {

                double x0 = points[i].x;
                double y0 = points[i].y;
                double x1 = points[i + 1].x;
                double y1 = points[i + 1].y;

                double A = (x0 * y1) - (x1 * y0);
                signedArea += A;

                cent.x += static_cast<T>((x0 + x1) * A);
                cent.y += static_cast<T>((y0 + y1) * A);
            }

            double x0 = points[len - 1].x;
            double y0 = points[len - 1].y;
            double x1 = points[0].x;
            double y1 = points[0].y;

            double A = (x0 * y1) - (x1 * y0);
            signedArea += A;

            cent.x += static_cast<T>((x0 + x1) * A);
            cent.y += static_cast<T>((y0 + y1) * A);

            signedArea *= 0.5;
            cent.x /= static_cast<T>((6 * signedArea));
            cent.y /= static_cast<T>((6 * signedArea));

            return cent;
        }

        /**
         * @brief moves the Poly2 and all its points to a new center
         * @details moves the points such that the centroid will be calculated
         * to be at the point newcenter
         * @param newcenter the center to translate the points to
        */
        constexpr void moveCenterTo(const Point2<T>& newcenter) {
            auto cent = centroid();
            auto diffVec = Vec2<T>(cent, newcenter);
            cent = newcenter;
            for (auto& p : points) {
                p += diffVec;
            }
        }
        
        /**
         * @brief moves the Poly2 and all its points to a new center
         @details moves the points such that the centroid will be calculated
         * to be at the point newcenter
         * @param cx the x coordinate of the new center
         * @param cy the y coordinate of the new center
        */
        constexpr void moveCenterTo(const T& cx, const T& cy) {
            moveCenterTo(Point2<T>(cx, cy));
        }

        /**
         * @brief Computes the normal of the face of the supplied index
         * @param i the index of the first point of the face
         * @return the normal of the face
        */
        constexpr NormVec2<T> getFaceNormal(const size_t i) const {
            if (i >= size()) throw std::out_of_range("Poly2 index out of range");
            unsigned int j = i < points.size() - 1 ? i + 1 : 0;
            return Vec2<T>(points[i], points[j]).unitNormal();
        }


        /**
         * @brief Get a Vec2 of the face of the supplied index
         * @details Get a Vec2 of the face of the supplied index,
         * where the index supplied is the index of the first point of the face
         * @param type The index of the minimum Point2 of the face to get the Vec2 of
         * @return The Vec2 representing the requested face
        */
        constexpr Vec2<T> getFaceVec(const size_t index) const {
            if (index >= size()) throw std::out_of_range("Poly2 index out of range");
            Point2<T> p1 = points.at(index);
            Point2<T> p2 = index == size() - 1 ? points.at(0) : points.at(index + 1);
            return Vec2<T>(p1, p2);
        }

      /**
       * @brief Get the two points that make up the supplied face of the Poly2
       * @param index the index of the first point of the face
       * @return The points of the supplied face, in an array ordered by index first
      */
        constexpr std::array<Point2<T>, 2> getFacePoints(const size_t index) const {
            if (index >= size()) throw std::out_of_range("Poly2 index out of range");
            Point2<T> p1 = points.at(index);
            Point2<T> p2 = index == size() - 1 ? points.at(0) : points.at(index + 1);
            return std::array<Point2<T>, 2>{p1, p2};
        }

        /**
         * @brief Computes the area of the Poly2
         * @details Area is also concavity sensative, so concavity is checked for
         * if the dirty bit is set
         * @return the computed area
        */
        constexpr T area() const {

            if (dirty) {
                if (!isConvex()) {
                    throw std::logic_error("Poly2 is not convex");
                }
                dirty = false;
            }

            T a = 0.0;

            auto len = points.size();
            auto j = len - 1;
            for (auto i = 0; i < len; i++)
            {
                a += (points[j].x + points[i].x) * (points[j].y - points[i].y);
                j = i;
            }
            return abs(a / (T)2.0f);
        }

        /**
         * @brief Computes the Axis Aligned Bounding Box (AABB) of the Poly2
         * @return the computed AABB
        */
        constexpr Rect2<T> getAABB() const noexcept {
            T minx = points[0].x;
            T miny = points[0].y;
            T maxx = points[0].x;
            T maxy = points[0].y;
            auto len = points.size();
            for (size_t i = 1; i < len; i++) {
                minx = min(minx, points[i].x);
                maxx = max(maxx, points[i].x);

                miny = min(miny, points[i].y);
                maxy = max(maxy, points[i].y);
            }
            return Rect<T>(Point2<T>(minx, miny), Point2<T>(maxx, maxy));
        }


        /**
         * @brief Rotates the Poly2 by the given radian value
         * @param rad the radian value to rotate by
         * @todo include a contains function for Poly2
        */
        constexpr void rotate(const Radians rad) noexcept {
            Mat3 rmat;
            rmat.rotate(rad);
            rmat.transform(*this);
        }

        /**
		 * @brief Prints the Poly2
		 * @param os Input stream
		 * @param it The Poly2 to print
		 * @return a reference to the stream for << chaining
         * @todo hard code better formatting for common smaller sizes, such as
         * triangles, rectangles, pentagons, hexagons, etc, then have a better general
         * printing formula for sizes larger than the hardcoded formats
		*/
        friend std::ostream& operator << (std::ostream& os, const Poly2& it) {

            std::string typname = typeid(T).name();
            std::string::size_type i = typname.find("struct");
            if (i != std::string::npos) {
                typname.erase(i, 7);
            }
            i = typname.find("class");
            if (i != std::string::npos) {
                typname.erase(i, 6);
            }

            os << "Polygon<" << typname << ">[size = " << it.points.size() << "]{\n";
            for (size_t i = 0; i < it.points.size(); i++) {
                os << "(" << it.points.at(i).x << ", " << it.points.at(i).y << "),\n";
            }
            os << "}";
            return os;
        }

        /**
         * @brief Explicit conversion to other Poly2 types
         * @tparam Other the underlying data type of the Poly2 to convert to
        */
        template<typename Other>
        explicit operator Poly2<Other>() const {
            auto otherPoints = std::vector<Point2<Other>>(size());
            for (size_t i = 0; i < size(); i++) {
                otherPoints.at(i) = (Point2<Other>)points.at(i);
            }
            return Poly2<Other>(otherPoints);
        }

#ifdef _SFML_ENABLED

        /**
         * @brief Creates a sf::ConvexPolygon (a drawable convex polygon)
         * @param borderSize the size of the Shape border
         * border size strictly adds to the size, so the Shape
         * will only retain the same dimensions as the s2d::Poly2 if border is 0.0f,
         * however the interior area(and interior color) will
         * always be the same dimensions as the original Poly2
         * @param interiorColor the interior color of the sf::RectangleShape
         * @param borderColor the border color of the sf::RectangleShape
         * @return the sf::RectangleShape
        */
        sf::ConvexShape getDrawableSFMLPolygon(const float borderSize = 0.0f, const sf::Color interiorColor = sf::Color(0, 0, 0, 0), const sf::Color borderColor = sf::Color(0, 0, 0, 0)) const {
            sf::ConvexShape newshape;
            newshape.setOutlineThickness(borderSize);
            newshape.setOutlineColor(borderColor);
            newshape.setFillColor(interiorColor);
            newshape.setPointCount(points.size());
            for (size_t i = 0; i < points.size(); i++) {
                sf::Vector2f vec((float)points[i].x, (float)points[i].y);
                newshape.setPoint(i, vec);
            }
            return newshape;
        }
#endif

        /**
         * @brief subtract-eq for Poly2 with Vec2
         * @param rhs the subtraction Vec2
        */
        S2D_POLY_2D_OP_EQ(-=, Vec2);

        /**
         * @brief subtract for Poly2 with Vec2
         * @param rhs the subtraction Vec2
        */
        S2D_POLY_2D_OP(-, Vec2);

        /**
         * @brief subtract-eq for Poly2 with NormVec2
         * @param rhs the subtraction NormVec2
        */
        S2D_POLY_2D_OP_EQ(-=, NormVec2);

        /**
         * @brief subtract for Poly2 with NormVec2
         * @param rhs the subtraction NormVec2
        */
        S2D_POLY_2D_OP(-, NormVec2);


        /**
         * @brief addition-eq for Poly2 with Vec2
         * @param rhs the addition Vec2
        */
        S2D_POLY_2D_OP_EQ(+=, Vec2);

        /**
         * @brief addition for Poly2 with Vec2
         * @param rhs the addition Vec2
        */
        S2D_POLY_2D_OP(+, Vec2);

        /**
         * @brief addition-eq for Poly2 with NormVec2
         * @param rhs the addition NormVec2
        */
        S2D_POLY_2D_OP_EQ(+=, NormVec2);

        /**
         * @brief addition for Poly2 with NormVec2
         * @param rhs the addition NormVec2
        */
        S2D_POLY_2D_OP(+, NormVec2);


        private:

            /**
             * @brief the points of the Poly2
            */
            std::vector<Point2<T>> points;

            /**
             * @brief a dirty bit for the Poly2
             * @details a dirty bit for the Poly2, this is used to 
             * check the concavity invariant; every time a point is accessed
             * non const, it is possible the user could change a point and break concavity,
             * therefore the dirty bit is set when a point is non const accessed, and any function
             * that is concavity sensative will check the invariant again if the dirty bit is set
            */
            mutable bool dirty = false;

            /**
             * @brief determines if the polygon is convex, necessasary for checking invariants
             * @return true if the polygon is convex
            */
            constexpr bool isConvex() const noexcept {
                T prev = 0;
                T curr = 0;

                size_t sizeval = size();
                std::vector<T> crossproducts;

                for (size_t i = 0; i < sizeval; i++) {
                    size_t j = (i + 1) % sizeval;

                    curr = (-getFaceVec(i)).cross(getFaceVec(j));

                    if (curr != 0) {
                        if (curr * prev < 0) {
                            return false;
                        }
                        else {
                            prev = curr;
                        }
                    }
                }
                return true;
            }
    };
}

