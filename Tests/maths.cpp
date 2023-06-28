#include <catch2/catch_test_macros.hpp>

import SE.Maths;

TEST_CASE("Vector initialize", "[Maths]") {
	SE::Vector<int, 3> v;
	REQUIRE(v[0] == 0);
	REQUIRE(v[1] == 0);
	REQUIRE(v[2] == 0);
}

TEST_CASE("Vector initialize from elements", "[Maths]") {
	SE::Vector<int, 3> v(1, 2, 3);
	REQUIRE(v[0] == 1);
	REQUIRE(v[1] == 2);
	REQUIRE(v[2] == 3);
}

TEST_CASE("Vector copy", "[Maths]") {
	SE::Vector<int, 3> v1;
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	SE::Vector<int, 3> v2 = v1;
	REQUIRE(v2[0] == 1);
	REQUIRE(v2[1] == 2);
	REQUIRE(v2[2] == 3);
}

TEST_CASE("Vector move", "[Maths]") {
	SE::Vector<int, 3> v1;
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	SE::Vector<int, 3> v2 = std::move(v1);
	REQUIRE(v2[0] == 1);
	REQUIRE(v2[1] == 2);
	REQUIRE(v2[2] == 3);
}

TEST_CASE("Vector copy assign", "[Maths]") {
	SE::Vector<int, 3> v1;
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	SE::Vector<int, 3> v2;
	v2 = v1;
	REQUIRE(v2[0] == 1);
	REQUIRE(v2[1] == 2);
	REQUIRE(v2[2] == 3);
}

TEST_CASE("Vector move assign", "[Maths]") {
	SE::Vector<int, 3> v1;
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	SE::Vector<int, 3> v2;
	v2 = std::move(v1);
	REQUIRE(v2[0] == 1);
	REQUIRE(v2[1] == 2);
	REQUIRE(v2[2] == 3);
}

TEST_CASE("Vector access", "[Maths]") {
	SE::Vector<int, 3> v;
	v[0] = 1;
	v[1] = 2;
	v[2] = 3;
	REQUIRE(v[0] == 1);
	REQUIRE(v[1] == 2);
	REQUIRE(v[2] == 3);
}

TEST_CASE("Vector const access", "[Maths]") {
	const SE::Vector<int, 3> v;
	REQUIRE(v[0] == 0);
	REQUIRE(v[1] == 0);
	REQUIRE(v[2] == 0);
}

TEST_CASE("Vector add", "[Maths]") {
	SE::Vector<int, 3> v1;
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	SE::Vector<int, 3> v2;
	v2[0] = 4;
	v2[1] = 5;
	v2[2] = 6;
	SE::Vector<int, 3> v3 = v1 + v2;
	REQUIRE(v3[0] == 5);
	REQUIRE(v3[1] == 7);
	REQUIRE(v3[2] == 9);
}

TEST_CASE("Vector subtract", "[Maths]") {
	SE::Vector<int, 3> v1;
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	SE::Vector<int, 3> v2;
	v2[0] = 4;
	v2[1] = 5;
	v2[2] = 6;
	SE::Vector<int, 3> v3 = v1 - v2;
	REQUIRE(v3[0] == -3);
	REQUIRE(v3[1] == -3);
	REQUIRE(v3[2] == -3);
}

TEST_CASE("Vector multiply", "[Maths]") {
	SE::Vector<int, 3> v1;
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	SE::Vector<int, 3> v2;
	v2[0] = 4;
	v2[1] = 5;
	v2[2] = 6;
	SE::Vector<int, 3> v3 = v1 * v2;
	REQUIRE(v3[0] == 4);
	REQUIRE(v3[1] == 10);
	REQUIRE(v3[2] == 18);
}

TEST_CASE("Vector divide", "[Maths]") {
	SE::Vector<int, 3> v1;
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;
	SE::Vector<int, 3> v2;
	v2[0] = 4;
	v2[1] = 5;
	v2[2] = 6;
	SE::Vector<int, 3> v3 = v1 / v2;
	REQUIRE(v3[0] == 0);
	REQUIRE(v3[1] == 0);
	REQUIRE(v3[2] == 0);
}
