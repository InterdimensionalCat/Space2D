#pragma once
#include "AngularType.h"
#include "S2DIterator.h"

namespace Space2D {

    template<typename T>
    class NormVec2
    {
    public:

        constexpr explicit NormVec2(const T& x, const T& y) : x(x / std::sqrt(x * x + y * y)), y(y / std::sqrt(x * x + y * y)) {}

        //constructor from radian angle value
        constexpr explicit NormVec2(const Radians radians) noexcept : 
            x(std::cos(radians.get())), 
            y(std::sin(radians.get())) {}

        auto operator<=>(const NormVec2&) const noexcept = default;
        bool operator== (const NormVec2& other) const noexcept {
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

        S2D_ITR_DEF(NormVec2)

        NormVec2 operator-() const noexcept {
            return NormalizedVec2(-x, -y);
        }

        Radians angle() const noexcept {
            return Radians(std::acos(x));
        }

#ifdef _SFML_ENABLED

        template <typename SFMLType = float>
        sf::Vector2<SFMLType> toSFMLVec() const noexcept {
            return sf::Vector2<SFMLType>(static_cast<SFMLType>(this->x), static_cast<SFMLType>(this->y));
        }
#endif

        friend std::ostream& operator << (std::ostream& os, const NormVec2<T>& it) {
            std::string typname = typeid(T).name();
            std::string::size_type i = typname.find("struct");
            if (i != std::string::npos) {
                typname.erase(i, 7);
            }
            i = typname.find("class");
            if (i != std::string::npos) {
                typname.erase(i, 6);
            }
            os << "NormVec2<" << typname << ">(" << it.x << ", " << it.y << ")";
            return os;
        }

        T x;
        T y;
    };
}



