module;

#include <array>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <valarray>

#include <emmintrin.h>
#include <smmintrin.h>
#include <xmmintrin.h>

export module SE.Maths:Vector;

import SE.Kernel;
import SE.SIMD;
import :Misc;

namespace SE {

	/// Vector data class, uses templates to choose right intrinsic type if available
	template<typename T, std::size_t N>
	class VectorData {
	private:
		using intrinsic_type = typename std::conditional_t<
				std::is_same_v<T, float>, __m128,
				typename std::conditional_t<
						std::is_same_v<T, double>, __m128d,
						typename std::conditional_t<std::is_integral_v<T>, __m128i, __m128>>>;

		/// Returns true if size is between 2 and 4, inclusive, optimal for SIMD
		[[nodiscard]] constexpr static auto canUseSIMD() noexcept -> bool {
			return N >= 2 && N <= 4 && std::is_arithmetic_v<T>;
		}

	protected:
		union {
			intrinsic_type intrinsic;
			alignas(sizeof(T) * nextPowerOfTwo(N)) std::array<T, N> elements {};
		};

		/* Arithmetics - Default/Fallback loops (both return and self) */

		auto loopAdd(const VectorData<T, N> &other) const noexcept -> VectorData<T, N> {
			VectorData<T, N> result;
			for (std::size_t i = 0; i < N; i++)
				result.elements[i] = elements[i] + other.elements[i];

			return result;
		}

		auto loopAddSelf(const VectorData<T, N> &other) noexcept -> void {
			for (std::size_t i = 0; i < N; i++)
				elements[i] += other.elements[i];
		}

		auto loopSub(const VectorData<T, N> &other) const noexcept -> VectorData<T, N> {
			VectorData<T, N> result;
			for (std::size_t i = 0; i < N; i++)
				result.elements[i] = elements[i] - other.elements[i];

			return result;
		}

		auto loopSubSelf(const VectorData<T, N> &other) noexcept -> void {
			for (std::size_t i = 0; i < N; i++)
				elements[i] -= other.elements[i];
		}

		auto loopMul(const VectorData<T, N> &other) const noexcept -> VectorData<T, N> {
			VectorData<T, N> result;
			for (std::size_t i = 0; i < N; i++)
				result.elements[i] = elements[i] * other.elements[i];

			return result;
		}

		auto loopMulSelf(const VectorData<T, N> &other) noexcept -> void {
			for (std::size_t i = 0; i < N; i++)
				elements[i] *= other.elements[i];
		}

		auto loopDiv(const VectorData<T, N> &other) const noexcept -> VectorData<T, N> {
			VectorData<T, N> result;
			for (std::size_t i = 0; i < N; i++)
				result.elements[i] = elements[i] / other.elements[i];

			return result;
		}

		auto loopDivSelf(const VectorData<T, N> &other) noexcept -> void {
			for (std::size_t i = 0; i < N; i++)
				elements[i] /= other.elements[i];
		}

		/* Arithmetics - with return */

		template<typename U, std::enable_if_t<std::is_same_v<U, float>> * = nullptr>
		auto simdAdd(const VectorData<U, N> &other) const -> VectorData<U, N> {
			if (SIMD::is_SSE2_available) {
				const auto res = SIMD::add_f32_sse2(intrinsic, other.intrinsic);
				return VectorData<U, N>(res);
			}
			return loopAdd(other);
		}

		template<typename U, std::enable_if_t<std::is_same_v<U, double>> * = nullptr>
		auto simdAdd(const VectorData<U, N> &other) const -> VectorData<U, N> {
			if (SIMD::is_SSE2_available) {
				const auto res = SIMD::add_f64_sse2(intrinsic, other.intrinsic);
				return VectorData<U, N>(res);
			}
			return loopAdd(other);
		}

		template<typename U, std::enable_if_t<std::is_same_v<U, int>> * = nullptr>
		auto simdAdd(const VectorData<U, N> &other) const -> VectorData<U, N> {
			if (SIMD::is_SSE2_available) {
				const auto res = SIMD::add_i32_sse2(intrinsic, other.intrinsic);
				return VectorData<U, N>(res);
			}
			return loopAdd(other);
		}

		template<typename U,
				 std::enable_if_t<!std::is_same_v<U, double> && !std::is_same_v<U, float> &&
								  !std::is_same_v<U, int>> * = nullptr>
		auto simdAdd(const VectorData<U, N> &other) const -> VectorData<U, N> {
			return loopAdd(other);
		}

