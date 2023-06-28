module;

#include <fstream>
#include <string>

export module SE.Utilities:FileUtilities;

namespace SE {

	export class FileUtilities {
	public:
		/// Checks whether a file exists.
		/// @warning This is a temporary function and will be removed once the filesystem library
		/// no longer has issues with modules.
		static auto exists(const std::string &filename) -> bool {
			auto file = std::ifstream(filename);
			const auto fileExists = file.good();

			if (fileExists)
				file.close();

			return fileExists;
		}
	};
} // namespace SE
