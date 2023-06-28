module;

#include <string>
#include <utility>

export module SE.Common:Result;

namespace SE {

	/// The result class.
	export template<typename T>
	class Result {
	public:
		enum class Status { Success, Failure, Warning };

		explicit Result(Status status, T value, std::string message = "")
			: status(status), value(std::move(value)), message(std::move(message)) {}

		[[nodiscard]] auto getStatus() const -> Status {
			return status;
		}
		[[nodiscard]] auto getValue() const -> const T & {
			return value;
		}
		[[nodiscard]] auto getMessage() const -> const std::string & {
			return message;
		}

		[[nodiscard]] auto isSuccess() const -> bool {
			return status == Status::Success;
		}
		[[nodiscard]] auto isFailure() const -> bool {
			return status == Status::Failure;
		}
		[[nodiscard]] auto isWarning() const -> bool {
			return status == Status::Warning;
		}

	protected:
		Status status;
		T value;
		std::string message;
	};
} // namespace SE
