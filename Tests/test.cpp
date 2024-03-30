#include <snitch/snitch.hpp>

auto main(const int argc, char *argv[]) -> int {
	const std::optional<snitch::cli::input> args = snitch::cli::parse_arguments(argc, argv);
	if (!args) {
		return 1;
	}

	snitch::tests.configure(*args);
	return snitch::tests.run_tests(*args) ? 0 : 1;
}