		auto vdAdd(const VectorData<T, N> &other) const noexcept -> VectorData<T, N> {
			return canUseSIMD() ? simdAdd<T>(other) : loopAdd(other);
		}

		template<typename U, std::enable_if_t<std::is_same_v<U, float>> * = nullptr>
		auto simdSub(const VectorData<U, N> &other) const -> VectorData<U, N> {
			if (SIMD::is_SSE2_available) {
				const auto res = SIMD::sub_f32_sse2(intrinsic, other.intrinsic);
				return VectorData<U, N>(res);
			}
			return loopSub(other);
		}

		template<typename U, std::enable_if_t<std::is_same_v<U, double>> * = nullptr>
		auto simdSub(const VectorData<U, N> &other) const -> VectorData<U, N> {
			if (SIMD::is_SSE2_available) {
				const auto res = SIMD::sub_f64_sse2(intrinsic, other.intrinsic);
				return VectorData<U, N>(res);
			}
			return loopSub(other);
		}

		template<typename U, std::enable_if_t<std::is_same_v<U, int>> * = nullptr>
		auto simdSub(const VectorData<U, N> &other) const -> VectorData<U, N> {
			if (SIMD::is_SSE2_available) {
				const auto res = SIMD::sub_i32_sse2(intrinsic, other.intrinsic);
				return VectorData<U, N>(res);
			}
			return loopSub(other);
		}

		template<typename U,
				 std::enable_if_t<!std::is_same_v<U, double> && !std::is_same_v<U, float> &&
								  !std::is_same_v<U, int>> * = nullptr>
		auto simdSub(const VectorData<U, N> &other) const -> VectorData<U, N> {
			return loopSub(other);
		}

		auto vdSub(const VectorData<T, N> &other) const noexcept -> VectorData<T, N> {
			return canUseSIMD() ? simdSub<T>(other) : loopSub(other);
		}

		template<typename U, std::enable_if_t<std::is_same_v<U, float>> * = nullptr>
		auto simdMul(const VectorData<U, N> &other) const -> VectorData<U, N> {
			if (SIMD::is_SSE2_available) {
				const auto res = SIMD::mul_f32_sse2(intrinsic, other.intrinsic);
				return VectorData<U, N>(res);
			}
			return loopMul(other);
		}

		template<typename U, std::enable_if_t<std::is_same_v<U, double>> * = nullptr>
		auto simdMul(const VectorData<U, N> &other) const -> VectorData<U, N> {
			if (SIMD::is_SSE2_available) {
				const auto res = SIMD::mul_f64_sse2(intrinsic, other.intrinsic);
				return VectorData<U, N>(res);
			}
			return loopMul(other);
		}

		template<typename U, std::enable_if_t<std::is_same_v<U, int>> * = nullptr>
		auto simdMul(const VectorData<U, N> &other) const -> VectorData<U, N> {
			if (SIMD::is_SSE42_available) {
				const auto res = SIMD::mul_i32_sse42(intrinsic, other.intrinsic);
				return VectorData<U, N>(res);
			}
			return loopMul(other);
		}

		template<typename U,
				 std::enable_if_t<!std::is_same_v<U, double> && !std::is_same_v<U, float> &&
								  !std::is_same_v<U, int>> * = nullptr>
		auto simdMul(const VectorData<U, N> &other) const -> VectorData<U, N> {
			return loopMul(other);
		}

		auto vdMul(const VectorData<T, N> &other) const noexcept -> VectorData<T, N> {
			return canUseSIMD() ? simdMul<T>(other) : loopMul(other);
		}

		template<typename U, std::enable_if_t<std::is_same_v<U, float>> * = nullptr>
		auto simdDiv(const VectorData<U, N> &other) const -> VectorData<U, N> {
			if (SIMD::is_SSE2_available) {
				const auto res = SIMD::div_f32_sse2(intrinsic, other.intrinsic);
				return VectorData<U, N>(res);
			}
			return loopDiv(other);
		}

		template<typename U, std::enable_if_t<std::is_same_v<U, double>> * = nullptr>
		auto simdDiv(const VectorData<U, N> &other) const -> VectorData<U, N> {
			if (SIMD::is_SSE2_available) {
				const auto res = SIMD::div_f64_sse2(intrinsic, other.intrinsic);
				return VectorData<U, N>(res);
			}
			return loopDiv(other);
		}

		template<typename U, std::enable_if_t<!std::is_same_v<U, double> &&
											  !std::is_same_v<U, float>> * = nullptr>
		auto simdDiv(const VectorData<U, N> &other) const -> VectorData<U, N> {
			return loopDiv(other);
		}

