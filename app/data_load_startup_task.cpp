#include "data_load_startup_task.h"

#include <sstream>
#include <utility>

#include "data_loader.h"
#include "load_result.h"
#include "offline_data_loader.h"

namespace trading::app {

DataLoadStartupTask::DataLoadStartupTask(std::filesystem::path dataRoot, infra::Logger& logger)
    : dataRoot_(std::move(dataRoot)),
      logger_(logger) {}

int DataLoadStartupTask::Start() {
    logger_.Info("starting data load root=" + dataRoot_.string());

    const data::LoadRequest request(dataRoot_);
    data::OfflineDataLoader loader;

    const auto result = data::LoadMarketData(loader, request);
    if (!result) {
        printLoadError(result.Error());
        return 1;
    }

    const auto& dataSet = result.Value();
    std::ostringstream message;
    message << "data load complete"
            << " symbols=" << dataSet.SymbolCount()
            << " bars=" << dataSet.BarCount();
    logger_.Info(message.str());

    return 0;
}

void DataLoadStartupTask::printLoadError(const data::LoadError& error) const {
    std::ostringstream message;
    message << "data load failed: " << error.Message();
    if (!error.Path().empty()) {
        message << " path=" << error.Path();
    }
    if (error.Line() != 0) {
        message << " line=" << error.Line();
    }
    logger_.Error(message.str());
}

}  // namespace trading::app
