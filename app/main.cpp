#include "data_load_startup_task.h"
#include "logger.h"
#include "startup_task.h"

int main() {
    trading::infra::Logger logger("temp/trading_engine.log");
    trading::app::DataLoadStartupTask dataLoadTask("data/dev/historical/daily/uk", logger);
    trading::app::StartupTask& startupTask = dataLoadTask;

    return startupTask.Start();
}
