#include <filesystem>

#include <doctest/doctest.h>

import SE;

TEST_CASE("Logger" * doctest::description("Tests Logger for common usage")) {
	REQUIRE_FALSE(SE::Logger::isInitialized());
	SE::Logger::Init("loggertest.txt");
	REQUIRE(SE::Logger::isInitialized());
	REQUIRE(std::filesystem::exists("loggertest.txt"));
}
