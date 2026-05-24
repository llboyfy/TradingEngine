#pragma once

#include "load_request.h"
#include "load_result.h"
#include "market_data_set.h"

namespace trading::data {

// Placeholder for future remote retrieval while preserving the shared shape.
class OnlineDataLoader {
public:
    [[nodiscard]] LoadResult<MarketDataSet> Load(const LoadRequest& request) const;
};

}  // namespace trading::data
