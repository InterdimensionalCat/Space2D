#pragma once


#ifndef S2D_DIM_2D_OPERATOR
#define S2D_DIM_2D_OPERATOR
#define S2D_DIM_2D_OP_EQ(op, typ2d) \
    constexpr inline Dim2& operator##op(const typ2d<T>& rhs) noexcept {\
    x op rhs.x;\
    y op rhs.y;\
    return (*this);\
	}

#define S2D_DIM_2D_OP(op, typ2d) \
	constexpr inline Dim2 operator##op(const typ2d<T>& rhs) const noexcept { \
    return Dim2(x op rhs.x, y op rhs.y);\
	}
#endif

#ifndef S2D_DIM_1D_OPERATOR
#define S2D_DIM_1D_OPERATOR
#define S2D_DIM_1D_OP_EQ(op) \
    constexpr inline Dim2& operator##op(const T& rhs) noexcept {\
    x op rhs;\
    y op rhs;\
    return (*this);\
	}

#define S2D_DIM_1D_OP(op) \
	constexpr inline Dim2 operator##op(const T& rhs) const noexcept { \
    return Dim2(x op rhs, y op rhs);\
	}
#endif

namespace Space2D {

    template<typename T>
    class Vec2;
    template<typename T>
    class NormVec2;
    template<typename T>
    class Point2;


    /**
	 * @brief Class representing a 2 Dimensional size, or dimension. 
     Used for widths and heights, or other size related functions
	 * @tparam T Underlying data type of the coordinates
	*/
    template<typename T>
    class Dim2
    {

    public:

        /**
         * @brief Constructs a Dim2 with size (0,0)
         * 
        */
        constexpr Dim2() noexcept : x{}, y{} {}

        /**
         * @brief Constructs a Dim2 with size (x,y)
         * @param x width of the dimension
         * @param y height of the dimension
         * 
        */
        constexpr explicit Dim2(const T& x, const T& y) noexcept : x(x), y(y) {}

        /**
         * @brief Construct a Dim2 directly from a Point2
         * @param other The Point2 to construct from
         * 
        */
        constexpr explicit Dim2(const Point2<T>& other) noexcept : Dim2(other.x, other.y) {}

         /**
         * @brief Construct a Dim2 directly from a Vec2
         * @param other The Vec to construct from
         * 
        */
        constexpr explicit Dim2(const Vec2<T>& other) noexcept : Dim2(other.x, other.y) {}

         /**
         * @brief Construct a Dim2 directly from a NormVec2
         * @param other The NormVec2 to construct from
         * 
        */
        constexpr explicit Dim2(const NormVec2<T>& other) noexcept : Dim2(other.x, other.y) {}

        /**
         * @brief Construct a Dim2 as the width and height of a 
         Rectangle defined by the supplied max and min Point2
         * @param min The maximum Point2 of the Rectangle used for calculations
         * @param max The minimum Point2 of the Rectangle used for calculations
         * @return 
        */
        constexpr explicit Dim2(const Point2<T>& min, const Point2<T>& max) noexcept : Dim2(max.x - min.x, max.y - min.y) {}

        /**
		 * @brief Comparison operators for Dim2, sorted by x coordinate then y coordinate
		 * @param other The Dim2 to compare with
		 * 
		*/
        auto operator<=>(const Dim2<T>& other) const noexcept = default;

        /**
		 * @brief Equality operator for Dim2
		 * @param other The Dim2 to compare with
		 * @return true if the two Dim2's are equal
		*/
        bool operator== (const Dim2<T>& other) const noexcept {
			return std::abs((double)(x - other.x)) < epsilon && std::abs((double)(y - other.y)) < epsilon;
        }

        /**
		 * @brief Access the Dim2 width and height as if it were an array
		 * @param i The index, 0 = x and 1 = y
		 * @return a read only reference to the coordinate
		*/
        const T& operator[] (const size_t i) const {
            if (i > 1) throw std::out_of_range("Dim2 subscript out of range");
            return i == 0 ? x : y;
        }

        /**
		 * @brief Access the Dim2 width and height as if it were an array
		 * @param i The index, 0 = x and 1 = y
		 * @return a read and write reference to the coordinate
		*/
        T& operator[] (const size_t i) {
            if (i > 1) throw std::out_of_range("Dim2 subscript out of range");
            return i == 0 ? x : y;
        }

		/**
		* @brief Read and write iterator for Dim2
		*/
		struct Iterator {

			Iterator(const size_t index, Dim2* val) : index(index), val(val) {}

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
			Dim2* val;
		};

		/**
		 * @brief Read only iterator for Dim2
		*/
		struct CIterator {

			CIterator(const size_t index, const Dim2* val) : index(index), val(val) {}

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
			const Dim2* val;
		};

		/**
		 * @brief Beginning of the Iterator range
		 * @return A const iterator at the beginning of the Dim2
		*/
		const CIterator cbegin() const {
			return CIterator(0, this);
		}

		/**
		 * @brief One past the end of the Iterator range
		 * @return A const iterator one past the end of the Dim2
		*/
		const CIterator cend() const {
			return CIterator(2, this);
		}

		/**
		 * @brief Beginning of the Iterator range
		 * @return An iterator at the beginning of the Dim2
		*/
		Iterator begin() {
			return Iterator(0, this);
		}

		/**
		 * @brief One past the end of the Iterator range
		 * @return An iterator one past the end of the Dim2
		*/
		Iterator end() {
			return Iterator(2, this);
		}


