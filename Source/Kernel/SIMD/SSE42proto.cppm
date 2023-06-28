module;

#include <array>

#include <smmintrin.h>

export module SE.SIMD:SSE42;

import SE.Utilities;

export namespace SE::SIMD {
	/* This file contains function pointers for various SSE4.2 SIMD library operations */

	/* Availability */
	inline bool is_SSE42_available = false;

	/* RETURN */
	/* 32-bit integer operations */
	using PFN_i32_sse42 = __m128i (*)(const __m128i &, const __m128i &);
	auto (*mul_i32_sse42)(const __m128i &a, const __m128i &b) -> __m128i;

	/* INPLACE */
	/* 32-bit integer operations */
	using PFN_i32_sse42_inplace = void (*)(__m128i &, const __m128i &);
	auto (*mul_i32_sse42_inplace)(__m128i &a, const __m128i &b) -> void;


	/* Strings of function pointers in array for easier loading */
	constexpr std::array function_pointers_sse42 = {
			// Availability
			"is_SSE42_available",

			// RETURN
			// 32-bit integer operations
			"mul_i32_sse42",

			// INPLACE
			// 32-bit integer operations
			"mul_i32_sse42_inplace"};


	/* Loads function pointers from the given library handle. */
	auto loadSSE42Functions(void *handle) -> bool {
		// RETURN
		// 32-bit integer operations
		mul_i32_sse42 = DynamicLoader::loadFunctionPointer<PFN_i32_sse42>(
				handle, function_pointers_sse42[1]);
		if (mul_i32_sse42 == nullptr)
			return false;


		// INPLACE
		// 32-bit integer operations
		mul_i32_sse42_inplace = DynamicLoader::loadFunctionPointer<PFN_i32_sse42_inplace>(
				handle, function_pointers_sse42[2]);
		if (mul_i32_sse42_inplace == nullptr)
			return false;

		is_SSE42_available = true;

		return true;
	}
} // namespace SE::SIMD
