#pragma once

#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

namespace trading::data {

// Machine-readable reason for an unsuccessful retrieval.
enum class LoadErrorCode {
    InvalidRequest,
    IoError,
    InvalidHeader,
    InvalidRow,
    ParseError,
    NotImplemented,
};

// Human-readable context for a failed read or parse step.
class LoadError {
public:
    LoadError() = default;

    LoadError(LoadErrorCode code,
              std::string message,
              std::filesystem::path path = {},
              std::size_t line = 0)
        : code_(code),
          message_(std::move(message)),
          path_(std::move(path)),
          line_(line) {}

    [[nodiscard]] LoadErrorCode Code() const { return code_; }
    [[nodiscard]] const std::string& Message() const { return message_; }
    [[nodiscard]] const std::filesystem::path& Path() const { return path_; }
    [[nodiscard]] std::size_t Line() const { return line_; }

private:
    LoadErrorCode code_ = LoadErrorCode::InvalidRequest;    // Category used by callers for branching.
    std::string message_;                                   // Diagnostic detail for logs or tests.
    std::filesystem::path path_;                            // Source location related to the failure.
    std::size_t line_ = 0;                                  // One-based source line when applicable.
};

// Value-or-error container used instead of exceptions at loader boundaries.
template <typename T>
class LoadResult {
public:
    static LoadResult Success(T value) {
        return LoadResult(std::move(value));
    }

    static LoadResult Failure(LoadError error) {
        return LoadResult(std::move(error));
    }

    [[nodiscard]] bool Ok() const {
        return value_.has_value();
    }

    [[nodiscard]] explicit operator bool() const {
        return Ok();
    }

    [[nodiscard]] const T& Value() const {
        if (!value_) {
            throw std::logic_error("LoadResult has no value");
        }
        return *value_;
    }

    [[nodiscard]] T& Value() {
        if (!value_) {
            throw std::logic_error("LoadResult has no value");
        }
        return *value_;
    }

    [[nodiscard]] const LoadError& Error() const {
        if (!error_) {
            throw std::logic_error("LoadResult has no error");
        }
        return *error_;
    }

private:
    explicit LoadResult(T value)
        : value_(std::move(value)) {}

    explicit LoadResult(LoadError error)
        : error_(std::move(error)) {}

    std::optional<T> value_;             // Present only on success.
    std::optional<LoadError> error_;     // Present only on failure.
};

}  // namespace trading::data
