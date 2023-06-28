module;

#include <smmintrin.h>

#ifdef SE_COMPILER_MSVC
#	define DLL_EXPORT __declspec(dllexport)
#else
#	define DLL_EXPORT
#endif

export module SE.SIMD:SSE42;

/* This file contains implementations for SSE4.2 SIMD library operations */
extern "C" {

/* Availability */
DLL_EXPORT auto is_SSE42_available() -> bool {
	return true;
}

/* RETURN */
/* 32-bit integer operations */
DLL_EXPORT auto mul_i32_sse42(const __m128i &a, const __m128i &b) -> __m128i {
	return _mm_mullo_epi32(a, b);
}

/* INPLACE */
/* 32-bit integer operations */
DLL_EXPORT auto mul_i32_sse42_inplace(__m128i &a, const __m128i &b) -> void {
	a = _mm_mullo_epi32(a, b);
}
}