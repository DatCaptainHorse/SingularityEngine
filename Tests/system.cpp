#include <snitch/snitch.hpp>

#include <span>
#include <array>
#include <chrono>
#include <atomic>
#include <ranges>
#include <thread>
#include <algorithm>

import SE.System;

constexpr auto RUN_N_TIMES = 10;

TEST_CASE("Test Thread Execution Single Job") {
	for (auto i = 0; i < RUN_N_TIMES; ++i) {
		SE::Thread t;
		bool executed = false;

		t.queue([&executed]() {
			executed = true;
			return SE::Thread::Result::eFinished;
		});

		t.start();
		t.wait();

		REQUIRE(executed);
	}
}

TEST_CASE("Test Thread Execution Multiple Jobs") {
	for (auto i = 0; i < RUN_N_TIMES; ++i) {
		SE::Thread t;
		std::atomic_int counter(0);
		std::array<SE::Thread::Job, 3> jobs{[&counter]() {
												counter += 1;
												return SE::Thread::Result::eFinished;
											},
											[&counter]() {
												counter += 2;
												return SE::Thread::Result::eFinished;
											},
											[&counter]() {
												counter += 3;
												return SE::Thread::Result::eFinished;
											}};

		t.queue(jobs);
		t.start();
		t.wait();

		REQUIRE(counter.load() == 6);
	}
}

TEST_CASE("Test Thread Execution Order") {
	for (auto i = 0; i < RUN_N_TIMES; ++i) {
		SE::Thread t;
		std::array<int, 3> results{0};

		t.queue([&results]() {
			results[0] = 1;
			return SE::Thread::Result::eFinished;
		});
		t.queue([&results]() {
			results[1] = 2;
			return SE::Thread::Result::eFinished;
		});
		t.queue([&results]() {
			results[2] = 3;
			return SE::Thread::Result::eFinished;
		});

		t.start();
		t.wait();

		REQUIRE(results[0] == 1 && results[1] == 2 && results[2] == 3);
	}
}

TEST_CASE("Test Thread Clear Jobs") {
	for (auto i = 0; i < RUN_N_TIMES; ++i) {
		SE::Thread t;
		int counter = 0;

		t.queue([&counter]() {
			counter += 1;
			return SE::Thread::Result::eFinished;
		});
		t.queue([&counter]() {
			counter += 2;
			return SE::Thread::Result::eFinished;
		});
		t.queue([&counter]() {
			counter += 3;
			return SE::Thread::Result::eFinished;
		});

		INFO("Jobs queued: ", t.count());
		t.clear();
		INFO("Jobs left: ", t.count());
		t.start();
		INFO("Jobs left after start: ", t.count());
		t.wait();

		REQUIRE(counter == 0);
		REQUIRE(t.count() == 0);
	}
}

TEST_CASE("Test Sequential Repeat Execution") {
	for (auto i = 0; i < RUN_N_TIMES; ++i) {
		SE::Thread t;
		std::atomic_int counter(0);

		t.queue([&counter]() {
			counter += 1;
			if (counter == 5) return SE::Thread::Result::eFinished;
			return SE::Thread::Result::eContinue;
		});

		t.start();
		t.wait();

		REQUIRE(counter.load() == 5);
	}
}

TEST_CASE("Test Stop Functionality") {
	for (auto i = 0; i < RUN_N_TIMES; ++i) {
		SE::Thread t;
		std::atomic_int counter(0);

		t.queue([&counter]() {
			counter += 1;
			return SE::Thread::Result::eContinue;
		});

		t.start();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		t.stop();
		t.wait();

		REQUIRE(counter.load() > 0);
	}
}

TEST_CASE("Test Wait Timeout") {
	for (auto i = 0; i < RUN_N_TIMES; ++i) {
		SE::Thread t;
		std::atomic_int counter(0);

		t.queue([&counter]() {
			counter += 1;
			return SE::Thread::Result::eContinue;
		});

		t.start();
		const auto result = t.wait(std::chrono::milliseconds(10));

		REQUIRE(result == SE::Thread::Result::eNone);
		REQUIRE(counter.load() > 0);
		// Thread destructor should handle the cleanup
	}
}

TEST_CASE("Test Thread Busy Status") {
	for (auto i = 0; i < RUN_N_TIMES; ++i) {
		SE::Thread t;
		std::atomic_bool executed(false);

		t.queue([&executed]() {
			executed = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			return SE::Thread::Result::eFinished;
		});

		REQUIRE_FALSE(t.busy());
		t.start();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		REQUIRE(t.busy());
		t.wait();
		REQUIRE_FALSE(t.busy());
		REQUIRE(executed.load());
	}
}
