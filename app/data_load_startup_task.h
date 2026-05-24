#pragma once

#include <filesystem>

#include "load_result.h"
#include "logger.h"
#include "startup_task.h"

namespace trading::app {

// Starts the local market data loading path.
class DataLoadStartupTask : public StartupTask {
public:
    DataLoadStartupTask(std::filesystem::path dataRoot, infra::Logger& logger);

    int Start() override;

private:
    // error: Context returned by the data module when loading fails.
    void printLoadError(const data::LoadError& error) const;

    std::filesystem::path dataRoot_;     // Root directory or file selected for local loading.
    infra::Logger& logger_;              // Shared application diagnostics sink.
};

}  // namespace trading::app
