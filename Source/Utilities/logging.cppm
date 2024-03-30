module;

#include <string>
#include <format>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <optional>
#include <filesystem>

export module SE.Utilities:Logging;

import :Filesystem;

namespace SE {

	// Log type enum class
	export enum class LogType : std::uint8_t {
		eNone,		  ///< No log type
		eInfo,		  ///< Informational log
		ePerformance, ///< Performance log
		eWarning,	  ///< Warning log
		eError,		  ///< Error log
		eFatal		  ///< Fatal error log
	};

	/// RAII Logging class
	/// Uses std::format for formatting of log messages
	export class Logger {
		inline static bool m_initialized = false;
		inline static std::ofstream m_outputLog;

	public:
		/// Initializes the logger
		/// @param logPath path where the log file will be saved,
		/// if not given only console output will be used
		static void Init(const std::optional<std::filesystem::path> &logPath = std::nullopt) {
			if (m_initialized) return;
			if (logPath) {
				const auto fullPath = Filesystem::GetRootPath() / logPath.value();
				m_outputLog = std::ofstream(fullPath, std::ios::out | std::ios::app);
			}
			m_initialized = true;
		}

		/// Cleans up the logger and closes file stream
		static void Clean() {
			if (!m_initialized) return;
			if (m_outputLog.is_open()) m_outputLog.close();
			m_initialized = false;
		}

		/// Returns whether the logger is initialized
		static auto IsInitialized() -> bool { return m_initialized; }

		/// For general logging purposes.
		/// @param message message to log
		/// @param args arguments to log
		template <typename... Args>
		static void Log(const std::string_view message, const Args &...args) {
			if (!m_initialized) return;
			log(LogType::eNone, message, args...);
		}

		/// For something that is particularly informative.
		/// @param message message to log
		/// @param args arguments to log
		template <typename... Args>
		static void Info(const std::string_view message, const Args &...args) {
			if (!m_initialized) return;
			log(LogType::eInfo, message, args...);
		}

		/// For logging something that might affect performance.
		/// @param message message to log
		/// @param args arguments to log
		template <typename... Args>
		static void Performance(const std::string_view message, const Args &...args) {
			if (!m_initialized) return;
			log(LogType::ePerformance, message, args...);
		}

		/// For things that shouldn't happen but execution can continue normally.
		/// @param message message to log
		/// @param args arguments to log
		template <typename... Args>
		static void Warning(const std::string_view message, const Args &...args) {
			if (!m_initialized) return;
			log(LogType::eWarning, message, args...);
		}

		/// For things that really shouldn't have happened and causes further issues.
		/// @param message message to log
		/// @param args arguments to log
		template <typename... Args>
		static void Error(const std::string_view message, const Args &...args) {
			if (!m_initialized) return;
			log(LogType::eError, message, args...);
		}

		/// When something went wrong so badly that the program has to quit immediately.
		/// @param message message to log
		/// @param args arguments to log
		template <typename... Args>
		static void Fatal(const std::string_view message, const Args &...args) {
			if (!m_initialized) return;
			log(LogType::eFatal, message, args...);
		}

		/// Logs newlines to output streams.
		/// @param count amount of newlines to add
		static void EndLine(const std::uint32_t count = 1) {
			if (!m_initialized) return;
			for (std::uint32_t i = 0; i < count; ++i) log(LogType::eNone, "\n");
		}

	private:
		static auto getLogTypePrefix(LogType logType) -> std::string;

		template <typename... Args>
		static void log(LogType logType, std::format_string<Args...> message, const Args &...args);
	};

	auto Logger::getLogTypePrefix(const LogType logType) -> std::string {
		switch (logType) {
		case LogType::eInfo:
			return "[INFO]";
		case LogType::ePerformance:
			return "[PERF]";
		case LogType::eWarning:
			return "[WARN]";
		case LogType::eError:
			return "[ERROR]";
		case LogType::eFatal:
			return "[FATAL]";
		case LogType::eNone:
		default:
			return "";
		}
	}

	template <typename... Args>
	void Logger::log(const LogType logType, const std::format_string<Args...> message,
					 const Args &...args) {
		if (!m_initialized) return;

		// Get the log type prefix
		const auto logPrefix = getLogTypePrefix(logType);
		// Construct the log message
		const auto logMessage = std::format(message, args...);

		// Print log to console
		std::print("{} {}\n", logPrefix, logMessage);
		// Print log to file if file stream is open
		if (m_outputLog.is_open()) std::print(m_outputLog, "{} {}\n", logPrefix, logMessage);
	}
} // namespace SE
