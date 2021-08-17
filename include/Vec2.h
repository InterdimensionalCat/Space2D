#pragma once


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
	 * @brief Class encapsulating a 2 Dimensional vector representation
	 * @tparam T Underlying data type of the coordinates
	*/
	template<typename T>
    class Vec2
    {

    public:

		/**
		 * @brief Constructs a Vec2 of size (0,0)
		 * 
		*/
		constexpr Vec2() noexcept : x(), y() {}

		/**
		 * @brief Constructs a Vec2 of size (x, y)
		 * @param x Vec2 x coordinate
		 * @param y Vec2 y coordinate
		 * 
		*/
		constexpr explicit Vec2(const T& x, const T& y) noexcept : x(x), y(y) {}

		/**
		 * @brief Constructs a Vec2 directly from a Point2
		 * @param other The point to construct from
		 * 
		*/
		constexpr explicit Vec2(const Point2<T>& other) noexcept : Vec2(other.x, other.y) {}

		/**
		 * @brief Constructs a Vec2 as a displacement vector from point start to end
		 * @param start Starting point
		 * @param end Ending point
		 *  
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
		*/
		constexpr auto operator<=>(const Vec2& other) const noexcept = default;


		/**
		 * @brief Equality operator for Vec2
		 * @param other The Vec2 to compare with
		 * @return true if the two Vec2's are equal
		*/
		constexpr bool operator==(const Vec2& other) const noexcept {
			return std::abs((double)(x - other.x)) < epsilon && std::abs((double)(y - other.y)) < epsilon;
		}

		/**
		 * @brief Magnitude of a Vec2
		 * @return the magnitude
		*/
		constexpr T mag() const noexcept {
			return sqrt<T>(x * x + y * y);
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

		/**
		 * @brief Calculates the dot product two Vec2's
		 * @param rhs the other Vec2 to calculate with
		 * @return the dot product scalar
		*/
		constexpr T dot(const Vec2<T>& rhs) const noexcept {
			return x * rhs.x + y * rhs.y; 
		}

		/**
		 * @brief Calculates the dot product two Vec2's
		 * @param rhs the other NormVec2 to calculate with
		 * @return the dot product scalar
		*/
		constexpr T dot(const NormVec2<T>& rhs) const noexcept {
			return x * rhs.x + y * rhs.y; 
		}

		/**
		 * @brief 2 Dimensional cross product with Vec2
		 * @param rhs the Vec2 to cross this Vec2 with
		 * @return the cross product scalar
		*/
		constexpr T cross(const Vec2& rhs) const noexcept {
			return x * rhs.y - y * rhs.x;
		}

		/**
		 * @brief 2 Dimensional cross product with NormVec2
		 * @param rhs the NormVec2 to cross this Vec2 with
		 * @return the cross product scalar
		*/
		constexpr T cross(const NormVec2<T>& rhs) const noexcept {
			return x * rhs.y - y * rhs.x;
		}

		/**
		 * @brief 2 Dimensional cross product with Scalar
		 * @details note that this cross product produces a
		 * CCW facing Vec2 outward according to the right hand rule
		 * @param rhs the scalar to cross this Vec2 with
		 * @return the cross product vector
		*/
		constexpr Vec2 cross(const T& rhs) const noexcept {
			return Vec2(rhs * y, -rhs * x);
		}

		/**
		 * @brief checks if the two vectors are perpindicular
		 * @param rhs the other Vec2 to calculate with
		 * @return returns true if the two vectors are perpindicular
		*/
		bool perp(const Vec2<T>& rhs) const noexcept {
			return std::abs((double)dot(rhs)) < epsilon;
		}

		/**
		 * @brief checks if the two vectors are perpindicular
		 * @param rhs the other NormVec2 to calculate with
		 * @return returns true if the two vectors are perpindicular
		*/
		bool perp(const NormVec2<T>& rhs) const noexcept {
			return std::abs((double)dot(rhs)) < epsilon;
		}

		/**
		 * @brief Implicit conversion to other Vec2 types
		 * @tparam Other the underlying data type of the Vec2 to convert to
		*/
		template<typename Other>
		explicit operator Vec2<Other>() const {
			return Vec2<Other>((Other)x, (Other)y);
		}

#ifdef _SFML_ENABLED


		/**
		 * @brief Implicit conversion function to an sf::Vector2
		 * @tparam SFMLType the template param for the desired sf::Vector2
		 * @return The sf::Vector2 equivalent of this Vec2
		*/
		template<typename SFMLType>
		constexpr operator sf::Vector2<SFMLType>() const noexcept {
			return toSFMLVec<SFMLType>();
		}

		/**
		 * @brief Explicit conversion function to an sf::Vector2
		 * @tparam SFMLType the template param for the desired sf::Vector2
		 * @return The sf::Vector2 equivalent of this Vec2
		*/
		template <typename SFMLType = float>
		constexpr sf::Vector2<SFMLType> toSFMLVec() const noexcept {
			return sf::Vector2<SFMLType>(static_cast<SFMLType>(this->x), static_cast<SFMLType>(this->y));
		}

#endif

		/**
		 * @brief Prints the Vec2
		 * @param os Input stream
		 * @param it The Vec2 to print
		 * @return a reference to the stream for << chaining
		*/
		friend std::ostream& operator << (std::ostream& os, const Vec2& it) {
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