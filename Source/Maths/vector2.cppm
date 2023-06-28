module;

#include <memory>

#ifdef SE_HAS_SSE2
#include <emmintrin.h>
#include <xmmintrin.h>
#endif

export module SE.Maths:Vector2;

import :Vector;

namespace SE {

	/// 2D Vector subclass, specialized for 2D operations
	export template<typename T>
	class Vector2 : public Vector<T, 2> {
	public:
		/// Default constructor
		Vector2() = default;
		/// Default destructor
		~Vector2() = default;

		/// Construct from elements
		/// @param elements elements to construct from
		template<typename... Elements>
		explicit Vector2(Elements... elements) noexcept : Vector<T, 2>(elements...) {}

		/// Copy constructor
		/// @param other vector to copy
		Vector2(const Vector2<T> &other) noexcept {
			this->elements = other.elements;
		}

		/// Move constructor
		/// @param other vector to move
		Vector2(Vector2<T> &&other) noexcept {
			this->elements = std::move(other.elements);
		}

		/// Copy assignment operator
		/// @param other vector to copy
		auto operator=(const Vector2<T> &other) noexcept -> Vector2<T> & {
			this->elements = other.elements;
			return *this;
		}

		/// Move assignment operator
		/// @param other vector to move
		auto operator=(Vector2<T> &&other) noexcept -> Vector2<T> & {
			this->elements = std::move(other.elements);
			return *this;
		}

		/* Base class compatibility */

		/// Copy constructor - base class
		/// @param other vector to copy
		Vector2(const Vector<T, 2> &other) noexcept {
			this->elements = other.elements;
		}

		/// Move constructor - base class
		/// @param other vector to move
		Vector2(Vector<T, 2> &other) noexcept {
			this->elements = std::move(other.elements);
		}

		/// Copy assignment operator - base class
		/// @param other vector to copy
		auto operator=(const Vector<T, 2> &other) noexcept -> Vector2<T> & {
			this->elements = other.elements;
			return *this;
		}

		/// Move assignment operator - base class
		/// @param other vector to move
		auto operator=(Vector<T, 2> &other) noexcept -> Vector2<T> & {
			this->elements = std::move(other.elements);
			return *this;
		}
	};
}
