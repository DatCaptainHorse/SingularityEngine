#include <benchmark/benchmark.h>

import SE.Maths;

static void BM_VectorAddFloat(benchmark::State& state) {
	SE::Vector<float, 3> a(1.0f, 2.0f, 3.0f);
	SE::Vector<float, 3> b(4.0f, 5.0f, 6.0f);
	SE::Vector<float, 3> c;
	for (auto _ : state) {
		c = a + b;
	}
}
BENCHMARK(BM_VectorAddFloat);

static void BM_VectorSubFloat(benchmark::State& state) {
	SE::Vector<float, 3> a(1.0f, 2.0f, 3.0f);
	SE::Vector<float, 3> b(4.0f, 5.0f, 6.0f);
	SE::Vector<float, 3> c;
	for (auto _ : state)
		c = a - b;
}
BENCHMARK(BM_VectorSubFloat);

static void BM_VectorMulFloat(benchmark::State& state) {
	SE::Vector<float, 3> a(1.0f, 2.0f, 3.0f);
	SE::Vector<float, 3> b(4.0f, 5.0f, 6.0f);
	SE::Vector<float, 3> c;
	for (auto _ : state)
		c = a * b;
}
BENCHMARK(BM_VectorMulFloat);

static void BM_VectorDivFloat(benchmark::State& state) {
	SE::Vector<float, 3> a(1.0f, 2.0f, 3.0f);
	SE::Vector<float, 3> b(4.0f, 5.0f, 6.0f);
	SE::Vector<float, 3> c;
	for (auto _ : state)
		c = a / b;
}
BENCHMARK(BM_VectorDivFloat);

static void BM_VectorAddDouble(benchmark::State& state) {
	SE::Vector<double, 3> a(1.0, 2.0, 3.0);
	SE::Vector<double, 3> b(4.0, 5.0, 6.0);
	SE::Vector<double, 3> c;
	for (auto _ : state)
		c = a + b;
}
BENCHMARK(BM_VectorAddDouble);

static void BM_VectorSubDouble(benchmark::State& state) {
	SE::Vector<double, 3> a(1.0, 2.0, 3.0);
	SE::Vector<double, 3> b(4.0, 5.0, 6.0);
	SE::Vector<double, 3> c;
	for (auto _ : state)
		c = a - b;
}
BENCHMARK(BM_VectorSubDouble);

static void BM_VectorMulDouble(benchmark::State& state) {
	SE::Vector<double, 3> a(1.0, 2.0, 3.0);
	SE::Vector<double, 3> b(4.0, 5.0, 6.0);
	SE::Vector<double, 3> c;
	for (auto _ : state)
		c = a * b;
}
BENCHMARK(BM_VectorMulDouble);

static void BM_VectorDivDouble(benchmark::State& state) {
	SE::Vector<double, 3> a(1.0, 2.0, 3.0);
	SE::Vector<double, 3> b(4.0, 5.0, 6.0);
	SE::Vector<double, 3> c;
	for (auto _ : state)
		c = a / b;
}
BENCHMARK(BM_VectorDivDouble);

static void BM_VectorAddInt(benchmark::State& state) {
	SE::Vector<int, 3> a(1, 2, 3);
	SE::Vector<int, 3> b(4, 5, 6);
	SE::Vector<int, 3> c;
	for (auto _ : state)
		c = a + b;
}
BENCHMARK(BM_VectorAddInt);

static void BM_VectorSubInt(benchmark::State& state) {
	SE::Vector<int, 3> a(1, 2, 3);
	SE::Vector<int, 3> b(4, 5, 6);
	SE::Vector<int, 3> c;
	for (auto _ : state)
		c = a - b;
}
BENCHMARK(BM_VectorSubInt);

static void BM_VectorMulInt(benchmark::State& state) {
	SE::Vector<int, 3> a(1, 2, 3);
	SE::Vector<int, 3> b(4, 5, 6);
	SE::Vector<int, 3> c;
	for (auto _ : state)
		c = a * b;
}
BENCHMARK(BM_VectorMulInt);

static void BM_VectorDivInt(benchmark::State& state) {
	SE::Vector<int, 3> a(1, 2, 3);
	SE::Vector<int, 3> b(4, 5, 6);
	SE::Vector<int, 3> c;
	for (auto _ : state)
		c = a / b;
}
BENCHMARK(BM_VectorDivInt);
