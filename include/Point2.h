#pragma once
#include "S2DIterator.h"

namespace Space2D {

    template<typename T>
    class Vec2;
    template<typename T>
    class NormVec2;
    template<typename T>
    class Dim2;

    template<typename T>
    class Point2
    {
    public:

        constexpr Point2() noexcept : x(), y() {}
        constexpr explicit Point2(const T& x, const T& y) noexcept : x(x), y(y) {}
        constexpr explicit Point2(const Vec2<T>& other) noexcept : x(other.x), y(other.y) {}
        constexpr explicit Point2(const Point2& base, const Vec2<T>& offset) noexcept : Point2(base.x + offset.x, base.y + offset.y) {}
        constexpr explicit Point2(const Point2& base, const NormVec2<T>& offset) noexcept : Point2(base.x + offset.x, base.y + offset.y) {}

        auto operator<=>(const Point2<T>&) const noexcept = default;
        bool operator== (const Point2<T>& other) const noexcept {
            return std::abs(x - other.x) < 1e-6 && std::abs(y - other.y) < 1e-6;
        }

        const T& operator[] (const size_t i) const noexcept {
            return i == 0 ? x : y;
        }
        T& operator[] (const size_t i) noexcept {

            return i == 0 ? x : y;
        }

        const size_t numVals() const {
            return 2;
        }

        S2D_ITR_DEF(Point2)


#ifdef _SFML_ENABLED
        //convert the point to an sf::Vector2<T> (T defaults to float)
        template <typename SFMLType = float>
        sf::Vector2<SFMLType> toSFMLVec() const noexcept {
            return sf::Vector2<SFMLType>(static_cast<SFMLType>(this->x), static_cast<SFMLType>(this->y));
        }
#endif

        friend std::ostream& operator << (std::ostream& os, const Point2<T>& it) {
            std::string typname = typeid(T).name();
            std::string::size_type i = typname.find("struct");
            if (i != std::string::npos) {
                typname.erase(i, 7);
            }
            i = typname.find("class");
            if (i != std::string::npos) {
                typname.erase(i, 6);
            }
            os << "Point2<" << typname << ">(" << it.x << ", " << it.y << ")";
            return os;
        }

        T x;
        T y;
    };
}



