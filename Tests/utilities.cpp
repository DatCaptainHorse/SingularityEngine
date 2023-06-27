#include <filesystem>

#include <catch2/catch_test_macros.hpp>

import SE.Utilities;

TEST_CASE("Logger initialize", "[Utilities]") {
	REQUIRE_FALSE(SE::Logger::isInitialized());
	SE::Logger::Init("loggertest.txt");
	REQUIRE(SE::Logger::isInitialized());
	REQUIRE(std::filesystem::exists("loggertest.txt"));
}
