module;

#include <array>

#include <emmintrin.h>
#include <xmmintrin.h>

export module SE.SIMD:SSE2;

import SE.Utilities;

export namespace SE::SIMD {
	/* This file contains function pointers for various SSE2 SIMD library operations */

	/* Availability */
	inline bool is_SSE2_available = false;

	/* RETURN */
	/* 32-bit floating point operations */
	using PFN_f32_sse2 = __m128 (*)(const __m128 &, const __m128 &);
	auto (*add_f32_sse2)(const __m128 &, const __m128 &) -> __m128;
	auto (*sub_f32_sse2)(const __m128 &, const __m128 &) -> __m128;
	auto (*mul_f32_sse2)(const __m128 &, const __m128 &) -> __m128;
	auto (*div_f32_sse2)(const __m128 &, const __m128 &) -> __m128;

	/* 64-bit floating point operations */
	using PFN_f64_sse2 = __m128d (*)(const __m128d &, const __m128d &);
	auto (*add_f64_sse2)(const __m128d &, const __m128d &) -> __m128d;
	auto (*sub_f64_sse2)(const __m128d &, const __m128d &) -> __m128d;
	auto (*mul_f64_sse2)(const __m128d &, const __m128d &) -> __m128d;
	auto (*div_f64_sse2)(const __m128d &, const __m128d &) -> __m128d;

	/* 32-bit integer operations */
	using PFN_i32_sse2 = __m128i (*)(const __m128i &, const __m128i &);
	auto (*add_i32_sse2)(const __m128i &, const __m128i &) -> __m128i;
	auto (*sub_i32_sse2)(const __m128i &, const __m128i &) -> __m128i;

	/* INPLACE */
	/* 32-bit floating point operations */
	using PFN_f32_sse2_inplace = void (*)(__m128 &, const __m128 &);
	auto (*add_f32_sse2_inplace)(__m128 &, const __m128 &) -> void;
	auto (*sub_f32_sse2_inplace)(__m128 &, const __m128 &) -> void;
	auto (*mul_f32_sse2_inplace)(__m128 &, const __m128 &) -> void;
	auto (*div_f32_sse2_inplace)(__m128 &, const __m128 &) -> void;

	/* 64-bit floating point operations */
	using PFN_f64_sse2_inplace = void (*)(__m128d &, const __m128d &);
	auto (*add_f64_sse2_inplace)(__m128d &, const __m128d &) -> void;
	auto (*sub_f64_sse2_inplace)(__m128d &, const __m128d &) -> void;
	auto (*mul_f64_sse2_inplace)(__m128d &, const __m128d &) -> void;
	auto (*div_f64_sse2_inplace)(__m128d &, const __m128d &) -> void;

	/* 32-bit integer operations */
	using PFN_i32_sse2_inplace = void (*)(__m128i &, const __m128i &);
	auto (*add_i32_sse2_inplace)(__m128i &, const __m128i &) -> void;
	auto (*sub_i32_sse2_inplace)(__m128i &, const __m128i &) -> void;


	/* Strings of function pointers in array for easier loading */
	constexpr std::array function_pointers_sse2 = {
			// Availability
			"is_SSE2_available",

			// RETURN
			// 32-bit floating point operations
			"add_f32_sse2", "sub_f32_sse2", "mul_f32_sse2", "div_f32_sse2",
			// 64-bit floating point operations
			"add_f64_sse2", "sub_f64_sse2", "mul_f64_sse2", "div_f64_sse2",
			// 32-bit integer operations
			"add_i32_sse2", "sub_i32_sse2",

			// INPLACE
			// 32-bit floating point operations
			"add_f32_sse2_inplace", "sub_f32_sse2_inplace", "mul_f32_sse2_inplace",
			"div_f32_sse2_inplace",
			// 64-bit floating point operations
			"add_f64_sse2_inplace", "sub_f64_sse2_inplace", "mul_f64_sse2_inplace",
			"div_f64_sse2_inplace",
			// 32-bit integer operations
			"add_i32_sse2_inplace", "sub_i32_sse2_inplace"};


