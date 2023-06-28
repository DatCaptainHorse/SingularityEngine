module;

#include <string>

#if defined(SE_COMPILER_GCC) || defined(SE_COMPILER_CLANG)
#	include <dlfcn.h>
#elif defined(SE_COMPILER_MSVC)
#	define WIN32_LEAN_AND_MEAN
#	define NOMINMAX
#	include <Windows.h>
#endif

export module SE.Utilities:DynamicLoader;

import :FileUtilities;
import :Logging;
import SE.Common;

namespace SE {

	/// Contains utilities for loading dynamic libraries.
	export class DynamicLoader {
	public:
		/// Loads a dynamic library from the given path.
		/// @param libraryPath The path to the library to load.
		/// @return Result Success with handle to the library, Failure if the library could not be
		/// loaded, Warning if the library was not found.
		static auto loadDynamicLibrary(const std::string &libraryPath) -> Result<void *> {
			auto systemCorrected = "lib" + libraryPath + ".so";
#if defined(SE_COMPILER_MSVC)
			systemCorrected = libraryPath + ".dll";
#endif
			// Perform filesystem check first
			if (!FileUtilities::exists(systemCorrected))
				return Result<void *>(Result<void *>::Status::Warning, nullptr,
									  "Library not found: " + systemCorrected);

			void *libraryHandle = nullptr;
#if defined(SE_COMPILER_GCC) || defined(SE_COMPILER_CLANG)
			libraryHandle = dlopen(systemCorrected.c_str(), RTLD_LAZY);
#elif defined(SE_COMPILER_MSVC)
			libraryHandle = static_cast<void *>(LoadLibrary(systemCorrected.c_str()));
#endif
			return Result<void *>(libraryHandle != nullptr ? Result<void *>::Status::Success
														   : Result<void *>::Status::Failure,
								  libraryHandle);
		}

		/// Frees a dynamic library from the given handle.
		/// @param libraryHandle The handle to the library to free.
		static auto freeDynamicLibrary(void *libraryHandle) -> void {
#if defined(SE_COMPILER_GCC) || defined(SE_COMPILER_CLANG)
			dlclose(libraryHandle);
#elif defined(SE_COMPILER_MSVC)
			FreeLibrary(static_cast<HMODULE>(libraryHandle));
#endif
		}

		/// Loads function pointer from the given library handle.
		/// @param libraryHandle The handle to the library to load the function pointer from.
		/// @param functionName The name of the function to load.
		/// @return The function pointer.
		template<typename T>
		static auto loadFunctionPointer(void *libraryHandle, const std::string &functionName) -> T {
			void *result = nullptr;
#if defined(SE_COMPILER_GCC) || defined(SE_COMPILER_CLANG)
			result = static_cast<void *>(dlsym(libraryHandle, functionName.c_str()));
#elif defined(SE_COMPILER_MSVC)
			result = reinterpret_cast<void *>(
					GetProcAddress(static_cast<HMODULE>(libraryHandle), functionName.c_str()));
			if (result == nullptr) {
				const auto errorCode = GetLastError();
				Logger::Error("Failed to load function pointer: {} (error code: {})",
							  functionName, errorCode);
				return nullptr;
			}
#endif
			return reinterpret_cast<T>(result);
		}
	};
} // namespace SE