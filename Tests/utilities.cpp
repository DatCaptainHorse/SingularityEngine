#include <snitch/snitch.hpp>

#include <filesystem>

import SE.Utilities;

constexpr auto RUN_N_TIMES = 10;

TEST_CASE("Filesystem root path", "[Utilities]") {
	for (auto i = 0; i < RUN_N_TIMES; ++i) {
		REQUIRE(SE::Filesystem::GetRootPath() == std::filesystem::current_path());
		REQUIRE(SE::Filesystem::SetRootPath(0, nullptr).error() == SE::FilesystemError::eParamError);
		REQUIRE(SE::Filesystem::SetRootPath(1, nullptr).error() == SE::FilesystemError::eParamError);
		std::array testPath = {"test/binary.bin"};
		REQUIRE(SE::Filesystem::SetRootPath(testPath.size(), testPath.data()).value().string() ==
				"test");
		REQUIRE(SE::Filesystem::GetRootPath().string() == "test");
		REQUIRE(SE::Filesystem::SetRootPath() == std::filesystem::current_path());
	}
}

TEST_CASE("Logger initialize", "[Utilities]") {
	// Make sure Filesystem root path is set to the current path
	SE::Filesystem::SetRootPath();
	for (auto i = 0; i < RUN_N_TIMES; ++i) {
		// Delete logtest if it exists from previous tests
		if (std::filesystem::exists("logtest.txt")) std::filesystem::remove("logtest.txt");

		REQUIRE_FALSE(SE::Logger::IsInitialized());
		SE::Logger::Init();
		REQUIRE(SE::Logger::IsInitialized());
		REQUIRE_FALSE(std::filesystem::exists("logtest.txt"));
		SE::Logger::Clean();
		REQUIRE_FALSE(SE::Logger::IsInitialized());
		SE::Logger::Init("logtest.txt");
		REQUIRE(SE::Logger::IsInitialized());
		REQUIRE(std::filesystem::exists("logtest.txt"));
		SE::Logger::Clean();
		REQUIRE_FALSE(SE::Logger::IsInitialized());
	}
}
