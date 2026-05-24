#pragma once

#include <cstdint>
#include <string>
#include <utility>

namespace trading::data {

// Normalized OHLCV row consumed by later backtest components.
class Bar {
public:
    Bar() = default;

    Bar(std::string symbol,
        std::string period,
        std::string date,
        std::string time,
        double open,
        double high,
        double low,
        double close,
        double volume,
        double openInterest)
        : symbol_(std::move(symbol)),
          period_(std::move(period)),
          date_(std::move(date)),
          time_(std::move(time)),
          open_(open),
          high_(high),
          low_(low),
          close_(close),
          volume_(volume),
          openInterest_(openInterest) {}

    [[nodiscard]] const std::string& Symbol() const { return symbol_; }
    [[nodiscard]] const std::string& Period() const { return period_; }
    [[nodiscard]] const std::string& Date() const { return date_; }
    [[nodiscard]] const std::string& Time() const { return time_; }
    [[nodiscard]] double Open() const { return open_; }
    [[nodiscard]] double High() const { return high_; }
    [[nodiscard]] double Low() const { return low_; }
    [[nodiscard]] double Close() const { return close_; }
    [[nodiscard]] double Volume() const { return volume_; }
    [[nodiscard]] double OpenInterest() const { return openInterest_; }

private:
    std::string symbol_;                 // Tradable instrument identifier.
    std::string period_;                 // Source frequency marker, such as daily.
    std::string date_;                   // Trading date in YYYYMMDD format.
    std::string time_;                   // Bar timestamp in HHMMSS format.
    double open_ = 0.0;                  // First traded price for the interval.
    double high_ = 0.0;                  // Highest traded price for the interval.
    double low_ = 0.0;                   // Lowest traded price for the interval.
    double close_ = 0.0;                 // Last traded price for the interval.
    double volume_ = 0.0;                // Reported traded quantity.
    double openInterest_ = 0.0;          // Reported outstanding contracts, when available.
};

}  // namespace trading::data
