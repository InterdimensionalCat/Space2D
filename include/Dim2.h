#pragma once
#include "S2DIterator.h"

namespace Space2D {

    template<typename T>
    class Vec2;
    template<typename T>
    class NormVec2;
    template<typename T>
    class Point2;

    template<typename T>
    class Dim2
    {

    public:


        constexpr Dim2() noexcept : x{}, y{} {}
        constexpr explicit Dim2(const T& x, const T& y) noexcept : x(x), y(y) {}
        constexpr explicit Dim2(const Point2<T>& other) noexcept : Dim2(other.x, other.y) {}
        constexpr explicit Dim2(const Vec2<T>& other) noexcept : Dim2(other.x, other.y) {}
        constexpr explicit Dim2(const NormVec2<T>& other) noexcept : Dim2(other.x, other.y) {}
        constexpr explicit Dim2(const Point2<T>& min, const Point2<T>& max) noexcept : Dim2(max.x - min.x, max.y - min.y) {}

        auto operator<=>(const Dim2<T>&) const noexcept = default;
        bool operator== (const Dim2<T>& other) const noexcept {
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

        S2D_ITR_DEF(Dim2)

        template<typename Other>
        explicit operator Dim2<Other>() const {
            return Dim2<Other>((Other)x, (Other)y);
        }

#ifdef _SFML_ENABLED

        template <typename SFMLType = float>
        sf::Vector2<SFMLType> toSFMLVec() const noexcept {
            return sf::Vector2<SFMLType>(static_cast<SFMLType>(this->x), static_cast<SFMLType>(this->y));
        }
#endif

        friend std::ostream& operator << (std::ostream& os, const Dim2& it) {
            os << "Dim2(" << it.x << ", " << it.y << ")";
            return os;
        }

        T x;
        T y;
    };
}



