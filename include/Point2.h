#pragma once


#ifndef S2D_POINT_2D_OPERATOR
#define S2D_POINT_2D_OPERATOR
#define S2D_POINT_2D_OP_EQ(op, typ2d) \
    constexpr inline Point2& operator##op(const typ2d<T>& rhs) noexcept {\
    x op rhs.x;\
    y op rhs.y;\
    return (*this);\
	}

#define S2D_POINT_2D_OP(op, typ2d) \
	constexpr inline Point2 operator##op(const typ2d<T>& rhs) const noexcept { \
    return Point2(x op rhs.x, y op rhs.y);\
	}
#endif

#ifndef S2D_POINT_1D_OPERATOR
#define S2D_POINT_1D_OPERATOR
#define S2D_POINT_1D_OP_EQ(op) \
    constexpr inline Point2& operator##op(const T& rhs) noexcept {\
    x op rhs;\
    y op rhs;\
    return (*this);\
	}

#define S2D_POINT_1D_OP(op) \
	constexpr inline Point2 operator##op(const T& rhs) const noexcept { \
    return Point2(x op rhs, y op rhs);\
	}
#endif

namespace Space2D {

    template<typename T>
    class Vec2;
    template<typename T>
    class NormVec2;
    template<typename T>
    class Dim2;

    /**
	 * @brief Class encapsulating a 2 Dimensional Point representation
	 * @tparam T Underlying data type of the coordinates
	*/
    template<typename T>
    class Point2
    {
    public:

        /**
         * @brief Constructs a Point2 at the point (0,0)
         * 
        */
        constexpr Point2() noexcept : x(), y() {}

        /**
         * @brief Constructs a Point2 at the point (x,y)
         * @param x The x coordinate of the point
         * @param y The y coordinate of the point
         * 
        */
        constexpr explicit Point2(const T& x, const T& y) noexcept : x(x), y(y) {}

        /**
         * @brief Constructs a Point2 directly from a Vec2
         * @param other The Vec2 to construct from
         * 
        */
        constexpr explicit Point2(const Vec2<T>& other) noexcept : x(other.x), y(other.y) {}
        
        /**
         * @brief Construct a Point2 at an offset from some base point (base.x + offset.x, base.y + offset.y)
         * @param base The base Point2
         * @param offset The offset Vec2
         * 
        */
        constexpr explicit Point2(const Point2& base, const Vec2<T>& offset) noexcept : Point2(base.x + offset.x, base.y + offset.y) {}

        /**
         * @brief Construct a Point2 at an offset from some base point (base.x + offset.x, base.y + offset.y)
         * @param base The base Point2
         * @param offset The offset NormVec2
         * 
        */
        constexpr explicit Point2(const Point2& base, const NormVec2<T>& offset) noexcept : Point2(base.x + offset.x, base.y + offset.y) {}

        /**
		 * @brief Comparison operators for Point2, sorted by x coordinate then y coordinate
		 * @param other The Point2 to compare with
		 * 
		*/
        constexpr auto operator<=>(const Point2<T>&) const noexcept = default;

        /**
		 * @brief Equality operator for Point2
		 * @param other The Point2 to compare with
		 * @return true if the two Point2's are equal
		*/
        constexpr bool operator== (const Point2<T>& other) const noexcept {
			return std::abs((double)(x - other.x)) < epsilon && std::abs((double)(y - other.y)) < epsilon;
        }


        /**
		 * @brief Access the Point2 coords as if it were an array
		 * @param i The index, 0 = x and 1 = y
		 * @return a read only reference to the coordinate
		*/
        constexpr const T& operator[] (const size_t i) const {
            if (i > 1) throw std::out_of_range("Point2 subscript out of range");
            return i == 0 ? x : y;
        }

        /**
		 * @brief Access the Point2 coords as if it were an array
		 * @param i The index, 0 = x and 1 = y
		 * @return a read and write reference to the coordinate
		*/
        constexpr T& operator[] (const size_t i) {
            if (i > 1) throw std::out_of_range("Point2 subscript out of range");
            return i == 0 ? x : y;
        }

		/**
	    * @brief Read and write iterator for Point2
		*/
		struct Iterator {

			Iterator(const size_t index, Point2* val) : index(index), val(val) {}

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
			Point2* val;
		};

		/**
		 * @brief Read only iterator for Point2
		*/
		struct CIterator {

			CIterator(const size_t index, const Point2* val) : index(index), val(val) {}

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
			const Point2* val;
		};

		/**
		 * @brief Beginning of the Iterator range
		 * @return A const iterator at the beginning of the Point2
		*/
		const CIterator cbegin() const {
			return CIterator(0, this);
		}

		/**
		 * @brief One past the end of the Iterator range
		 * @return A const iterator one past the end of the Point2
		*/
		const CIterator cend() const {
			return CIterator(2, this);
		}

		/**
		 * @brief Beginning of the Iterator range
		 * @return An iterator at the beginning of the Point2
		*/
		Iterator begin() {
			return Iterator(0, this);
		}

		/**
		 * @brief One past the end of the Iterator range
		 * @return An iterator one past the end of the Point2
		*/
		Iterator end() {
			return Iterator(2, this);
		}

