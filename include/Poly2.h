#pragma once
#include <initializer_list>
#include "Space2D.h"
#include <vector>
#include <algorithm>
#include "S2DMath.h"
#include "S2DIterator.h"

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
    class Poly2
    {

    public:

        //defaults to a single-point polygon with centroid (0,0)
        constexpr Poly2() noexcept : points{ Point2<T>() } {}

        //construct a polygon directly from a set of points
        constexpr explicit Poly2(const std::vector<Point2<T>>& points) noexcept : points(points) {}

        constexpr explicit Poly2(std::initializer_list<Point2<T>> list) noexcept : points(list) {}

        //construct a polygon from a variadic number of points
        template<typename... Args>
        constexpr explicit Poly2(Args... pts) noexcept : points({ pts ... }) {}

        //construct a polygon from a set of points ranging from zero to one
        constexpr explicit Poly2(const std::vector<Point2<T>>& points0_to_1, const Rect2<T>& quadDim) noexcept {
            auto len = points.size();
            for (size_t i = 0; i < len; i++) {
                auto& p = points.at(i);
                p.x = lerp<T>(quadDim.min.x, quadDim.max.x, p.x);
                p.y = lerp<T>(quadDim.min.y, quadDim.max.y, p.y);
            }
        }

        //construct the polygon equivilant of a rectangle
        explicit Poly2(const Rect2<T>& quadDim) noexcept 
            : Poly2({ Point2<T>(0,0), Point2<T>(1,0), Point2<T>(1,1), Point2<T>(0,1) }, 
                quadDim) {}

        Point2<T>& operator[] (const size_t i) {
            size_t pointInd = (size_t)floor(i * 0.5);
            if (i % 2 == 0) {
                return points[i].x;
            }
            else {
                return points[i].y;
            }
        }

        const Point2<T>& operator[] (const size_t i) const {
            size_t pointInd = (size_t)floor(i * 0.5);
            if (i % 2 == 0) {
                return points[i].x;
            }
            else {
                return points[i].y;
            }
        }

        auto operator<=> (const Poly2<T>& other) const noexcept {
            if (numVals() != other.numVals()) {
                return numVals() <=> other.numVals();
            } else {
                size_t ind = 0;
                size_t maxind = 0;
                while (ind < maxind - 1 && (*this)[ind] != other[ind]) {
                    ind++;
                }
                return (*this)[ind] <=> other[ind];
            }
        }

        bool operator==  (const Poly2<T>& other) const noexcept {
            points == other.points;
        }

        const size_t numVals() const {
            return points.size() * 2;
        }

        const size_t numPoints() const {
            return points.size();
        }

        S2D_ITR_DEF(Poly2)

        const Point2<T>& centroid() const {
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

        void translateToCenter(const Point2<T>& newcenter) {
            auto cent = centroid();
            auto diffVec = Vec2<T>(cent, newcenter);
            cent = newcenter;
            for (auto& p : points) {
                p += diffVec;
            }
        }

        NormVec2<T> normal(const size_t i) const {
            unsigned int j = i < points.size() - 1 ? i + 1 : 0;
            return Vec2<T>(points[i], points[j]).unitNormal();
        }

        T area() const noexcept {
            T a = 0.0;

            auto len = points.size();
            auto j = len - 1;
            for (auto i = 0; i < len; i++)
            {
                a += (points[j].x + points[i].x) * (points[j].y - points[i].y);
                j = i;
            }
            return abs(a / 2.0f);
        }

        Rect2<T> getAABB() const noexcept {
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

#ifdef _SFML_ENABLED

        sf::ConvexShape getDrawableSFMLPolygon(const float bordersize = 0.0f, const sf::Color interior_color = sf::Color(0, 0, 0, 0), const sf::Color border_color = sf::Color(0, 0, 0, 0)) const {
            sf::ConvexShape newshape;
            newshape.setOutlineThickness(bordersize);
            newshape.setOutlineColor(border_color);
            newshape.setFillColor(interior_color);
            newshape.setPointCount(points.size());
            for (size_t i = 0; i < points.size(); i++) {
                sf::Vector2f vec((float)points[i].x, (float)points[i].y);
                newshape.setPoint(i, vec);
            }
            return newshape;
        }
#endif

        std::vector<Point2<T>> points;
    };
}

