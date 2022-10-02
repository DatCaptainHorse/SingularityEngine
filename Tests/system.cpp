#include <chrono>
#include <thread>

#include <doctest/doctest.h>

import SE;

TEST_CASE("Thread - sequential finish" *
		  doctest::description("Tests Thread for common usage with eSequentialFinish"
							   ", shouldn't take longer than 5 seconds") *
		  doctest::timeout(5.0)) {
	auto thr = SE::Thread();
	REQUIRE(thr.count() == 0);
	REQUIRE_FALSE(thr.busy());

	SUBCASE("Wait for jobless thread") {
		const auto waitResult = thr.wait();
		REQUIRE(waitResult);
	}

	SUBCASE("Create an ending job") {
		int counter = 0;
		auto threadjob = [&counter](std::atomic_bool &stop) {
			CHECK_FALSE(stop);
			counter = 10;
		};

		SUBCASE("Test job") {
			std::atomic_bool atomicBool = false;
			threadjob(atomicBool);
			CHECK(counter == 10);
		}

		SUBCASE("Queue job") {
			thr.queue(threadjob);

			CHECK(counter == 0);
			CHECK(thr.count() == 1);
			CHECK_FALSE(thr.busy());

			SUBCASE("Start and wait for job") {
				thr.start();
				const auto waitResult = thr.wait();
				CHECK(counter == 10);
				CHECK(thr.count() == 0);
				CHECK_FALSE(thr.busy());
				CHECK(waitResult);
			}

			SUBCASE("Start and wait for job - sleep") {
				thr.start();
				std::this_thread::sleep_for(std::chrono::microseconds(1));
				const auto waitResult = thr.wait();
				CHECK(counter == 10);
				CHECK(thr.count() == 0);
				CHECK_FALSE(thr.busy());
				CHECK(waitResult);
			}

			SUBCASE("Start and clear job") {
				thr.start();
				thr.clear();
				CAPTURE(counter);
				CHECK(thr.count() == 0);
				CHECK_FALSE(thr.busy());
			}
		}
	}

	SUBCASE("Create an unending job") {
		int counter = 0;
		auto threadjob = [&counter](std::atomic_bool &stop) {
			while (!stop)
				counter++;
		};

		SUBCASE("Test job") {
			std::atomic_bool atomicBool = true;
			threadjob(atomicBool);
			CHECK(counter == 0);
		}

		SUBCASE("Queue job") {
			thr.queue(threadjob);

			CHECK(counter == 0);
			CHECK(thr.count() == 1);
			CHECK_FALSE(thr.busy());

			SUBCASE("Start, stop and wait for job") {
				thr.start();
				thr.stop();
				const auto waitResult = thr.wait();
				CAPTURE(counter);
				CAPTURE(thr.count());
				CAPTURE(thr.busy());
				CHECK(waitResult);
			}

			SUBCASE("Start and wait for job - timeout") {
				thr.start();
				auto waitResult = thr.wait(1);
				CAPTURE(counter);
				CAPTURE(thr.count());
				CAPTURE(thr.busy());
				CHECK_FALSE(waitResult);

				thr.stop();
				waitResult = thr.wait();
				CAPTURE(counter);
				CHECK(thr.count() == 0);
				CHECK_FALSE(thr.busy());
				CHECK(waitResult);
			}

			SUBCASE("Start, stop and wait for job - sleep") {
				thr.start();
				thr.stop();
				std::this_thread::sleep_for(std::chrono::microseconds(1));
				const auto waitResult = thr.wait();
				CAPTURE(counter);
				CAPTURE(thr.count());
				CAPTURE(thr.busy());
				CHECK(waitResult);
			}

			SUBCASE("Start and wait for job - sleep and timeout") {
				thr.start();
				std::this_thread::sleep_for(std::chrono::microseconds(1));
				auto waitResult = thr.wait(1);
				CAPTURE(counter);
				CAPTURE(thr.count());
				CAPTURE(thr.busy());
				CHECK_FALSE(waitResult);

				thr.stop();
				waitResult = thr.wait();
				CAPTURE(counter);
				CHECK(thr.count() == 0);
				CHECK_FALSE(thr.busy());
				CHECK(waitResult);
			}
		}
	}

	SUBCASE("Create multiple ending jobs") {
		int counter = 0;
		auto threadjob1 = [&counter](std::atomic_bool &) {
			counter++;
		};
		auto threadjob2 = [&counter](std::atomic_bool &) {
			counter++;
		};
		auto threadjob3 = [&counter](std::atomic_bool &) {
			counter--;
		};
		auto threadjob4 = [&counter](std::atomic_bool &) {
			counter--;
		};

		SUBCASE("Test jobs") {
			std::atomic_bool atomicBool = false;
			threadjob1(atomicBool);
			threadjob2(atomicBool);
			threadjob3(atomicBool);
			threadjob4(atomicBool);
			REQUIRE(counter == 0);
		}

		SUBCASE("Queue multiple jobs") {
			thr.queue(threadjob1, threadjob2, threadjob3, threadjob4);

			CHECK(counter == 0);
			CHECK(thr.count() == 4);
			CHECK_FALSE(thr.busy());

			SUBCASE("Start and wait for jobs") {
				thr.start();
				const auto waitResult = thr.wait();
				CHECK(counter == 0);
				CHECK(thr.count() == 0);
				CHECK_FALSE(thr.busy());
				CHECK(waitResult);
			}
		}
	}
}

TEST_CASE("Thread - sequential repeat" *
		  doctest::description("Tests Thread for common usage with eSequentialRepeat"
							   ", shouldn't take longer than 5 seconds") *
		  doctest::timeout(5.0)) {
	auto thr = SE::Thread(SE::Thread::ExecutionType::eSequentialRepeat);
	REQUIRE(thr.count() == 0);
	REQUIRE_FALSE(thr.busy());

	SUBCASE("Wait for jobless thread") {
		const auto waitResult = thr.wait();
		REQUIRE(waitResult);
	}

	SUBCASE("Create a job") {
		int counter = 0;
		auto threadjob = [&counter](std::atomic_bool &) {
			counter++;
		};

		SUBCASE("Test job") {
			std::atomic_bool atomicBool = false;
			threadjob(atomicBool);
			CHECK(counter == 1);
		}

		SUBCASE("Queue job") {
			thr.queue(threadjob);

			CHECK(counter == 0);
			CHECK(thr.count() == 1);
			CHECK_FALSE(thr.busy());

			SUBCASE("Start and wait for job, stop afterwards") {
				thr.start();
				auto waitResult = thr.wait();
				CHECK(counter != 0);
				CHECK(thr.count() == 1);
				CHECK(thr.busy());
				CHECK(waitResult);

				thr.stop();
				waitResult = thr.wait();
				CHECK(thr.count() == 0);
				CHECK_FALSE(thr.busy());
				CHECK(waitResult);
			}

			SUBCASE("Start and wait for job, stop afterwards - sleep") {
				thr.start();
				std::this_thread::sleep_for(std::chrono::microseconds(1));
				auto waitResult = thr.wait();
				CHECK(counter != 0);
				CHECK(thr.count() == 1);
				CHECK(thr.busy());
				CHECK(waitResult);

				thr.stop();
				waitResult = thr.wait();
				CHECK(thr.count() == 0);
				CHECK_FALSE(thr.busy());
				CHECK(waitResult);
			}

			SUBCASE("Start and clear job") {
				thr.start();
				thr.clear();
				CAPTURE(counter);
				CHECK(thr.count() == 0);
				CHECK_FALSE(thr.busy());
			}
		}
	}
}
