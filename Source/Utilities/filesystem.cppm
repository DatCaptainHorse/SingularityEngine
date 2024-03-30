module;

#include <cstdint>
#include <expected>
#include <filesystem>

export module SE.Utilities:Filesystem;

namespace SE {

	/// Filesystem error codes
	export enum class FilesystemError : std::uint8_t {
		eParamError, ///< Error in given parameters
	};

	/// Filesystem class
	/// Contains helpers for filesystem operations
	export class Filesystem {
		inline static std::filesystem::path m_rootPath = std::filesystem::current_path();

	public:
		/// Sets the root path for the filesystem from passed main argc, argv parameters
		/// @param argc main argc parameter
		/// @param argv main argv parameter
		/// @return std::expected<std::filesystem::path, FilesystemError> root path
		static auto SetRootPath(const int argc, const char **argv)
			-> std::expected<std::filesystem::path, FilesystemError> {
			if (argc < 1 || argv == nullptr) return std::unexpected(FilesystemError::eParamError);

			m_rootPath = std::filesystem::path(argv[0]).parent_path();
			return m_rootPath;
		}

		/// Resets the root path to the current path reported by std::filesystem::current_path
		/// @return std::filesystem::path root path
		static auto SetRootPath() -> std::filesystem::path {
			m_rootPath = std::filesystem::current_path();
			return m_rootPath;
		}

		/// Returns the root path of the filesystem
		/// @return std::filesystem::path root path
		static auto GetRootPath() -> std::filesystem::path { return m_rootPath; }
	};
} // namespace SE