		/**
		 * @brief Implicit conversion to other Point2 types
		 * @tparam Other the underlying data type of the Point2 to convert to
		*/
		template<typename Other>
		explicit operator Point2<Other>() const {
			return Point2<Other>((Other)x, (Other)y);
		}


#ifdef _SFML_ENABLED
		/**
		 * @brief Implicit conversion function to an sf::Vector2
		 * @tparam SFMLType the template param for the desired sf::Vector2
		 * @return The sf::Vector2 equivalent of this Point2
		*/
		template<typename SFMLType>
		constexpr operator sf::Vector2<SFMLType>() const noexcept {
			return toSFMLVec<SFMLType>();
		}

		/**
		 * @brief Explicit conversion function to an sf::Vector2
		 * @tparam SFMLType the template param for the desired sf::Vector2
		 * @return The sf::Vector2 equivalent of this Point2
		*/
		template <typename SFMLType = float>
		constexpr sf::Vector2<SFMLType> toSFMLVec() const noexcept {
			return sf::Vector2<SFMLType>(static_cast<SFMLType>(this->x), static_cast<SFMLType>(this->y));
		}
#endif

		/**
		 * @brief Prints the Point2
		 * @param os Input stream
		 * @param it The Point2 to print
		 * @return a reference to the stream for << chaining
		*/
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


		/**
		 * @brief subtract-eq for Point2 with Point2
		 * @param rhs the subtraction Point2
		*/
		S2D_POINT_2D_OP_EQ(-=, Point2);

		/**
		 * @brief subtract for Point2 with Point2
		 * @param rhs the subtraction Point2
		*/
		S2D_POINT_2D_OP(-, Point2);

		/**
		 * @brief subtract-eq for Point2 with NormVec2
		 * @param rhs the subtraction NormVec2
		*/
		S2D_POINT_2D_OP_EQ(-=, NormVec2);

		/**
		 * @brief subtract for Point2 with NormVec2
		 * @param rhs the subtraction NormVec2
		*/
		S2D_POINT_2D_OP(-, NormVec2);

		/**
		 * @brief subtract-eq for Point2 with Vec2
		 * @param rhs the subtraction Vec2
		*/
		S2D_POINT_2D_OP_EQ(-=, Vec2);

		/**
		 * @brief subtract for Point2 with Vec2
		 * @param rhs the subtraction Vec2
		*/
		S2D_POINT_2D_OP(-, Vec2);

		/**
		 * @brief subtract-eq for Point2 with Dim2
		 * @param rhs the subtraction Dim2
		*/
		S2D_POINT_2D_OP_EQ(-=, Dim2);

		/**
		 * @brief subtract for Point2 with Dim2
		 * @param rhs the subtraction Dim2
		*/
		S2D_POINT_2D_OP(-, Dim2);


		/**
		 * @brief addition-eq for Point2 with Point2
		 * @param rhs the addition Point2
		*/
		S2D_POINT_2D_OP_EQ(+=, Point2);

		/**
		 * @brief addition for Point2 with Point2
		 * @param rhs the addition Point2
		*/
		S2D_POINT_2D_OP(+, Point2);

		/**
		 * @brief addition-eq for Point2 with NormVec2
		 * @param rhs the addition NormPoint2
		*/
		S2D_POINT_2D_OP_EQ(+=, NormVec2);

		/**
		 * @brief addition for Point2 with NormVec2
		 * @param rhs the addition NormVec2
		*/
		S2D_POINT_2D_OP(+, NormVec2);

		/**
		 * @brief addition-eq for Point2 with Vec2
		 * @param rhs the addition Point2
		*/
		S2D_POINT_2D_OP_EQ(+=, Vec2);

		/**
		 * @brief addition for Point2 with Vec2
		 * @param rhs the addition Vec2
		*/
		S2D_POINT_2D_OP(+, Vec2);

		/**
		 * @brief addition-eq for Point2 with Vec2
		 * @param rhs the addition Point2
		*/
		S2D_POINT_2D_OP_EQ(+=, Dim2);

		/**
		 * @brief addition for Point2 with Vec2
		 * @param rhs the addition Vec2
		*/
		S2D_POINT_2D_OP(+, Dim2);


		/**
		 * @brief multiplication-eq for Point2
		 * @param rhs the factor to multiply by
		*/
		S2D_POINT_1D_OP_EQ(*=);

		/**
		 * @brief multiplication for Point2
		 * @param rhs the factor to multiply by
		*/
		S2D_POINT_1D_OP(*);

		/**
		 * @brief division-eq for Point2
		 * @param rhs the divisor
		*/
		S2D_POINT_1D_OP_EQ(/=);

		/**
		 * @brief division for Point2
		 * @param rhs the divisor
		*/
		S2D_POINT_1D_OP(/);




		/**
		 * @brief x coordinate of the Point2
		*/
		T x;

		/**
		 * @brief y coordinate of the Point2
		*/
		T y;
    };
}

#undef S2D_POINT_2D_OP
#undef S2D_POINT_2D_OP_EQ
#undef S2D_POINT_1D_OP
#undef S2D_POINT_1D_OP_EQ