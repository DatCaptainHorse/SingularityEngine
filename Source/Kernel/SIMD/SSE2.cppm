module;

#include <emmintrin.h>
#include <xmmintrin.h>

#ifdef SE_COMPILER_MSVC
#	define DLL_EXPORT __declspec(dllexport)
#else
#	define DLL_EXPORT
#endif

export module SE.SIMD:SSE2;

/* This file contains implementations for SSE2 operations */
extern "C" {

/* Availability */
DLL_EXPORT auto is_SSE2_available() -> bool {
	return true;
}

/* RETURN */
/* 32-bit floating point operations */
DLL_EXPORT auto add_f32_sse2(const __m128 &a, const __m128 &b) -> __m128 {
	return _mm_add_ps(a, b);
}
DLL_EXPORT auto sub_f32_sse2(const __m128 &a, const __m128 &b) -> __m128 {
	return _mm_sub_ps(a, b);
}
DLL_EXPORT auto mul_f32_sse2(const __m128 &a, const __m128 &b) -> __m128 {
	return _mm_mul_ps(a, b);
}
DLL_EXPORT auto div_f32_sse2(const __m128 &a, const __m128 &b) -> __m128 {
	return _mm_div_ps(a, b);
}

/* 64-bit floating point operations */
DLL_EXPORT auto add_f64_sse2(const __m128d &a, const __m128d &b) -> __m128d {
	return _mm_add_pd(a, b);
}
DLL_EXPORT auto sub_f64_sse2(const __m128d &a, const __m128d &b) -> __m128d {
	return _mm_sub_pd(a, b);
}
DLL_EXPORT auto mul_f64_sse2(const __m128d &a, const __m128d &b) -> __m128d {
	return _mm_mul_pd(a, b);
}
DLL_EXPORT auto div_f64_sse2(const __m128d &a, const __m128d &b) -> __m128d {
	return _mm_div_pd(a, b);
}

/* 32-bit integer operations */
DLL_EXPORT auto add_i32_sse2(const __m128i &a, const __m128i &b) -> __m128i {
	return _mm_add_epi32(a, b);
}
DLL_EXPORT auto sub_i32_sse2(const __m128i &a, const __m128i &b) -> __m128i {
	return _mm_sub_epi32(a, b);
}

/* INPLACE */
/* 32-bit floating point operations */
DLL_EXPORT auto add_f32_sse2_inplace(__m128 &a, const __m128 &b) -> void {
	a = _mm_add_ps(a, b);
}
DLL_EXPORT auto sub_f32_sse2_inplace(__m128 &a, const __m128 &b) -> void {
	a = _mm_sub_ps(a, b);
}
DLL_EXPORT auto mul_f32_sse2_inplace(__m128 &a, const __m128 &b) -> void {
	a = _mm_mul_ps(a, b);
}
DLL_EXPORT auto div_f32_sse2_inplace(__m128 &a, const __m128 &b) -> void {
	a = _mm_div_ps(a, b);
}

/* 64-bit floating point operations */
DLL_EXPORT auto add_f64_sse2_inplace(__m128d &a, const __m128d &b) -> void {
	a = _mm_add_pd(a, b);
}
DLL_EXPORT auto sub_f64_sse2_inplace(__m128d &a, const __m128d &b) -> void {
	a = _mm_sub_pd(a, b);
}
DLL_EXPORT auto mul_f64_sse2_inplace(__m128d &a, const __m128d &b) -> void {
	a = _mm_mul_pd(a, b);
}
DLL_EXPORT auto div_f64_sse2_inplace(__m128d &a, const __m128d &b) -> void {
	a = _mm_div_pd(a, b);
}

/* 32-bit integer operations */
DLL_EXPORT auto add_i32_sse2_inplace(__m128i &a, const __m128i &b) -> void {
	a = _mm_add_epi32(a, b);
}
DLL_EXPORT auto sub_i32_sse2_inplace(__m128i &a, const __m128i &b) -> void {
	a = _mm_sub_epi32(a, b);
}
}