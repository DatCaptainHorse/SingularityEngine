#include <benchmark/benchmark.h>

import SE.Kernel;

auto main(int argc, char** argv) -> int {
	SE::EngineKernel::Config kernelConfig;
	kernelConfig.loadSIMD = true; //< Change this to see the difference
	SE::EngineKernel::Init(kernelConfig);

	benchmark::Initialize(&argc, argv);
	benchmark::RunSpecifiedBenchmarks();

	SE::EngineKernel::Clean();

	return 0;
}