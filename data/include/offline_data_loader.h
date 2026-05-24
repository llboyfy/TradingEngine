#pragma once

#include "load_request.h"
#include "load_result.h"
#include "market_data_set.h"

namespace trading::data {

// Reads local development files and normalizes them into the shared shape.
class OfflineDataLoader {
public:
    [[nodiscard]] LoadResult<MarketDataSet> Load(const LoadRequest& request) const;
};

}  // namespace trading::data
