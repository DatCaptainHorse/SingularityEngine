module;

#include <functional>
#include <thread>
#include <vector>
#include <memory>
#include <algorithm>

export module SE.System:ThreadPool;

import :Thread;

namespace SE {

	/// A thread pool holds multiple threads and can queue work for parallel use
	export class ThreadPool {
	private:
		std::vector<std::unique_ptr<Thread>> pool {};

	public:
		/// Constructs ThreadPool with specific amount of threads
		/// @param count specify the amount of threads in the pool.
		/// If 0 is given, threadpool size is calculated.
		/// @param execType execution type for threads in pool.
		ThreadPool(std::uint32_t count = 0,
				   Thread::ExecutionType execType = Thread::ExecutionType::eSequentialFinish) {
			if (count == 0) {
				const auto sysThreads = std::thread::hardware_concurrency();
				if (sysThreads >= 8)
					count = sysThreads / 4;
				else
					count = 2;
			}

			pool.reserve(count);
			for (std::uint32_t i = 0; i < count; i++)
				pool.emplace_back(std::make_unique<Thread>(execType));
		}

		/// Queues new jobs to execute in parallel
		/// @param functions job functions to queue, std::atomic_bool& required for early stopping
		auto queue(std::convertible_to<Thread::ThreadJob> auto... functions) -> void {
			std::vector<Thread::ThreadJob> expanded = {functions...};
			for (const auto &job : expanded) {
				auto min = std::min_element(pool.begin(), pool.end(),
											[](const auto &a, const auto &b) { return a->count() < b->count(); });
				(*min)->queue(job);
			}
		}

		/// Starts executing jobs for each thread, blocks until all threads have started
		auto start() -> void {
			for (auto &thr : pool)
				thr->start();
		}

		/// Request's all threads to stop current job
		auto stop() -> void {
			for (auto &thr : pool)
				thr->stop();
		}

		/// Blocks until all threads are done
		/// @param timeout time in milliseconds to wait for before timing out
		/// @returns false on timeout, true otherwise
		auto wait(std::uint32_t timeout = 0) -> bool {
			bool result = false;
			for (auto &thr : pool) {
				result = thr->wait(timeout);
				if (!result)
					break;
			}
			return result;
		}

		/// Clears all threads, blocks until stopping and clearing all threads jobs is done
		auto clear() -> void {
			stop();
			for (auto &thr : pool)
				thr->clear();
		}

		/// Get amount of threads in the pool
		/// @returns amount of threads in the pool
		auto size() -> std::size_t {
			return pool.size();
		}

		/// Returns if any thread in pool is running jobs
		/// @returns true when there are threads running jobs, false otherwise
		auto busy() -> bool {
			return std::ranges::any_of(pool, [](const auto &thr) {
				return thr->busy();
			});
		}
	};
} // namespace SE
