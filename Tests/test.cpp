#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

import SE;

import <filesystem>;

using namespace SE;
using namespace Utilities;

TEST_CASE("logger tests") {
	REQUIRE(Logger::isInitialized() == false);
	Logger::Init("loggertest.txt");
	REQUIRE(Logger::isInitialized() == true);
	REQUIRE(std::filesystem::exists("loggertest.txt") == true);
}