		auto vdDiv(const VectorData<T, N> &other) const noexcept -> VectorData<T, N> {
			return canUseSIMD() ? simdDiv<T>(other) : loopDiv(other);
		}

		/* Arithmetics - for self */

		auto vdAddSelf(const VectorData<T, N> &other) noexcept -> void {
			if constexpr (canUseSIMD()) {
				if (std::is_same_v<T, float> && SIMD::is_SSE2_available)
					SIMD::add_f32_sse2_inplace(intrinsic, other.intrinsic);
				else if (std::is_same_v<T, double> && SIMD::is_SSE2_available)
					SIMD::add_f64_sse2_inplace(intrinsic, other.intrinsic);
				else if (std::is_integral_v<T> && SIMD::is_SSE2_available)
					SIMD::add_i32_sse2_inplace(intrinsic, other.intrinsic);
				else
					loopAddSelf(other);
			} else
				loopAddSelf(other);
		}

		auto vdSubSelf(const VectorData<T, N> &other) noexcept -> void {
			if constexpr (canUseSIMD()) {
				if (std::is_same_v<T, float> && SIMD::is_SSE2_available)
					SIMD::sub_f32_sse2_inplace(intrinsic, other.intrinsic);
				else if (std::is_same_v<T, double> && SIMD::is_SSE2_available)
					SIMD::sub_f64_sse2_inplace(intrinsic, other.intrinsic);
				else if (std::is_integral_v<T> && SIMD::is_SSE2_available)
					SIMD::sub_i32_sse2_inplace(intrinsic, other.intrinsic);
				else
					loopSubSelf(other);
			} else
				loopSubSelf(other);
		}

		auto vdMulSelf(const VectorData<T, N> &other) noexcept -> void {
			if constexpr (canUseSIMD()) {
				if (std::is_same_v<T, float> && SIMD::is_SSE2_available)
					SIMD::mul_f32_sse2_inplace(intrinsic, other.intrinsic);
				else if (std::is_same_v<T, double> && SIMD::is_SSE2_available)
					SIMD::mul_f64_sse2_inplace(intrinsic, other.intrinsic);
				else if (std::is_integral_v<T> && SIMD::is_SSE42_available)
					SIMD::mul_i32_sse42_inplace(intrinsic, other.intrinsic);
				else
					loopMulSelf(other);
			} else
				loopMulSelf(other);
		}

		auto vdDivSelf(const VectorData<T, N> &other) noexcept -> void {
			if constexpr (canUseSIMD()) {
				if (std::is_same_v<T, float> && SIMD::is_SSE2_available)
					SIMD::div_f32_sse2_inplace(intrinsic, other.intrinsic);
				else if (std::is_same_v<T, double> && SIMD::is_SSE2_available)
					SIMD::div_f64_sse2_inplace(intrinsic, other.intrinsic);
				else
					loopDivSelf(other);
			} else
				loopDivSelf(other);
		}

	public:
		/// Construct from elements
		/// @param elements elements to construct from
		template<typename... Elements>
		explicit VectorData(Elements... elements) noexcept : elements {elements...} {}

		/// Explicit intrinsic_type constructor
		/// @param intrinsic intrinsic to construct from
		explicit VectorData(intrinsic_type intrinsic) noexcept {
			this->intrinsic = intrinsic;
		}
	};


	/// General template Vector class, arbitrary size and type
	export template<typename T, std::size_t N>
	class Vector : public VectorData<T, N> {
	public:
		/// Default constructor
		Vector() = default;
		/// Default destructor
		~Vector() = default;

		/// Construct from elements
		/// @param elements elements to construct from
		template<typename... Elements>
		explicit Vector(Elements... elements) noexcept : VectorData<T, N>(elements...) {}

		/// Copy constructor
		/// @param other vector to copy
		Vector(const Vector<T, N> &other) noexcept {
			this->elements = other.elements;
		}

		/// Move constructor
		/// @param other vector to move
		Vector(Vector<T, N> &&other) noexcept {
			this->elements = std::move(other.elements);
		}

		/// Copy assignment operator
		/// @param other vector to copy
		auto operator=(const Vector<T, N> &other) noexcept -> Vector<T, N> & {
			this->elements = other.elements;
			return *this;
		}

		/// Move assignment operator
		/// @param other vector to move
		auto operator=(Vector<T, N> &&other) noexcept -> Vector<T, N> & {
			this->elements = std::move(other.elements);
			return *this;
		}

