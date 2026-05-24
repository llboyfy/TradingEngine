#pragma once

#include <utility>

#include "load_request.h"

namespace trading::data {

// Static dispatch entry for loader types that expose a compatible load method.
template <typename Loader>
auto LoadMarketData(Loader& loader, const LoadRequest& request) {
    return loader.Load(request);
}

// Allows temporary loader instances without introducing a runtime interface.
template <typename Loader>
auto LoadMarketData(Loader&& loader, const LoadRequest& request) {
    return std::forward<Loader>(loader).Load(request);
}

}  // namespace trading::data
