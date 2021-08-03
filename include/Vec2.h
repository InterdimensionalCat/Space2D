#pragma once
#include "S2DIterator.h"


#ifndef S2D_VEC_2D_OPERATOR
#define S2D_VEC_2D_OPERATOR
#define S2D_VEC_2D_OP_EQ(op, typ2d) \
    constexpr inline Vec2& operator##op(const typ2d<T>& rhs) noexcept {\
    x op rhs.x;\
    y op rhs.y;\
    return (*this);\
	}

#define S2D_VEC_2D_OP(op, typ2d) \
	constexpr inline Vec2 operator##op(const typ2d<T>& rhs) const noexcept { \
    return Vec2(x op rhs.x, y op rhs.y);\
	}
#endif

#ifndef S2D_VEC_1D_OPERATOR
#define S2D_VEC_1D_OPERATOR
#define S2D_VEC_1D_OP_EQ(op) \
    constexpr inline Vec2& operator##op(const T& rhs) noexcept {\
    x op rhs;\
    y op rhs;\
    return (*this);\
	}

#define S2D_VEC_1D_OP(op) \
	constexpr inline Vec2 operator##op(const T& rhs) const noexcept { \
    return Vec2(x op rhs, y op rhs);\
	}
#endif


namespace Space2D {

	template<typename T>
	class Point2;
	template<typename T>
	class NormVec2;


	/**
	 * @brief Class encapsulating a 2 Dimensional Vector representation
	 * @tparam T Underlying data type of the coordinates
	*/
	template<typename T>
    class Vec2
    {

    public:

		/**
		 * @brief Constructs a Vec2 of size (0,0)
		 * @return 
		*/
		constexpr Vec2() noexcept : x(), y() {}

		/**
		 * @brief Constructs a Vec2 of size (x, y)
		 * @param x Vec2 x coordinate
		 * @param y Vec2 y coordinate
		 * @return 
		*/
		constexpr explicit Vec2(const T& x, const T& y) noexcept : x(x), y(y) {}

		/**
		 * @brief Constructs a Vec2 directly from a Point2
		 * @param other The point to construct from
		 * @return 
		*/
		constexpr explicit Vec2(const Point2<T>& other) noexcept : Vec2(other.x, other.y) {}

		/**
		 * @brief Constructs a Vec2 as a displacement vector from point start to end
		 * @param start Starting point
		 * @param end Ending point
		 * @return 
		*/
		constexpr explicit Vec2(const Point2<T>& start, const Point2<T>& end) noexcept : Vec2(end.x - start.x, end.y - start.y) {}

		
		/**
		 * @brief (x, y) -> (-x, -y)
		 * @return the sign-negative Vec2
		*/
		constexpr Vec2 operator-() const noexcept {
			return Vec2<T>(-x, -y);
		}

				/**
		 * @brief (x, y) -> (+x, +y)
		 * @return the sign-positive Vec2
		*/
		constexpr Vec2 operator+() const noexcept {
			return Vec2<T>(+x, +y);
		}

		/**
		 * @brief Access the Vec2 coords as if it were an array
		 * @param i The index, 0 = x and 1 = y
		 * @return a read only reference to the coordinate
		*/
		constexpr const T& operator[] (const size_t i) const {
			if (i > 1) throw std::out_of_range("Vec2 subscript out of range");
			return i == 0 ? x : y;
		}

		/**
		 * @brief Access the Vec2 coords as if it were an array
		 * @param i The index, 0 = x and 1 = y
		 * @return a read and write reference to the coordinate
		*/
		constexpr T& operator[] (const size_t i){
			if (i > 1) throw std::out_of_range("Vec2 subscript out of range");
			return i == 0 ? x : y;
		}

		constexpr const size_t numVals() const {
			return 2;
		}

		/**
		 * @brief the size of the Vec2, always 2
		 * @return the size of the Vec2
		*/
		constexpr const size_t size() const {
			return 2;
		}

		/**
		 * @brief Read and write iterator for Vec2
		*/
		struct Iterator {

			Iterator(const size_t index, Vec2* val) : index(index), val(val) {}

			T& operator*() {
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
			Vec2* val;
		};

		/**
		 * @brief Read only iterator for Vec2
		*/
		struct CIterator {

			CIterator(const size_t index, const Vec2* val) : index(index), val(val) {}

			const T& operator*() const {
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
			const Vec2* val;
		};

		/**
		 * @brief Beginning of the Iterator range
		 * @return A const iterator at the beginning of the Vec2
		*/
		const CIterator cbegin() const {
			return CIterator(0, this);
		}

		/**
		 * @brief One past the end of the Iterator range
		 * @return A const iterator one past the end of the Vec2
		*/
		const CIterator cend() const {
			return CIterator(2, this);
		}

		/**
		 * @brief Beginning of the Iterator range
		 * @return An iterator at the beginning of the Vec2
		*/
		Iterator begin() {
			return Iterator(0, this);
		}

		/**
		 * @brief One past the end of the Iterator range
		 * @return An iterator one past the end of the Vec2
		*/
		Iterator end() {
			return Iterator(2, this);
		}