	/* Loads function pointers from the given library handle. */
	auto loadSSE2Functions(void *handle) -> bool {
		// RETURN
		// 32-bit floating point operations
		add_f32_sse2 =
				DynamicLoader::loadFunctionPointer<PFN_f32_sse2>(handle, function_pointers_sse2[1]);
		if (add_f32_sse2 == nullptr)
			return false;
		sub_f32_sse2 =
				DynamicLoader::loadFunctionPointer<PFN_f32_sse2>(handle, function_pointers_sse2[2]);
		if (sub_f32_sse2 == nullptr)
			return false;
		mul_f32_sse2 =
				DynamicLoader::loadFunctionPointer<PFN_f32_sse2>(handle, function_pointers_sse2[3]);
		if (mul_f32_sse2 == nullptr)
			return false;
		div_f32_sse2 =
				DynamicLoader::loadFunctionPointer<PFN_f32_sse2>(handle, function_pointers_sse2[4]);
		if (div_f32_sse2 == nullptr)
			return false;

		// 64-bit floating point operations
		add_f64_sse2 =
				DynamicLoader::loadFunctionPointer<PFN_f64_sse2>(handle, function_pointers_sse2[5]);
		if (add_f64_sse2 == nullptr)
			return false;
		sub_f64_sse2 =
				DynamicLoader::loadFunctionPointer<PFN_f64_sse2>(handle, function_pointers_sse2[6]);
		if (sub_f64_sse2 == nullptr)
			return false;
		mul_f64_sse2 =
				DynamicLoader::loadFunctionPointer<PFN_f64_sse2>(handle, function_pointers_sse2[7]);
		if (mul_f64_sse2 == nullptr)
			return false;
		div_f64_sse2 =
				DynamicLoader::loadFunctionPointer<PFN_f64_sse2>(handle, function_pointers_sse2[8]);
		if (div_f64_sse2 == nullptr)
			return false;

		// 32-bit integer operations
		add_i32_sse2 =
				DynamicLoader::loadFunctionPointer<PFN_i32_sse2>(handle, function_pointers_sse2[9]);
		if (add_i32_sse2 == nullptr)
			return false;

		sub_i32_sse2 = DynamicLoader::loadFunctionPointer<PFN_i32_sse2>(handle,
																		function_pointers_sse2[10]);
		if (sub_i32_sse2 == nullptr)
			return false;


		// INPLACE
		// 32-bit floating point operations
		add_f32_sse2_inplace = DynamicLoader::loadFunctionPointer<PFN_f32_sse2_inplace>(
				handle, function_pointers_sse2[11]);
		if (add_f32_sse2_inplace == nullptr)
			return false;
		sub_f32_sse2_inplace = DynamicLoader::loadFunctionPointer<PFN_f32_sse2_inplace>(
				handle, function_pointers_sse2[12]);
		if (sub_f32_sse2_inplace == nullptr)
			return false;
		mul_f32_sse2_inplace = DynamicLoader::loadFunctionPointer<PFN_f32_sse2_inplace>(
				handle, function_pointers_sse2[13]);
		if (mul_f32_sse2_inplace == nullptr)
			return false;
		div_f32_sse2_inplace = DynamicLoader::loadFunctionPointer<PFN_f32_sse2_inplace>(
				handle, function_pointers_sse2[14]);
		if (div_f32_sse2_inplace == nullptr)
			return false;

		// 64-bit floating point operations
		add_f64_sse2_inplace = DynamicLoader::loadFunctionPointer<PFN_f64_sse2_inplace>(
				handle, function_pointers_sse2[15]);
		if (add_f64_sse2_inplace == nullptr)
			return false;
		sub_f64_sse2_inplace = DynamicLoader::loadFunctionPointer<PFN_f64_sse2_inplace>(
				handle, function_pointers_sse2[16]);
		if (sub_f64_sse2_inplace == nullptr)
			return false;
		mul_f64_sse2_inplace = DynamicLoader::loadFunctionPointer<PFN_f64_sse2_inplace>(
				handle, function_pointers_sse2[17]);
		if (mul_f64_sse2_inplace == nullptr)
			return false;
		div_f64_sse2_inplace = DynamicLoader::loadFunctionPointer<PFN_f64_sse2_inplace>(
				handle, function_pointers_sse2[18]);
		if (div_f64_sse2_inplace == nullptr)
			return false;

		// 32-bit integer operations
		add_i32_sse2_inplace = DynamicLoader::loadFunctionPointer<PFN_i32_sse2_inplace>(
				handle, function_pointers_sse2[19]);
		if (add_i32_sse2_inplace == nullptr)
			return false;

		sub_i32_sse2_inplace = DynamicLoader::loadFunctionPointer<PFN_i32_sse2_inplace>(
				handle, function_pointers_sse2[20]);
		if (sub_i32_sse2_inplace == nullptr)
			return false;

		is_SSE2_available = true;

		return true;
	}
} // namespace SE::SIMD
