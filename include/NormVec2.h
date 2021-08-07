#pragma once

namespace Space2D {

    /**
	 * @brief Class encapsulating a 2 Dimensional normal vector representation, being a normal
     * vector, the magnitude is always 1 and each vector represents an angle
	 * NormVec2 does not have operators, although they can be used as the right hand argument to
	 * Vec2 operations, furthermore the X and Y coordinates are non modifiable
	 * @tparam T Underlying data type of the coordinatez
	*/
    template<typename T>
    class NormVec2
    {
    public:

        /**
         * @brief Construct a NormVec2 from an x and y input, the 
         * inputs are automatically normalized to maintain invariants
         * @param x x coordinate input
         * @param y y coordinate input
        */
        constexpr explicit NormVec2(const T& x, const T& y) : x(x / Space2D::sqrt<T>(x * x + y * y)), y(y / Space2D::sqrt<T>(x * x + y * y)) {}

        /**
         * @brief Construct a NormVec2 from a radian angle value
         * @param radians the angle to construct the NormVec2 of
        */
        constexpr explicit NormVec2(const Radians radians) noexcept : 
            x(std::cos(radians.get())), 
            y(std::sin(radians.get())) {}

        /**
		 * @brief (x, y) -> (-x, -y)
		 * @return the sign-negative Vec2
		*/
		constexpr NormVec2 operator-() const noexcept {
			return NormVec2(-x, -y);
		}

		/**
		 * @brief (x, y) -> (+x, +y)
		 * @return the sign-positive Vec2
		*/
		constexpr NormVec2 operator+() const noexcept {
			return NormVec2(+x, +y);
		}

		/**
		 * @brief Access the NormVec2 coords as if it were an array
		 * @param i The index, 0 = x and 1 = y
		 * @return a read only reference to the coordinate
		*/
		constexpr const T& operator[] (const size_t i) const {
			if (i > 1) throw std::out_of_range("NormVec2 subscript out of range");
			return i == 0 ? x : y;
		}


		/**
		 * @brief Read only iterator for NormVec2
		*/
		struct Iterator {

			Iterator(const size_t index, const NormVec2* val) : index(index), val(val) {}

			const T& operator*() const {
				return (*val)[index];
			}
			bool operator==(const Iterator& other) const {
				return index == other.index && val == other.val;
			}
			bool operator!=(const Iterator& other) const {
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
			const NormVec2* val;
		};

		/**
		 * @brief Beginning of the Iterator range
		 * @return An iterator at the beginning of the NormVec2
		*/
		const Iterator begin() const {
			return Iterator(0, this);
		}

		/**
		 * @brief One past the end of the Iterator range
		 * @return An iterator one past the end of the NormVec2
		*/
		const Iterator end() const {
			return Iterator(2, this);
		}

		/**
		 * @brief Comparison operators for NormVec2, sorted by x coordinate then y coordinate
		 * @param other The NormVec2 to compare with
		 *
		*/
		constexpr auto operator<=>(const NormVec2& other) const noexcept = default;


		/**
		 * @brief Equality operator for NormVec2
		 * @param other The NormVec2 to compare with
		 * @return true if the two NormVec2's are equal
		*/
		constexpr bool operator==(const NormVec2 & other) const noexcept {
			return std::abs((double)(x - other.x)) < epsilon && std::abs((double)(y - other.y)) < epsilon;
		}

        /**
         * @brief Calculates the angle of the NormVec2
         * @return The angle of the NormVec2, in Radians
        */
        Radians angle() const noexcept {
            return Radians(std::acos(x));
        }

			/**
		 * @brief Calculates the dot product two NormVec2's
		 * @param rhs the other NormVec2 to calculate with
		 * @return the dot product scalar
		*/
		T dot(const NormVec2<T>& rhs) const noexcept {
			return x * rhs.x + y * rhs.y; 
		}

		/**
		 * @brief Calculates the dot product two NormVec2's
		 * @param rhs the other Vec2 to calculate with
		 * @return the dot product scalar
		*/
		T dot(const Vec2<T>& rhs) const noexcept {
			return x * rhs.x + y * rhs.y; 
		}

		/**
		 * @brief 2 Dimensional cross product with NormVec2
		 * @param rhs the NormVec2 to cross this NormVec2 with
		 * @return the cross product scalar
		*/
		constexpr T cross(const NormVec2& rhs) const noexcept {
			return x * rhs.y - y * rhs.x;
		}

	    /**
		 * @brief 2 Dimensional cross product with Vec2
		 * @param rhs the Vec2 to cross this NormVec2 with
		 * @return the cross product scalar
		*/
		constexpr T cross(const Vec2<T>& rhs) const noexcept {
			return x * rhs.y - y * rhs.x;
		}

		/**
		 * @brief 2 Dimensional cross product with Scalar
		 * @details note that this cross product produces a
		 * CCW facing Vec2 outward according to the right hand rule
		 * @param rhs the scalar to cross this Vec2 with
		 * @return the cross product vector
		*/
		constexpr Vec2<T> cross(const T& rhs) const noexcept {
			return Vec2<T>(rhs * y, -rhs * x);
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
		 * @brief checks if the two vectors are perpindicular
		 * @param rhs the other Vec2 to calculate with
		 * @return returns true if the two vectors are perpindicular
		*/
		bool perp(const Vec2<T>& rhs) const noexcept {
			return std::abs((double)dot(rhs)) < epsilon;
		}

	    /**
		 * @brief Implicit conversion to other NormVec2 types
		 * @tparam Other the underlying data type of the NormVec2 to convert to
		*/
		template<typename Other>
		explicit operator NormVec2<Other>() const {
			return NormVec2<Other>((Other)x, (Other)y);
		}

#ifdef _SFML_ENABLED


		/**
		 * @brief Implicit conversion function to an sf::Vector2
		 * @tparam SFMLType the template param for the desired sf::Vector2
		 * @return The sf::Vector2 equivalent of this NormVec2
		*/
		template<typename SFMLType>
		constexpr operator sf::Vector2<SFMLType>() const noexcept {
			return toSFMLVec<SFMLType>();
		}

		/**
		 * @brief Explicit conversion function to an sf::Vector2
		 * @tparam SFMLType the template param for the desired sf::Vector2
		 * @return The sf::Vector2 equivalent of this NormVec2
		*/
		template <typename SFMLType = float>
		constexpr sf::Vector2<SFMLType> toSFMLVec() const noexcept {
			return sf::Vector2<SFMLType>(static_cast<SFMLType>(this->x), static_cast<SFMLType>(this->y));
		}

#endif

		/**
		 * @brief Prints the NormVec2
		 * @param os Input stream
		 * @param it The NormVec2 to print
		 * @return a reference to the stream for << chaining
		*/
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

		/**
		 * @brief x coordinate of the NormVec2, non-modifyable
		*/
		const T x;

		/**
		 * @brief y coordinate of the NormVec2, non-modifyable
		*/
		const T y;
    };
}