		/// Access operator
		/// @param i index to access
		/// @return reference to element at index i
		auto operator[](std::size_t i) noexcept -> T & {
			return this->elements[i];
		}

		/// Const access operator
		/// @param i index to access
		/// @return const reference to element at index i
		auto operator[](std::size_t i) const noexcept -> const T & {
			return this->elements[i];
		}

		/// Three-way comparison operator
		/// @param other vector to compare
		/// @return result of comparison
		auto operator<=>(const Vector<T, N> &other) const noexcept -> std::strong_ordering {
			return this->elements <=> other.elements;
		}

		/// Returns element at index with bounds checking
		/// @param i index to access
		/// @return reference to element at index i
		auto at(std::size_t i) const -> const T & {
			if (i >= N)
				throw std::out_of_range("index out of range");

			return this->elements.at(i);
		}

		/// Returns the size of the vector
		/// @return size of the vector
		[[nodiscard]] auto size() const noexcept -> std::size_t {
			return N;
		}

		/// Returns the internal data of the vector
		/// @return data of the vector
		auto data() const noexcept -> const std::array<T, N> & {
			return this->elements;
		}

		/* Basic methods */

		/// Magnitude of the vector
		/// @return magnitude of the vector
		auto magnitude() const noexcept -> T {
			return std::sqrt(dot(*this));
		}

		/// Normalized vector
		/// @return normalized vector
		auto normalized() const noexcept -> Vector<T, N> {
			return *this / magnitude();
		}

		/// Dot product against another vector
		/// @param other vector to dot against
		/// @return dot product
		auto dot(const Vector<T, N> &other) const noexcept -> T {
			return std::inner_product(this->elements.begin(), this->elements.end(),
									  other.elements.begin(), T {});
		}

		/* Arithmetics */

		/// Addition
		/// @param other vector to add
		/// @return sum of vectors
		auto add(const Vector<T, N> &other) const noexcept -> Vector<T, N> {
			return Vector<T, N>(this->vdAdd(other));
		}

		/// Addition operator
		/// @param other vector to add
		/// @return sum of vectors
		auto operator+(const Vector<T, N> &other) const noexcept -> Vector<T, N> {
			return add(other);
		}

		/// Addition assign operator
		/// @param other vector to add
		/// @return sum of vectors
		auto operator+=(const Vector<T, N> &other) noexcept -> Vector<T, N> & {
			this->vdAddSelf(other);
			return *this;
		}

		/// Subtraction
		/// @param other vector to subtract
		/// @return difference of vectors
		auto subtract(const Vector<T, N> &other) const noexcept -> Vector<T, N> {
			return Vector<T, N>(this->vdSub(other));
		}

		/// Subtraction operator
		/// @param other vector to subtract
		/// @return difference of vectors
		auto operator-(const Vector<T, N> &other) const noexcept -> Vector<T, N> {
			return subtract(other);
		}

		/// Subtraction assign operator
		/// @param other vector to subtract
		/// @return difference of vectors
		auto operator-=(const Vector<T, N> &other) noexcept -> Vector<T, N> & {
			this->vdSubSelf(other);
			return *this;
		}

		/// Multiplication
		/// @param other vector to multiply
		/// @return product of vectors
		auto multiply(const Vector<T, N> &other) const noexcept -> Vector<T, N> {
			return Vector<T, N>(this->vdMul(other));
		}

		/// Multiplication operator
		/// @param other vector to multiply
		/// @return product of vectors
		auto operator*(const Vector<T, N> &other) const noexcept -> Vector<T, N> {
			return multiply(other);
		}

		/// Multiplication assign operator
		/// @param other vector to multiply
		/// @return product of vectors
		auto operator*=(const Vector<T, N> &other) noexcept -> Vector<T, N> & {
			this->vdMultiplySelf(other);
			return *this;
		}

		/// Division
		/// @param other vector to divide
		/// @return quotient of vectors
		auto divide(const Vector<T, N> &other) const noexcept -> Vector<T, N> {
			return Vector<T, N>(this->vdDiv(other));
		}

		/// Division operator
		/// @param other vector to divide
		/// @return quotient of vectors
		auto operator/(const Vector<T, N> &other) const noexcept -> Vector<T, N> {
			return divide(other);
		}

		/// Division assign operator
		/// @param other vector to divide
		/// @return quotient of vectors
		auto operator/=(const Vector<T, N> &other) noexcept -> Vector<T, N> & {
			this->vdDivSelf(other);
			return *this;
		}
	};
} // namespace SE
