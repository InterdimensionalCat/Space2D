#pragma once
#include "AngularType.h"
#include "S2DIterator.h"


namespace Space2D {

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

    template<typename T>
    class Rect2 
    {

    public:

        //constructs a rect with points (0,0) and (0,0)
        Rect2() noexcept {}

        constexpr explicit Rect2(const T& x0, const T& y0, const T& x1, const T& y1) noexcept
            : min(Point2<T>(x0, y0)), max(Point2<T>(x1, y1)) {}

        constexpr explicit Rect2(const Point2<T>& min, const Point2<T>& max) noexcept
            : min(min), max(max) {}

        //define a rect like an sfml rectangle(min_point + dimensions)
        constexpr explicit Rect2(const Point2<T>& min, const Dim2<T>& dim) noexcept
            : min(min), max(Point2<T>(min.x + dim.x, min.y + dim.y)) {}

        //define a rectangle as a sub area of a smaller rectangle such that
        //     _____________
        //    |             |
        //    |x0,y0__      |
        //    |  |   |      |
        //    |  |___x1, y1 |
        //    |_____________|
        //where (x0, x1) is base_rect.min + min_offset
        //and (x1, y1) is base_rect.max - max_offset
        constexpr explicit Rect2(const Rect2& baseRect,
            const Vec2<T>& minOffset, const Vec2<T>& maxOffset) noexcept
            : min(baseRect.min + minOffset), max(baseRect.max - maxOffset) {}

        //same constructor as above but as a ratio of the rectangle size
        constexpr explicit Rect2(const Rect2<T>& baseRect, 
            const Dim2<Percent>& minOffset, const Dim2<Percent>& maxOffset) noexcept :
            min(baseRect.min + baseRect.size() * ((Dim2<T>)minOffset)),
            max(baseRect.max - baseRect.size() * ((Dim2<T>)maxOffset))
        {}

        auto operator<=>(const Rect2&) const noexcept = default;
        bool operator==(const Rect2& other) const noexcept {
            return min == other.min && max == other.max;
        }

        const T& operator[] (const size_t i) const noexcept {
            if (i < 2) {
                return min[i];
            }
            else {
                return max[i - 2];
            }
        }
        T& operator[] (const size_t i) noexcept {
            if (i < 2) {
                return min[i];
            }
            else {
                return max[i - 2];
            }
        }

        const size_t numVals() const {
            return 4;
        }

        S2D_ITR_DEF(Rect2)

        T area() const noexcept {
            return width() * height();
        }

        Point2<T> center() const noexcept {
            return Point2<T>((max.x + min.x) * 50_pcent, (max.y + min.y) * 50_pcent);
        }

        bool contains(const Point2<T>& query) const noexcept {
            return (query.x > min.x) &&
                (query.x < max.x) &&
                (query.y > min.y) &&
                (query.y < max.y);
        }

        Dim2<T> size() const noexcept {
            return Dim2<T>(min, max);
        }

        T width() const noexcept {
            return max.x - min.x;
        }

        T height() const noexcept {
            return max.y - min.y;
        }

        void moveMinTo(const Point2<T>& newMin) noexcept {
            auto size = Rect2::size();
            min = newMin;
            max = newMin + Vec(size.x, size.y);
        }

        void moveCenterTo(const Point2<T>& newCenter) noexcept {
            auto size = Rect2::size();
            min = newCenter - (size / 2.0f);
            max = newCenter + (size / 2.0f);
        }

        NormVec2<T> getNormal(const RectFace type) {
            switch (type) {
            case RectFace::Up:
                return getNormal_0_neg1();
            case RectFace::Down:
                return getNormal_0_1();
            case RectFace::Left:
                return getNormal_neg1_0();
            case RectFace::Right:
                return getNormal_1_0();
            }

        }

        NormVec2<T> getNormal_1_0() {
            return NormVec2<T>(static_cast<T>(1), static_cast<T>(0));
        }

        NormVec2<T> getNormal_neg1_0() {
            return NormVec2<T>(static_cast<T>(-1), static_cast<T>(0));
        }

        NormVec2<T> getNormal_0_1() {
            return NormVec2<T>(static_cast<T>(0), static_cast<T>(1));
        }

        NormVec2<T> getNormal_0_neg1() {
            return NormVec2<T>(static_cast<T>(0), static_cast<T>(-1));
        }

#ifdef _SFML_ENABLED

        template <typename SFMLType = float>
        sf::Rect<SFMLType> toSFMLRect() const noexcept {
            sf::Vector2<SFMLType> v1 = sf::Vector2<SFMLType>(static_cast<SFMLType>(min.x), static_cast<SFMLType>(min.y));
            sf::Vector2<SFMLType> v2 = sf::Vector2<SFMLType>(static_cast<SFMLType>(width()), static_cast<SFMLType>(height()));
            return sf::Rect<SFMLType>(v1, v2);
        }
        
        //border size strictly adds to the size, so the RectangleShape will only retain the same dimensions as the s2d::Rect if border is 0.0f
        //however, section that is the "interior color" will never get clipped due to a large border size
        sf::RectangleShape makeDrawableSFMLRect(const float bordersize = 0.0f, const sf::Color interior_color = sf::Color(0,0,0,0), const sf::Color border_color = sf::Color(0, 0, 0, 0)) const {
            sf::RectangleShape newshape;
            newshape.setOutlineThickness(bordersize);
            newshape.setOutlineColor(border_color);
            newshape.setFillColor(interior_color);
            sf::Vector2f v1 = sf::Vector2f(static_cast<float>(min.x), static_cast<float>(min.y));
            sf::Vector2f v2 = sf::Vector2f(static_cast<float>(width()), static_cast<float>(height()));
            newshape.setPosition(v1);
            newshape.setSize(v2);
            return newshape;
        }
#endif

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

            os  << "Rect<" << typname << ">(Min: " << it.min << ", Max : " << it.max << ")";
            return os;
        }


        Point2<T> min;
        Point2<T> max;
    };
}