		/**
		 * @brief Comparison operators for Vec2, sorted by x coordinate then y coordinate
		 * @param other The Vec2 to compare with
		 * @return 
		*/
		constexpr auto operator<=>(const Vec2& other) const noexcept = default;


		/**
		 * @brief Equality operator for Vec2
		 * @param other The Vec2 to compare with
		 * @return true if the two Vec2's are equal
		*/
		constexpr bool operator==(const Vec2& other) const noexcept {
			return std::abs(x - other.x) < 1e-6 && std::abs(y - other.y) < 1e-6;
		}

		/**
		 * @brief Magnitude of a Vec2
		 * @return the magnitude
		*/
		constexpr T mag() const noexcept {
			return std::sqrt(x * x + y * y);
		}

		/**
		 * @brief Magnitude^2 of a Vec2
		 * @return The square magnitude
		*/
		constexpr T magSquared() const noexcept {
			return x * x + y * y;
		}

		/**
		 * @brief normalizes the Vec2
		 * @return a NormVec2 pointing in the direction of the Vec2
		*/
		constexpr NormVec2<T> normalize() const {
			return NormVec2<T>(x, y);
		}

		/**
		 * @brief calculates a unit normal vector, 
		 which is a normal vector perpindicular to the supplied vector
		 * @return the unit normal vector: unit normal rule: (x, y) -> (y, -x)
		*/
		constexpr NormVec2<T> unitNormal() const {
			return NormVec2<T>(y, -x);
		}

#ifdef _SFML_ENABLED

		template<typename SFMLType>
		constexpr operator sf::Vector2<SFMLType>() const noexcept {
			return toSFMLVec<SFMLType>();
		}

		template <typename SFMLType = float>
		constexpr sf::Vector2<SFMLType> toSFMLVec() const noexcept {
			return sf::Vector2<SFMLType>(static_cast<SFMLType>(this->x), static_cast<SFMLType>(this->y));
		}

#endif

		/**
		 * @brief Prints the Vec2
		 * @param os Input stream
		 * @param it The vector to print
		 * @return a reference to the stream for << chaining
		*/
		friend std::ostream& operator << (std::ostream& os, const Vec2<T>& it) {
			std::string typname = typeid(T).name();
			std::string::size_type i = typname.find("struct");
			if (i != std::string::npos) {
				typname.erase(i, 7);
			}
			i = typname.find("class");
			if (i != std::string::npos) {
				typname.erase(i, 6);
			}
			os << "Vec2<" << typname << ">(" << it.x << ", " << it.y << ")";
			return os;
		}

		/**
		 * @brief subtract-eq for Vec2 with Vec2
		 * @param rhs the subtraction Vec2
		*/
		S2D_VEC_2D_OP_EQ(-=, Vec2);

		/**
		 * @brief subtract for Vec2 with Vec2
		 * @param rhs the subtraction Vec2
		*/
		S2D_VEC_2D_OP(-, Vec2);

		/**
		 * @brief subtract-eq for Vec2 with NormVec2
		 * @param rhs the subtraction NormVec2
		*/
		S2D_VEC_2D_OP_EQ(-=, NormVec2);

		/**
		 * @brief subtract for Vec2 with NormVec2
		 * @param rhs the subtraction NormVec2
		*/
		S2D_VEC_2D_OP(-, NormVec2);


		/**
		 * @brief addition-eq for Vec2 with Vec2
		 * @param rhs the addition Vec2
		*/
		S2D_VEC_2D_OP_EQ(+=, Vec2);

		/**
		 * @brief addition for Vec2 with Vec2
		 * @param rhs the addition Vec2
		*/
		S2D_VEC_2D_OP(+, Vec2);

		/**
		 * @brief addition-eq for Vec2 with NormVec2
		 * @param rhs the addition NormVec2
		*/
		S2D_VEC_2D_OP_EQ(+=, NormVec2);

		/**
		 * @brief addition for Vec2 with NormVec2
		 * @param rhs the addition NormVec2
		*/
		S2D_VEC_2D_OP(+, NormVec2);

		/**
		 * @brief multiplication-eq for Vec2
		 * @param rhs the factor to multiply by
		*/
		S2D_VEC_1D_OP_EQ(*=);

		/**
		 * @brief multiplication for Vec2
		 * @param rhs the factor to multiply by
		*/
		S2D_VEC_1D_OP(*);

		/**
		 * @brief division-eq for Vec2
		 * @param rhs the divisor
		*/
		S2D_VEC_1D_OP_EQ(/=);

		/**
		 * @brief division for Vec2
		 * @param rhs the divisor
		*/
		S2D_VEC_1D_OP(/);


		/**
		 * @brief x coordinate of the Vec2
		*/
		T x;

		/**
		 * @brief y coordinate of the Vec2
		*/
		T y;
    };
}

#undef S2D_VEC_2D_OP
#undef S2D_VEC_2D_OP_EQ
#undef S2D_VEC_1D_OP
#undef S2D_VEC_1D_OP_EQ
#undef S2D_VEC_2D_OPERATOR