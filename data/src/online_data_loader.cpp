#include "online_data_loader.h"

namespace trading::data {

LoadResult<MarketDataSet> OnlineDataLoader::Load(const LoadRequest& request) const {
    return LoadResult<MarketDataSet>::Failure(
        LoadError(LoadErrorCode::NotImplemented,
                  "Online data loading is not implemented yet",
                  request.RootPath()));
}

}  // namespace trading::data
