module;

#include <span>
#include <deque>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstdint>
#include <semaphore>
#include <functional>

export module SE.System:Thread;

namespace SE {

	/// Thread class
	/// Wraps std::thread for added functionality
	export class Thread {
	public:
		/// Thread job result
		/// Allows a job to tell the thread how it should proceed
		enum class Result : std::uint8_t {
			eNone,	   ///< Empty value, should not be used as a return value
			eFinished, ///< Job has finished successfully and can be removed
			eContinue, ///< Job has ran successfully and should be repeated
			eFailed,   ///< Job has finished with failure and should be removed
			eFatal	   ///< Job has encountered a fatal error and thread should stop
		};

		/// Thread job function
		using Job = std::function<Result()>;

	private:
		/// Thread state enums
		enum class ThreadState : std::uint8_t {
			eWaiting, ///< Thread is waiting for jobs
			eRunning, ///< Thread is running jobs
			eStopped  ///< Thread has stopped and can't be restarted, should be joined
		};

		std::thread m_thread;
		std::atomic_bool m_fullStopper, m_jobStopper;
		ThreadState m_threadState = ThreadState::eWaiting;
		std::binary_semaphore m_workSemaphore, m_beginSemaphore, m_endSemaphore;
		std::deque<Job> m_jobs;
		Result m_lastResult = Result::eNone;

		/// Job execution thread
		void runner() {
			while (!m_fullStopper) {
				// If no jobs are available, wait for signal
				if (m_threadState == ThreadState::eWaiting) m_workSemaphore.acquire();

				// If full stopper is set, break out of loop early
				if (m_fullStopper) break;

				// If no jobs are available, continue wait
				if (m_jobs.empty()) continue;

				// If m_jobStopper is set, empty out jobs and continue
				if (m_jobStopper) {
					m_jobs.clear();
					m_jobStopper = false;
					m_endSemaphore.release();
					m_threadState = ThreadState::eWaiting;
					continue;
				}

				// Get job from front of queue
				const auto &job = std::move(m_jobs.front());
				m_jobs.pop_front();

				// Set state to running before executing job
				m_threadState = ThreadState::eRunning;

				// Let starter thread know that job has begun
				m_beginSemaphore.release();

				// Execute job
				m_lastResult = job(); // Call job function without passing any parameters

				// Process result from job function
				switch (m_lastResult) {
				case Result::eFinished:
					break;
				case Result::eContinue:
					m_jobs.emplace_back(job); // Queue job again if needed
					break;
				case Result::eFailed:
					break;
				case Result::eFatal:
					m_fullStopper = true; // Stop thread if job encounters fatal error
					break;
				default:
					break;
				}

				// Notify waiting threads that jobs have finished
				if (m_jobs.empty()) {
					m_threadState = ThreadState::eWaiting;
					m_endSemaphore.release();
				}
			}
			m_threadState = ThreadState::eStopped;
			m_beginSemaphore.release();
			m_endSemaphore.release();
		}

	public:
		/// Creates a thread without jobs
		explicit Thread()
			: m_fullStopper(false), m_jobStopper(false), m_workSemaphore(0), m_beginSemaphore(0),
			  m_endSemaphore(0) {
			m_thread = std::thread([this] { runner(); });
		}

		/// Destructor, stops current job before joining thread
		~Thread() {
			m_fullStopper = true;
			m_workSemaphore.release();
			if (m_thread.joinable()) m_thread.join();
		}

		auto operator=(const Thread &other) -> Thread & = delete;
		auto operator=(Thread &&other) -> Thread & = delete;

		Thread(const Thread &other) = delete;
		Thread(Thread &&other) = delete;

		/// Adds multiple jobs to be executed by the thread
		/// @param jobs span of jobs to be added to the queue
		auto queue(const std::span<Job> jobs) -> void {
			for (const auto &job : jobs) m_jobs.emplace_back(job);
		}

		/// Adds a job to be executed by the thread
		/// @param job job to be added to the queue
		auto queue(const Job &job) -> void { m_jobs.emplace_back(job); }

		/// Signal thread to start executing jobs
		/// Blocks until thread has started working
		auto start() -> void {
			if (!m_jobs.empty()) {
				m_workSemaphore.release();	// Signal thread to start working
				m_beginSemaphore.acquire(); // Wait for thread to start working
			}
		}

		/// Requests thread to stop executing existing jobs
		auto stop() -> void { m_jobStopper = true; }

		/// Blocks until thread has finished executing jobs and is waiting
		/// @param timeout milliseconds to wait for before timing out
		/// @returns Result of the waited job
		auto wait(const std::chrono::milliseconds timeout = std::chrono::milliseconds(0))
			-> Result {
			// Early return if thread is not running
			if (m_threadState != ThreadState::eRunning) return m_lastResult;
			if (timeout.count() == 0)
				m_endSemaphore.acquire(); // Wait for thread to finish working
			else {
				if (!m_endSemaphore.try_acquire_for(timeout)) return Result::eNone;
			}
			return m_lastResult; // Return lastResult after waiting completes
		}

		/// Clears all jobs of the thread, first stopping and blocking until all jobs are cleared
		auto clear() -> void {
			stop();
			m_jobs.clear();
		}

		/// Returns amount of jobs left in queue
		/// @returns count of jobs in queue
		[[nodiscard]] auto count() const -> std::size_t { return m_jobs.size(); }

		/// Returns if thread is running jobs
		/// @returns true when running jobs, false otherwise
		[[nodiscard]] auto busy() const -> bool { return m_threadState == ThreadState::eRunning; }

		/// Returns thread's ID
		/// @returns ID of the thread
		[[nodiscard]] auto id() const -> std::thread::id { return m_thread.get_id(); }

		/// Returns native handle
		/// @returns native handle to the thread
		auto native() -> std::thread::native_handle_type { return m_thread.native_handle(); }
	};
} // namespace SE
