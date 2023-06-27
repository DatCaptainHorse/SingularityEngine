module;

#include <atomic>
#include <chrono>
#include <concepts>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <semaphore>
#include <stop_token>
#include <thread>

export module SE.System:Thread;

namespace SE {

	/// Thread class
	/// Wraps std::jthread for added functionality
	export class Thread {
	public:
		/// Thread job execution type
		enum class ExecutionType {
			eSequentialFinish, ///< Jobs are executed and removed sequentially
			eSequentialRepeat  ///< Jobs are executed sequentially and repeated until stopped
		};

		/// Thread job function base
		using ThreadJob = std::function<void(std::atomic_bool &)>;

	private:
		/// Thread state enums
		enum class ThreadState {
			eWaiting, ///< Thread is waiting for jobs
			eRunning  ///< Thread is running jobs
		};

		std::jthread thread;
		std::mutex runnerMutex;
		std::atomic_bool stopper;
		ExecutionType executionType;
		std::atomic<ThreadState> threadState;
		std::binary_semaphore startSemaphore, endSemaphore, workSemaphore;
		std::queue<ThreadJob> jobs;

		/// Job execution thread
		void runner(const std::stop_token &token) {
			while (!token.stop_requested()) {
				ThreadJob job;

				// Wait for more work or stop request
				if (threadState == ThreadState::eWaiting && !token.stop_requested())
					workSemaphore.acquire();

				// Early break on stop request
				if (token.stop_requested()) {
					threadState = ThreadState::eWaiting;
					endSemaphore.release();
					break;
				}

				{
					const std::scoped_lock lock(runnerMutex);

					// Assign job from front of queue
					job = std::move(jobs.front());

					// We are running
					threadState = ThreadState::eRunning;
				}

				// Signal starter
				startSemaphore.release();

				// Execute job
				job(stopper);

				{
					const std::scoped_lock lock(runnerMutex);

					// Update job queue
					if (executionType == ExecutionType::eSequentialFinish) {
						if (!jobs.empty())
							jobs.pop();

						stopper = false;
					} else if (executionType == ExecutionType::eSequentialRepeat) {
						if (!jobs.empty())
							jobs.pop();

						if (!stopper)
							jobs.push(job);
						else if (jobs.empty())
							stopper = false;
					}

					// If there are no jobs left, we are waiting
					if (jobs.empty())
						threadState = ThreadState::eWaiting;

					// Signal waiter
					endSemaphore.release();
				}
			}
		}

	public:
		/// Creates a thread without jobs
		/// @param execType ExecutionType of thread
		explicit Thread(ExecutionType execType = ExecutionType::eSequentialFinish)
			: executionType(execType), threadState(ThreadState::eWaiting), stopper(false),
			  startSemaphore(0), endSemaphore(0), workSemaphore(0) {
			thread = std::jthread([this](const std::stop_token &stopToken) {
				runner(stopToken);
			});
		}

		/// Destructor, stops current job before deconstruction
		~Thread() {
			if (threadState == ThreadState::eWaiting)
				workSemaphore.release();

			thread.request_stop();
		}

		auto operator=(const Thread &other) -> Thread & = delete;
		auto operator=(Thread &&other) -> Thread & = delete;

		Thread(const Thread &other) = delete;
		Thread(Thread &&other) = delete;

		/// Queues new jobs to execute
		/// @param functions job functions to queue, std::atomic_bool& required for early stopping
		auto queue(std::convertible_to<ThreadJob> auto... functions) -> void {
			const std::scoped_lock lock(runnerMutex);
			std::vector<ThreadJob> expanded = {functions...};
			for (const auto &job : expanded)
				jobs.push(job);
		}

		/// Starts executing jobs in queue if not already, blocks until thread has started
		auto start() -> void {
			if (!jobs.empty() && threadState == ThreadState::eWaiting) {
				workSemaphore.release();
				startSemaphore.acquire();
			}
		}

		/// Request's to stop execution of current job
		auto stop() -> void {
			if (threadState == ThreadState::eRunning)
				stopper = true;
		}

		/// Blocks until all jobs have finished if thread execution type is eSequentialFinish.
		/// If thread execution type is eSequentialRepeat, waits until single job has finished.
		/// @param timeout time in milliseconds to wait for before timing out
		/// @returns false on timeout, true if thread has finished
		auto wait(std::uint32_t timeout = 0) -> bool {
			// Early return if thread is in waiting state
			if (threadState == ThreadState::eWaiting)
				return true;

			auto should_continue_waiting = [this]() {
				return (executionType == ExecutionType::eSequentialFinish &&
						threadState == ThreadState::eRunning) ||
					   (executionType == ExecutionType::eSequentialRepeat && stopper);
			};

			if (timeout == 0) {
				while (should_continue_waiting()) {
					endSemaphore.acquire();
				}
			} else {
				while (true) {
					if (!should_continue_waiting())
						break;
					if (!endSemaphore.try_acquire_for(std::chrono::milliseconds(timeout))) {
						return false;
					}
				}
			}

			return true;
		}

		/// Clear all jobs from queue, blocks until done incase thread is running
		auto clear() -> void {
			stop();
			wait();
			{
				const std::scoped_lock lock(runnerMutex);
				jobs = {};
			}
		}

		/// Returns amount of jobs left in queue
		/// @returns count of jobs in queue
		auto count() -> std::size_t {
			const std::scoped_lock lock(runnerMutex);
			return jobs.size();
		}

		/// Returns if thread is running jobs
		/// @returns true when running jobs, false otherwise
		auto busy() -> bool {
			return threadState == ThreadState::eRunning;
		}

		/// Returns thread's ID
		/// @returns ID of the thread
		auto id() -> std::jthread::id {
			return thread.get_id();
		}

		/// Returns native handle
		/// @returns native handle to the thread
		auto native() -> std::jthread::native_handle_type {
			return thread.native_handle();
		}
	};
} // namespace SE
