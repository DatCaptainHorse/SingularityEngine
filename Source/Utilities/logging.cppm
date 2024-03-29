﻿module;

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

export module SE.Utilities:Logging;

namespace SE {

	/// RAII Logging class
	/// Uses std::format for formatting of log messages
	export class Logger {
	private:
		enum class LogType {
			eNone,
			eInfo,
			ePerformance,
			eWarning,
			eError,
			eFatal,
		};

		inline static bool m_Init = false;
		inline static std::ofstream m_OutStream = {};

	public:
		/// Initializes the logger
		/// @param outputFile file to output log to
		static void Init(const std::string &outputFile = "log.txt") {
			m_OutStream = std::ofstream(outputFile);
			m_Init = true;
		}

		/// Cleans up the logger and closes file stream
		static void Clean() {
			if (!m_Init)
				return;

			m_OutStream.close();
			m_Init = false;
		}

		/// Returns whether the logger is initialized
		static auto isInitialized() -> bool {
			return m_Init;
		}

		/// For general logging purposes.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Log(const std::string_view logMessage, const Args &...args) {
			i_log(logMessage, LogType::eNone, std::make_format_args(args...));
		}

		/// For something that is particularly informative.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Info(const std::string_view logMessage, const Args &...args) {
			i_log(logMessage, LogType::eInfo, std::make_format_args(args...));
		}

		/// For logging something that might affect performance.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Performance(const std::string_view logMessage, const Args &...args) {
			i_log(logMessage, LogType::ePerformance, std::make_format_args(args...));
		}

		/// For things that shouldn't happen but execution can continue normally.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Warning(const std::string_view logMessage, const Args &...args) {
			i_log(logMessage, LogType::eWarning, std::make_format_args(args...));
		}

		/// For things that really shouldn't have happened and causes further issues.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Error(const std::string_view logMessage, const Args &...args) {
			i_log(logMessage, LogType::eError, std::make_format_args(args...));
		}

		/// When something went wrong so badly that the program has to quit immediately.
		/// @param logMessage message to log
		/// @param args arguments to log
		template<typename... Args>
		static void Fatal(const std::string_view logMessage, const Args &...args) {
			i_log(logMessage, LogType::eFatal, std::make_format_args(args...));
		}

		/// Logs a newline
		/// @param count amount of newlines to add
		static void EndLine(const uint32_t count = 1) {
			if (!m_Init)
				return;

			for (uint32_t i = 0; i < count; ++i) {
				std::cout << std::endl;
				m_OutStream << std::endl;
			}
		}

	private:
		static auto i_getLogTypeString(LogType logType) -> std::string {
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

		static void i_log(std::string_view message, LogType logType,
						  std::basic_format_args<std::format_context> fmtargs) {
			if (!m_Init)
				return;

			// Skip newline if line carriage exists
			bool skipNewline = false;
			if (!message.empty() && message.find('\r') != std::string::npos)
				skipNewline = true;

			// Replace tabs with |||| instead
			bool tabFill = false;
			if (!message.empty() && message.front() == '\t')
				tabFill = true;

			auto fmtd = std::vformat(message, fmtargs);
			if (tabFill)
				fmtd.erase(0, 1);

			const auto logTypeString = i_getLogTypeString(logType);

			switch (logType) {
				case LogType::eInfo:
					std::cout << "\033[34m" << logTypeString << "\033[0m";
					break;
				case LogType::ePerformance:
				case LogType::eWarning:
					std::cout << "\033[33m" << logTypeString << "\033[0m";
					break;
				case LogType::eError:
					std::cout << "\033[31m" << logTypeString << "\033[0m";
					break;
				case LogType::eFatal:
					std::cout << "\033[30;41m" << logTypeString << "\033[0m";
					break;
				case LogType::eNone:
				default:
					break;
			}

			if (tabFill) {
				if (logType == LogType::eNone) {
					m_OutStream << logTypeString << "|||| " << fmtd;
					std::cout << "|||| " << fmtd;
				} else {
					m_OutStream << logTypeString << " |||| " << fmtd;
					std::cout << " |||| " << fmtd;
				}
			} else {
				if (logType == LogType::eNone) {
					m_OutStream << logTypeString << fmtd;
					std::cout << fmtd;
				} else {
					m_OutStream << logTypeString << " " << fmtd;
					std::cout << " " << fmtd;
				}
			}

			if (!skipNewline) {
				m_OutStream << std::endl;
				std::cout << std::endl;
			}
		}
	};
} // namespace SE
