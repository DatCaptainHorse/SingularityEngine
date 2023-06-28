module;

#include <array>

#ifdef SE_COMPILER_MSVC
#	include <intrin.h>
#endif

export module SE.Kernel:Capabilities;

namespace SE {

	/// CPU capabilities class.
	/// Contains methods for runtime checking of SIMD capabilities.
	export class CPUCapabilities {
	private:
		/// Internal cpuid for compatibility with multiple compilers.
		/// @param output The output array to store the cpuid result in.
		/// @param functionId The function id to use for the cpuid.
#if defined(SE_COMPILER_GCC) || defined(SE_COMPILER_CLANG)
		static auto cpuid(std::array<int, 4> &output, int functionId) -> void {
			__cpuid(functionId, output[0], output[1], output[2], output[3]);
		}
#elif defined(SE_COMPILER_MSVC)
		static auto cpuid(std::array<int, 4> &output, int functionId) -> void {
			__cpuid(output.data(), functionId);
		}
#endif

	public:
		/// Checks if the CPU supports SSE
		/// @return True if the CPU supports SSE, false otherwise.
		static auto hasSSE() -> bool {
			std::array<int, 4> info = {};
			cpuid(info, 1);
			return (info[3] & (1 << 25)) != 0;
		}

		/// Checks if the CPU supports SSE2
		/// @return True if the CPU supports SSE2, false otherwise.
		static auto hasSSE2() -> bool {
			std::array<int, 4> info = {};
			cpuid(info, 1);
			return (info[3] & (1 << 26)) != 0;
		}

		/// Checks if the CPU supports SSE3
		/// @return True if the CPU supports SSE3, false otherwise.
		static auto hasSSE3() -> bool {
			std::array<int, 4> info = {};
			cpuid(info, 1);
			return (info[2] & (1 << 0)) != 0;
		}

		/// Checks if the CPU supports SSSE3
		/// @return True if the CPU supports SSSE3, false otherwise.
		static auto hasSSSE3() -> bool {
			std::array<int, 4> info = {};
			cpuid(info, 1);
			return (info[2] & (1 << 9)) != 0;
		}

		/// Checks if the CPU supports SSE4.1
		/// @return True if the CPU supports SSE4.1, false otherwise.
		static auto hasSSE41() -> bool {
			std::array<int, 4> info = {};
			cpuid(info, 1);
			return (info[2] & (1 << 19)) != 0;
		}

		/// Checks if the CPU supports SSE4.2
		/// @return True if the CPU supports SSE4.2, false otherwise.
		static auto hasSSE42() -> bool {
			std::array<int, 4> info = {};
			cpuid(info, 1);
			return (info[2] & (1 << 20)) != 0;
		}

		/// Checks if the CPU supports AVX
		/// @return True if the CPU supports AVX, false otherwise.
		static auto hasAVX() -> bool {
			std::array<int, 4> info = {};
			cpuid(info, 1);
			return (info[2] & (1 << 28)) != 0;
		}

		/// Checks if the CPU supports AVX2
		/// @return True if the CPU supports AVX2, false otherwise.
		static auto hasAVX2() -> bool {
			std::array<int, 4> info = {};
			cpuid(info, 7);
			return (info[1] & (1 << 5)) != 0;
		}
	};
} // namespace SE