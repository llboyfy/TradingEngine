#pragma once

#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

#include "bar.h"
#include "load_request.h"
#include "load_result.h"
#include "market_data_set.h"

namespace trading::data {

// Reads local development files and normalizes them into the shared shape.
class OfflineDataLoader {
public:
    [[nodiscard]] LoadResult<MarketDataSet> Load(const LoadRequest& request) const;

private:
    static constexpr const char* kExpectedHeader_ =
        "<TICKER>,<PER>,<DATE>,<TIME>,<OPEN>,<HIGH>,<LOW>,<CLOSE>,<VOL>,<OPENINT>";  // Required Stooq schema.

    // value: Source field text that may contain Windows line endings.
    [[nodiscard]] static std::string trimCr(std::string value);

    // line: Raw comma-separated source row.
    [[nodiscard]] static std::vector<std::string> splitCsvLine(const std::string& line);

    // value: Candidate trading date text.
    [[nodiscard]] static bool isYyyymmdd(const std::string& value);

    // value: Candidate intraday timestamp text.
    [[nodiscard]] static bool isHhmmss(const std::string& value);

    // code: Machine-readable failure category.
    // message: Human-readable diagnostic detail.
    // path: Source location related to the failure.
    // line: One-based source line when applicable.
    [[nodiscard]] static LoadError makeError(LoadErrorCode code,
                                             std::string message,
                                             std::filesystem::path path = {},
                                             std::size_t line = 0);

    // symbols: Accepted instruments; an empty set accepts all rows.
    // symbol: Candidate instrument from a source row.
    [[nodiscard]] static bool symbolAllowed(const std::unordered_set<std::string>& symbols,
                                            const std::string& symbol);

    // request: Date bounds selected by the caller.
    // date: Candidate trading date from a source row.
    [[nodiscard]] static bool dateAllowed(const LoadRequest& request, const std::string& date);

    // value: Numeric source field.
    // out: Parsed destination when conversion succeeds.
    [[nodiscard]] static bool parseDouble(const std::string& value, double& out);

    // path: Source file used for diagnostics.
    // lineNumber: One-based row location inside the source file.
    // line: Raw source row to normalize.
    [[nodiscard]] static LoadResult<Bar> parseBar(const std::filesystem::path& path,
                                                  std::size_t lineNumber,
                                                  const std::string& line);

    // path: Local Stooq text file or ignorable non-data file.
    // request: Caller filters reused for each parsed row.
    // symbols: Accepted instruments; an empty set accepts all rows.
    // dataSet: Destination collection for normalized rows.
    [[nodiscard]] LoadResult<bool> loadFile(const std::filesystem::path& path,
                                            const LoadRequest& request,
                                            const std::unordered_set<std::string>& symbols,
                                            MarketDataSet& dataSet) const;
};

}  // namespace trading::data
