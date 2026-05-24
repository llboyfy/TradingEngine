#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace trading::data {

// Caller-provided constraints for a data retrieval operation.
class LoadRequest {
public:
    LoadRequest() = default;

    LoadRequest(std::filesystem::path rootPath,
                std::vector<std::string> symbols = {},
                std::optional<std::string> startDate = std::nullopt,
                std::optional<std::string> endDate = std::nullopt)
        : rootPath_(std::move(rootPath)),
          symbols_(std::move(symbols)),
          startDate_(std::move(startDate)),
          endDate_(std::move(endDate)) {}

    [[nodiscard]] const std::filesystem::path& RootPath() const { return rootPath_; }
    [[nodiscard]] const std::vector<std::string>& Symbols() const { return symbols_; }
    [[nodiscard]] const std::optional<std::string>& StartDate() const { return startDate_; }
    [[nodiscard]] const std::optional<std::string>& EndDate() const { return endDate_; }

    void SetRootPath(std::filesystem::path rootPath) { rootPath_ = std::move(rootPath); }
    void SetSymbols(std::vector<std::string> symbols) { symbols_ = std::move(symbols); }
    void SetStartDate(std::optional<std::string> startDate) { startDate_ = std::move(startDate); }
    void SetEndDate(std::optional<std::string> endDate) { endDate_ = std::move(endDate); }

private:
    std::filesystem::path rootPath_;          // Directory or endpoint root selected by the loader.
    std::vector<std::string> symbols_;        // Optional allow-list of instrument identifiers.
    std::optional<std::string> startDate_;    // Inclusive lower bound in YYYYMMDD format.
    std::optional<std::string> endDate_;      // Inclusive upper bound in YYYYMMDD format.
};

}  // namespace trading::data
