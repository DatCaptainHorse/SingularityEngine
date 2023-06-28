module;

#include <string>
#include <unordered_map>

export module SE.Kernel:EngineKernel;

import :Capabilities;
import SE.Common;
import SE.Utilities;
import SE.SIMD;

namespace SE {

	/* Default dynamic library paths */
	export constexpr auto LIB_SIMD_SSE2 = "sesimdsse2";
	export constexpr auto LIB_SIMD_SSE42 = "sesimdsse42";

	/// The engine kernel class.
	/// Handles loading dynamic components of engine such as SIMD based on CPU capabilities.
	export class EngineKernel {
	public:
		/// The engine kernel configuration.
		/// Contains configurable options for the engine kernel initialization.
		struct Config {
			bool debugMode {true};	   ///< Whether to initialize in debug mode or not.
			bool loadSIMD {true};	   ///< Whether to try to load and use SIMD libraries.
			bool enableLogging {true}; ///< Whether engine kernel should initialize logging.

			/// Constructs the engine kernel configuration with default values.
			Config() {} // NOLINT(*-use-equals-default)
		};

	private:
		inline static bool initialized = false; ///< Whether the engine kernel has been initialized.

		inline static std::unordered_map<std::string, void *> loadedLibraries =
				{}; ///< The loaded libraries.

	public:
		/// Initializes the engine kernel along with necessary components.
		/// @return Whether the engine kernel was initialized successfully.
		static auto Init(const Config &config = Config()) -> bool {
			if (initialized)
				return true; // Already initialized

			// Initialize logger if enabled
			if (config.enableLogging) {
				Logger::Config loggerConfig;
				loggerConfig.logLevel =
						config.debugMode ? Logger::LogLevel::Debug : Logger::LogLevel::Info;
				Logger::Init(loggerConfig);
			}

			// Log initialization
			Logger::Info("Initializing Engine Kernel..");

			// Load SIMD libraries if enabled
			if (config.loadSIMD && !iLoadSIMDLibraries()) {
				Logger::Fatal("Failed to load SIMD libraries.");
				return false;
			}

			initialized = true;
			Logger::Info("Engine Kernel Initialization Success!");
			return true;
		}

		/// Cleans up the engine kernel.
		static auto Clean() -> void {
			if (!initialized)
				return; // Not initialized

			// Log cleanup
			Logger::Info("Cleaning up Engine Kernel..");

			// Free libraries
			for (auto &library : loadedLibraries) {
				if (library.second != nullptr) {
					DynamicLoader::freeDynamicLibrary(library.second);
					library.second = nullptr;
				}
			}

			// Clean up logger
			Logger::Clean();

			initialized = false;
			Logger::Info("Engine Kernel Cleanup Success!");
		}

		/// Gets whether the engine kernel has been initialized.
		/// @return Whether the engine kernel has been initialized.
		static auto isInitialized() -> bool {
			return initialized;
		}

		/// Gets the handle to the given library.
		/// If the library is not loaded or does not exist, nullptr is returned.
		/// @param libraryName The name of the library to get the handle to.
		/// @return The handle to the given library.
		static auto getLibraryHandle(const std::string &libraryName) -> void * {
			auto it = loadedLibraries.find(libraryName);
			if (it == loadedLibraries.end())
				return nullptr;

			return it->second;
		}

	private:
		/// Loads dynamic SIMD libraries based on CPU capabilities.
		/// @return Whether the libraries were loaded successfully.
		static auto iLoadSIMDLibraries() -> bool {
			bool loadedAny = false;
			if (CPUCapabilities::hasSSE2()) {
				Logger::Log("Loading SSE2 SIMD library..");
				const auto res = DynamicLoader::loadDynamicLibrary(LIB_SIMD_SSE2);
				if (res.isSuccess() && SIMD::loadSSE2Functions(res.getValue())) {
					loadedAny = true;
					loadedLibraries[LIB_SIMD_SSE2] = res.getValue();
				} else if (res.isWarning())
					Logger::Warning(res.getMessage());
				else if (res.isFailure()) {
					Logger::Error("Failed to load SSE2 SIMD library");
					return false;
				}
			}

			if (CPUCapabilities::hasSSE42()) {
				Logger::Log("Loading SSE4.2 SIMD library..");
				const auto res = DynamicLoader::loadDynamicLibrary(LIB_SIMD_SSE42);
				if (res.isSuccess() && SIMD::loadSSE42Functions(res.getValue())) {
					loadedAny = true;
					loadedLibraries[LIB_SIMD_SSE42] = res.getValue();
				} else if (res.isWarning())
					Logger::Warning(res.getMessage());
				else if (res.isFailure()) {
					Logger::Error("Failed to load SSE4.2 SIMD library");
					return false;
				}
			}

			if (!loadedAny)
				Logger::Warning("No SIMD libraries available for this CPU");

			return true;
		}
	};
} // namespace SE
