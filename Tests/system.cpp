#include <chrono>
#include <thread>

#include <catch2/catch_test_macros.hpp>

import SE.System;

TEST_CASE("Test Thread Execution Single Job") {
	SE::Thread t(SE::Thread::ExecutionType::eSequentialFinish);
	std::atomic_bool executed(false);

	t.queue([&executed](std::atomic_bool &stop) {
		executed = true;
	});

	t.start();
	t.wait();

	REQUIRE(executed);
}

TEST_CASE("Test Thread Execution Multiple Jobs") {
	SE::Thread t(SE::Thread::ExecutionType::eSequentialFinish);
	std::atomic_int counter(0);

	t.queue([&counter](std::atomic_bool &stop) { counter += 1; });
	t.queue([&counter](std::atomic_bool &stop) { counter += 2; });
	t.queue([&counter](std::atomic_bool &stop) { counter += 3; });

	t.start();
	t.wait();

	REQUIRE(counter == 6);
}

TEST_CASE("Test Thread Execution Order") {
	SE::Thread t(SE::Thread::ExecutionType::eSequentialFinish);
	std::vector<int> results;

	t.queue([&results](std::atomic_bool &stop) { results.push_back(1); });
	t.queue([&results](std::atomic_bool &stop) { results.push_back(2); });
	t.queue([&results](std::atomic_bool &stop) { results.push_back(3); });

	t.start();
	t.wait();

	REQUIRE(results == std::vector<int>({1, 2, 3}));
}

TEST_CASE("Test Thread Clear Jobs") {
	SE::Thread t(SE::Thread::ExecutionType::eSequentialFinish);
	std::atomic_int counter(0);

	t.queue([&counter](std::atomic_bool &stop) { counter += 1; });
	t.queue([&counter](std::atomic_bool &stop) { counter += 2; });
	t.queue([&counter](std::atomic_bool &stop) { counter += 3; });

	t.clear();
	t.start();
	t.wait();

	REQUIRE(counter == 0);
	REQUIRE(t.count() == 0);
}

TEST_CASE("Test Sequential Repeat Execution") {
	SE::Thread t(SE::Thread::ExecutionType::eSequentialRepeat);
	std::atomic_int counter(0);

	t.queue([&counter](std::atomic_bool &stop) {
		counter += 1;
		if (counter == 5) {
			stop = true;
		}
	});

	t.start();
	t.wait();

	REQUIRE(counter == 5);
}

TEST_CASE("Test Stop Functionality") {
	SE::Thread t(SE::Thread::ExecutionType::eSequentialFinish);
	std::atomic_int counter(0);

	t.queue([&counter](std::atomic_bool &stop) {
		for (int i = 0; i < 5; ++i) {
			if (stop) {
				break;
			}
			counter += 1;
		}
	});

	t.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	t.stop();
	t.wait();

	REQUIRE(counter > 0);
}

TEST_CASE("Test Wait Timeout") {
	SE::Thread t(SE::Thread::ExecutionType::eSequentialFinish);
	std::atomic_int counter(0);

	t.queue([&counter](std::atomic_bool &stop) {
		for (int i = 0; i < 5; ++i) {
			if (stop) {
				break;
			}
			counter += 1;
		}
	});

	t.start();
	bool result = t.wait(100); // 10ms timeout

	REQUIRE(result);
	REQUIRE(counter > 0);
	t.stop(); // Ensure the thread is stopped before exiting the test
}

TEST_CASE("Test Thread Busy Status") {
	SE::Thread t(SE::Thread::ExecutionType::eSequentialFinish);
	std::atomic_bool executed(false);

	t.queue([&executed](std::atomic_bool &stop) {
		executed = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	});

	REQUIRE_FALSE(t.busy());
	t.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(t.busy());
	t.wait();
	REQUIRE_FALSE(t.busy());
}

TEST_CASE("Test ThreadPool Parallel Execution") {
	SE::ThreadPool tp;
	std::atomic_int counter(0);

	tp.queue([&counter](std::atomic_bool &stop) { counter += 1; },
			 [&counter](std::atomic_bool &stop) { counter += 2; },
			 [&counter](std::atomic_bool &stop) { counter += 3; });

	tp.start();
	tp.wait();

	REQUIRE(counter == 6);
}

TEST_CASE("Test ThreadPool Clear Jobs") {
	SE::ThreadPool tp;
	std::atomic_int counter(0);

	tp.queue([&counter](std::atomic_bool &stop) { counter += 1; },
			 [&counter](std::atomic_bool &stop) { counter += 2; },
			 [&counter](std::atomic_bool &stop) { counter += 3; });

	tp.clear();
	tp.start();
	tp.wait();

	REQUIRE(counter == 0);
}

TEST_CASE("Test ThreadPool Stop Functionality") {
	SE::ThreadPool tp;
	std::atomic_int counter(0);

	tp.queue([&counter](std::atomic_bool &stop) {
		for (int i = 0; i < 5; ++i) {
			if (stop) {
				break;
			}
			counter += 1;
		}
	});

	tp.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	tp.stop();
	tp.wait();

	REQUIRE(counter > 0);
}

TEST_CASE("Test ThreadPool Wait Timeout") {
	SE::ThreadPool tp;
	std::atomic_int counter(0);

	tp.queue([&counter](std::atomic_bool &stop) {
		for (int i = 0; i < 5; ++i) {
			if (stop) {
				break;
			}
			counter += 1;
		}
	});

	tp.start();
	bool result = tp.wait(100); // 100ms timeout

	REQUIRE(result);
	REQUIRE(counter > 0);
	tp.stop(); // Ensure the thread pool is stopped before exiting the test
}

TEST_CASE("Test ThreadPool Busy Status") {
	SE::ThreadPool tp;
	std::atomic_bool executed(false);

	tp.queue([&executed](std::atomic_bool &stop) {
		executed = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	});

	REQUIRE_FALSE(tp.busy());
	tp.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	REQUIRE(tp.busy());
	tp.wait();
	REQUIRE_FALSE(tp.busy());
}
