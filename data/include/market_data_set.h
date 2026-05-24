#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "bar.h"

namespace trading::data {

// Symbol-indexed collection returned by every data source.
class MarketDataSet {
public:
    using SymbolBars = std::map<std::string, std::vector<Bar>>;

    void AddBar(Bar bar) {
        const auto symbol = bar.Symbol();
        barsBySymbol_[symbol].push_back(std::move(bar));
    }

    [[nodiscard]] bool Empty() const {
        return barsBySymbol_.empty();
    }

    [[nodiscard]] std::size_t SymbolCount() const {
        return barsBySymbol_.size();
    }

    [[nodiscard]] std::size_t BarCount() const {
        std::size_t total = 0;
        for (const auto& entry : barsBySymbol_) {
            total += entry.second.size();
        }
        return total;
    }

    [[nodiscard]] const SymbolBars& BarsBySymbol() const {
        return barsBySymbol_;
    }

private:
    SymbolBars barsBySymbol_;     // Deterministic ordering for repeatable tests.
};

}  // namespace trading::data
