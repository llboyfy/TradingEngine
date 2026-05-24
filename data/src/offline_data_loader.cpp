#include "offline_data_loader.h"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace trading::data {

std::string OfflineDataLoader::trimCr(std::string value) {
    if (!value.empty() && value.back() == '\r') {
        value.pop_back();
    }
    return value;
}

std::vector<std::string> OfflineDataLoader::splitCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream stream(line);
    std::string field;
    while (std::getline(stream, field, ',')) {
        fields.push_back(trimCr(std::move(field)));
    }
    return fields;
}

bool OfflineDataLoader::isYyyymmdd(const std::string& value) {
    return value.size() == 8 &&
           std::all_of(value.begin(), value.end(), [](unsigned char ch) {
               return std::isdigit(ch) != 0;
           });
}

bool OfflineDataLoader::isHhmmss(const std::string& value) {
    return value.size() == 6 &&
           std::all_of(value.begin(), value.end(), [](unsigned char ch) {
               return std::isdigit(ch) != 0;
           });
}

LoadError OfflineDataLoader::makeError(LoadErrorCode code,
                                       std::string message,
                                       std::filesystem::path path,
                                       std::size_t line) {
    return LoadError(code, std::move(message), std::move(path), line);
}

bool OfflineDataLoader::symbolAllowed(const std::unordered_set<std::string>& symbols,
                                      const std::string& symbol) {
    return symbols.empty() || symbols.find(symbol) != symbols.end();
}

bool OfflineDataLoader::dateAllowed(const LoadRequest& request, const std::string& date) {
    if (request.StartDate() && date < *request.StartDate()) {
        return false;
    }
    if (request.EndDate() && date > *request.EndDate()) {
        return false;
    }
    return true;
}

bool OfflineDataLoader::parseDouble(const std::string& value, double& out) {
    char* end = nullptr;
    errno = 0;
    const double parsed = std::strtod(value.c_str(), &end);
    if (errno != 0 || end == value.c_str() || *end != '\0') {
        return false;
    }
    out = parsed;
    return true;
}

LoadResult<Bar> OfflineDataLoader::parseBar(const std::filesystem::path& path,
                                            std::size_t lineNumber,
                                            const std::string& line) {
    const auto fields = splitCsvLine(line);
    if (fields.size() != 10) {
        return LoadResult<Bar>::Failure(
            makeError(LoadErrorCode::InvalidRow, "Expected 10 CSV fields", path, lineNumber));
    }

    if (!isYyyymmdd(fields[2])) {
        return LoadResult<Bar>::Failure(
            makeError(LoadErrorCode::ParseError, "Expected date in YYYYMMDD format", path, lineNumber));
    }
    if (!isHhmmss(fields[3])) {
        return LoadResult<Bar>::Failure(
            makeError(LoadErrorCode::ParseError, "Expected time in HHMMSS format", path, lineNumber));
    }

    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
    double volume = 0.0;
    double openInterest = 0.0;

    if (!parseDouble(fields[4], open) ||
        !parseDouble(fields[5], high) ||
        !parseDouble(fields[6], low) ||
        !parseDouble(fields[7], close)) {
        return LoadResult<Bar>::Failure(
            makeError(LoadErrorCode::ParseError, "Failed to parse OHLC numeric fields", path, lineNumber));
    }

    if (!parseDouble(fields[8], volume) ||
        !parseDouble(fields[9], openInterest)) {
        return LoadResult<Bar>::Failure(
            makeError(LoadErrorCode::ParseError, "Failed to parse volume or open interest", path, lineNumber));
    }

    return LoadResult<Bar>::Success(
        Bar(fields[0], fields[1], fields[2], fields[3], open, high, low, close, volume, openInterest));
}