		/**
		 * @brief Implicit conversion to other Dim2 types
		 * @tparam Other the underlying data type of the Dim2 to convert to
		*/
        template<typename Other>
        explicit operator Dim2<Other>() const {
            return Dim2<Other>((Other)x, (Other)y);
        }

#ifdef _SFML_ENABLED
		/**
		 * @brief Implicit conversion function to an sf::Vector2
		 * @tparam SFMLType the template param for the desired sf::Vector2
		 * @return The sf::Vector2 equivalent of this Dim2
		*/
		template<typename SFMLType>
		constexpr operator sf::Vector2<SFMLType>() const noexcept {
			return toSFMLVec<SFMLType>();
		}

		/**
		 * @brief Explicit conversion function to an sf::Vector2
		 * @tparam SFMLType the template param for the desired sf::Vector2
		 * @return The sf::Vector2 equivalent of this Dim2
		*/
		template <typename SFMLType = float>
		constexpr sf::Vector2<SFMLType> toSFMLVec() const noexcept {
			return sf::Vector2<SFMLType>(static_cast<SFMLType>(this->x), static_cast<SFMLType>(this->y));
		}
#endif

		/**
		 * @brief Prints the Dim2
		 * @param os Input stream
		 * @param it The Dim2 to print
		 * @return a reference to the stream for << chaining
		*/
        friend std::ostream& operator << (std::ostream& os, const Dim2& it) {
			std::string typname = typeid(T).name();
			std::string::size_type i = typname.find("struct");
			if (i != std::string::npos) {
				typname.erase(i, 7);
			}
			i = typname.find("class");
			if (i != std::string::npos) {
				typname.erase(i, 6);
			}
			os << "Dim2<" << typname << ">(" << it.x << ", " << it.y << ")";
			return os;
        }

		/**
		 * @brief subtract-eq for Dim2 with Dim2
		 * @param rhs the subtraction Dim2
		*/
		S2D_DIM_2D_OP_EQ(-=, Dim2);

		/**
		 * @brief subtract for Dim2 with Dim2
		 * @param rhs the subtraction Dim2
		*/
		S2D_DIM_2D_OP(-, Dim2);

		/**
		 * @brief subtract-eq for Dim2 with NormVec2
		 * @param rhs the subtraction NormVec2
		*/
		S2D_DIM_2D_OP_EQ(-=, NormVec2);

		/**
		 * @brief subtract for Dim2 with NormVec2
		 * @param rhs the subtraction NormVec2
		*/
		S2D_DIM_2D_OP(-, NormVec2);

		/**
		 * @brief subtract-eq for Dim2 with Vec2
		 * @param rhs the subtraction Vec2
		*/
		S2D_DIM_2D_OP_EQ(-=, Vec2);

		/**
		 * @brief subtract for Dim2 with Vec2
		 * @param rhs the subtraction Vec2
		*/
		S2D_DIM_2D_OP(-, Vec2);


		/**
		 * @brief addition-eq for Dim2 with Dim2
		 * @param rhs the addition Dim2
		*/
		S2D_DIM_2D_OP_EQ(+=, Dim2);

		/**
		 * @brief addition for Dim2 with Dim2
		 * @param rhs the addition Dim2
		*/
		S2D_DIM_2D_OP(+, Dim2);

		/**
		 * @brief addition-eq for Dim2 with NormVec2
		 * @param rhs the addition NormDim2
		*/
		S2D_DIM_2D_OP_EQ(+=, NormVec2);

		/**
		 * @brief addition for Dim2 with NormVec2
		 * @param rhs the addition NormVec2
		*/
		S2D_DIM_2D_OP(+, NormVec2);

		/**
		 * @brief addition-eq for Dim2 with Vec2
		 * @param rhs the addition Dim2
		*/
		S2D_DIM_2D_OP_EQ(+=, Vec2);

		/**
		 * @brief addition for Dim2 with Vec2
		 * @param rhs the addition Vec2
		*/
		S2D_DIM_2D_OP(+, Vec2);

		/**
		 * @brief multiplication-eq for Dim2
		 * @param rhs the factor to multiply by
		*/
		S2D_DIM_1D_OP_EQ(*=);

		/**
		 * @brief multiplication for Dim2
		 * @param rhs the factor to multiply by
		*/
		S2D_DIM_1D_OP(*);

		/**
		 * @brief division-eq for Dim2
		 * @param rhs the divisor
		*/
		S2D_DIM_1D_OP_EQ(/=);

		/**
		 * @brief division for Dim2
		 * @param rhs the divisor
		*/
		S2D_DIM_1D_OP(/);

		/**
		 * @brief Square multiplication for Dim2
		 * @param rhs The other Dim2 to multiply with
		 * @return The multiplied Dim2
		*/
		constexpr Dim2 operator*(const Dim2& rhs) const noexcept {
			return Dim2(x * rhs.x, y * rhs.y);
		}

		/**
		 * @brief Square multiplication-eq for Dim2
		 * @param rhs The other Dim2 to multiply with
		 * @return The multiplied Dim2
		*/
		constexpr Dim2& operator*=(const Dim2<T>& rhs) noexcept {
			x *= rhs.x;
			y *= rhs.y;
			return (*this);
		}

		/**
		 * @brief x coordinate of the Dim2
		*/
		T x;

		/**
		 * @brief y coordinate of the Dim2
		*/
		T y;
    };
}

#undef S2D_DIM_2D_OP
#undef S2D_DIM_2D_OP_EQ
#undef S2D_DIM_1D_OP
#undef S2D_DIM_1D_OP_EQ



