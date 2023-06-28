module;

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

export module SE.Utilities:Logging;

import fmt;

namespace SE {

	/// RAII logging class
	export class Logger {
	public:
		/// Log level enums.
		enum class LogLevel {
			None,		 //< For general logging purposes.
			Info,		 //< For something that is particularly informative.
			Performance, //< For logging something that might affect performance.
			Warning,	 //< For things that shouldn't happen but execution can continue normally.
			Error, //< For things that really shouldn't have happened and causes further issues.
			Fatal, //< When something went wrong so badly that the program has to quit immediately.
			Debug, //< For debugging purposes, only logs to console if log level is set to debug.
		};

		/// The logger configuration.
		struct Config {
			std::string outputFile;				///< The file to output log to.
			LogLevel logLevel {LogLevel::Info}; ///< The log level to use for console.

			/// Constructs the logger configuration with default values.
			Config() : outputFile("log.txt") {}
		};

	private:
		inline static bool initialized = false;
		inline static std::ofstream outputFileStream;
		inline static LogLevel logLevel;

	public:
		/// Initializes the logger
		/// @param outputFile file to output log to
		static void Init(const Config &config = Config()) {
			outputFileStream = std::ofstream(config.outputFile);
			logLevel = config.logLevel;
			initialized = true;
		}

		/// Cleans up the logger and closes file stream
		static void Clean() {
			if (!initialized)
				return;

			outputFileStream.close();
			initialized = false;
		}

		/// Returns whether the logger is initialized
		static auto isInitialized() -> bool {
			return initialized;
		}

		/// For general logging purposes.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Log(const std::string_view logMessage, const Args &...args) {
			iLog(logMessage, LogLevel::None, fmt::make_format_args(args...));
		}

		/// For something that is particularly informative.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Info(const std::string_view logMessage, const Args &...args) {
			iLog(logMessage, LogLevel::Info, fmt::make_format_args(args...));
		}

		/// For logging something that might affect performance.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Performance(const std::string_view logMessage, const Args &...args) {
			iLog(logMessage, LogLevel::Performance, fmt::make_format_args(args...));
		}

		/// For things that shouldn't happen but execution can continue normally.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Warning(const std::string_view logMessage, const Args &...args) {
			iLog(logMessage, LogLevel::Warning, fmt::make_format_args(args...));
		}

		/// For things that really shouldn't have happened and causes further issues.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Error(const std::string_view logMessage, const Args &...args) {
			iLog(logMessage, LogLevel::Error, fmt::make_format_args(args...));
		}

		/// When something went wrong so badly that the program has to quit immediately.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Fatal(const std::string_view logMessage, const Args &...args) {
			iLog(logMessage, LogLevel::Fatal, fmt::make_format_args(args...));
		}

		/// For debugging purposes, only logs to console if log level is set to debug.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Debug(const std::string_view logMessage, const Args &...args) {
			iLog(logMessage, LogLevel::Debug, fmt::make_format_args(args...));
		}

		/// Logs a newline
		/// @param count amount of newlines to add
		static void EndLine(const uint32_t count = 1) {
			if (!initialized)
				return;

			for (uint32_t i = 0; i < count; ++i) {
				std::cout << "\n";
				outputFileStream << "\n";
			}
		}

	private:
		static auto iGetLogLevelString(LogLevel level) -> std::string {
			switch (level) {
				case LogLevel::Info:
					return "[INFO]";
				case LogLevel::Performance:
					return "[PERF]";
				case LogLevel::Warning:
					return "[WARN]";
				case LogLevel::Error:
					return "[ERROR]";
				case LogLevel::Fatal:
					return "[FATAL]";
				case LogLevel::Debug:
					return "[DEBUG]";
				case LogLevel::None:
				default:
					return "";
			}
		}

		static void iLog(std::string_view message, LogLevel level,
						 fmt::basic_format_args<fmt::format_context> fmtargs) {
			// Early return if logger not initialized
			if (!initialized)
				return;

			// Replace tabs with |||| and skip newline if line carriage exists
			const auto tabFill = (!message.empty() && message.front() == '\t');
			const auto skipNewline = (!message.empty() && message.find('\r') != std::string::npos);

			std::string fmtd = fmt::vformat(message, fmtargs);
			if (tabFill)
				fmtd.erase(0, 1); // remove the first char if it was '\t'

			// Get log level string
			const auto logLevelString = iGetLogLevelString(level);

			// Get color code for console
			std::string colorCode;
			switch (level) {
				case LogLevel::Info:
					colorCode = "\033[34m";
					break;
				case LogLevel::Performance:
				case LogLevel::Warning:
					colorCode = "\033[33m";
					break;
				case LogLevel::Error:
					colorCode = "\033[31m";
					break;
				case LogLevel::Fatal:
					colorCode = "\033[30;41m";
					break;
				case LogLevel::Debug:
					colorCode = "\033[35m";
					break;
				default:
					break;
			}

			// Construct log stringstream
			std::stringstream ss;
			ss << (tabFill ? " |||| " : " ") << fmtd;

			// If given level is less than logLevel, don't log to console
			if (level <= logLevel) {
				if (!colorCode.empty())
					std::cout << colorCode << logLevelString << "\033[0m";
				else
					std::cout << logLevelString;

				std::cout << ss.str();
				if (!skipNewline)
					std::cout << "\n";
			}

			// For file, log everything up to Fatal
			if (level <= LogLevel::Fatal) {
				outputFileStream << logLevelString << ss.str();
				if (!skipNewline)
					outputFileStream << "\n";
			}
		}
	};
} // namespace SE