LoadResult<bool> OfflineDataLoader::loadFile(const std::filesystem::path& path,
                                             const LoadRequest& request,
                                             const std::unordered_set<std::string>& symbols,
                                             MarketDataSet& dataSet) const {
    if (path.filename() == ".DS_Store" || path.extension() != ".txt") {
        return LoadResult<bool>::Success(true);
    }

    std::ifstream file(path);
    if (!file) {
        return LoadResult<bool>::Failure(
            makeError(LoadErrorCode::IoError, "Failed to open file", path));
    }

    std::string line;             // Current source row.
    std::size_t lineNumber = 0;   // One-based line counter after the first read.
    if (!std::getline(file, line)) {
        return LoadResult<bool>::Success(true);
    }
    ++lineNumber;
    if (trimCr(line) != kExpectedHeader_) {
        return LoadResult<bool>::Failure(
            makeError(LoadErrorCode::InvalidHeader, "Unexpected Stooq header", path, lineNumber));
    }

    while (std::getline(file, line)) {
        ++lineNumber;
        line = trimCr(std::move(line));
        if (line.empty()) {
            continue;
        }

        auto parsed = parseBar(path, lineNumber, line);
        if (!parsed) {
            return LoadResult<bool>::Failure(parsed.Error());
        }

        auto bar = std::move(parsed.Value());
        if (!symbolAllowed(symbols, bar.Symbol()) || !dateAllowed(request, bar.Date())) {
            continue;
        }
        dataSet.AddBar(std::move(bar));
    }

    return LoadResult<bool>::Success(true);
}

LoadResult<MarketDataSet> OfflineDataLoader::Load(const LoadRequest& request) const {
    if (request.RootPath().empty()) {
        return LoadResult<MarketDataSet>::Failure(
            makeError(LoadErrorCode::InvalidRequest, "LoadRequest rootPath is empty"));
    }
    if (request.StartDate() && !isYyyymmdd(*request.StartDate())) {
        return LoadResult<MarketDataSet>::Failure(
            makeError(LoadErrorCode::InvalidRequest, "startDate must use YYYYMMDD format", request.RootPath()));
    }
    if (request.EndDate() && !isYyyymmdd(*request.EndDate())) {
        return LoadResult<MarketDataSet>::Failure(
            makeError(LoadErrorCode::InvalidRequest, "endDate must use YYYYMMDD format", request.RootPath()));
    }
    if (request.StartDate() && request.EndDate() && *request.StartDate() > *request.EndDate()) {
        return LoadResult<MarketDataSet>::Failure(
            makeError(LoadErrorCode::InvalidRequest, "startDate must be <= endDate", request.RootPath()));
    }

    std::error_code existsError;
    if (!std::filesystem::exists(request.RootPath(), existsError) || existsError) {
        return LoadResult<MarketDataSet>::Failure(
            makeError(LoadErrorCode::IoError, "rootPath does not exist", request.RootPath()));
    }

    std::unordered_set<std::string> symbols(request.Symbols().begin(), request.Symbols().end());  // Fast allow-list lookup.
    MarketDataSet dataSet;                                                                   // Accumulates normalized rows.

    std::error_code typeError;
    if (std::filesystem::is_regular_file(request.RootPath(), typeError)) {
        auto loaded = loadFile(request.RootPath(), request, symbols, dataSet);
        if (!loaded) {
            return LoadResult<MarketDataSet>::Failure(loaded.Error());
        }
        return LoadResult<MarketDataSet>::Success(std::move(dataSet));
    }
    if (typeError) {
        return LoadResult<MarketDataSet>::Failure(
            makeError(LoadErrorCode::IoError, typeError.message(), request.RootPath()));
    }

    std::error_code walkError;
    std::filesystem::recursive_directory_iterator it(
        request.RootPath(), std::filesystem::directory_options::skip_permission_denied, walkError);
    const std::filesystem::recursive_directory_iterator end;
    if (walkError) {
        return LoadResult<MarketDataSet>::Failure(
            makeError(LoadErrorCode::IoError, walkError.message(), request.RootPath()));
    }

    for (; it != end;) {
        const auto& entry = *it;
        const auto path = entry.path();
        if (!entry.is_regular_file()) {
            it.increment(walkError);
            if (walkError) {
                return LoadResult<MarketDataSet>::Failure(
                    makeError(LoadErrorCode::IoError, walkError.message(), path));
            }
            continue;
        }

        auto loaded = loadFile(path, request, symbols, dataSet);
        if (!loaded) {
            return LoadResult<MarketDataSet>::Failure(loaded.Error());
        }

        it.increment(walkError);
        if (walkError) {
            return LoadResult<MarketDataSet>::Failure(
                makeError(LoadErrorCode::IoError, walkError.message(), path));
        }
    }

    return LoadResult<MarketDataSet>::Success(std::move(dataSet));
}

}  // namespace trading::data
