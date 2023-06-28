module;

#include <array>
#include <valarray>

export module SE.Maths:Misc;

namespace SE {

	/// Integer lookup array of the next power of 2, with 128 values.
	constexpr std::array<int, 128> luNextPowerOfTwo = {
			1,	 2,	  4,   4,	8,	 8,	  8,   8,	16,	 16,  16,  16,	16,	 16,  16,  16,
			32,	 32,  32,  32,	32,	 32,  32,  32,	32,	 32,  32,  32,	32,	 32,  32,  32,
			64,	 64,  64,  64,	64,	 64,  64,  64,	64,	 64,  64,  64,	64,	 64,  64,  64,
			64,	 64,  64,  64,	64,	 64,  64,  64,	64,	 64,  64,  64,	64,	 64,  64,  64,
			128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
			128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
			128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
			128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128
	};

	/// Internal method for calculating the next power of 2 of the given value.
	constexpr auto iNextPowerOfTwo(int value) -> int {
		--value;
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		return ++value;
	}

	/// Function that returns the next power of 2 of the given value.
	/// Uses lookup table for first 128 values, calculates above that.
	/// @param value The value to get the next power of 2 of.
	/// @return The next power of 2 of the given value.
	export constexpr auto nextPowerOfTwo(int value) -> int {
		return value <= 0 ? 1 : value >= 128 ? iNextPowerOfTwo(value) : luNextPowerOfTwo.at(value);
	}

} // namespace SE
