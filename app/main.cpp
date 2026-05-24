#include <filesystem>
#include <iostream>

#include "data_loader.h"
#include "load_result.h"
#include "offline_data_loader.h"

namespace {

void PrintLoadError(const trading::data::LoadError& error) {
    std::cerr << "data load failed: " << error.Message();
    if (!error.Path().empty()) {
        std::cerr << " path=" << error.Path();
    }
    if (error.Line() != 0) {
        std::cerr << " line=" << error.Line();
    }
    std::cerr << std::endl;
}

}  // namespace

int main() {
    const std::filesystem::path dataRoot =
        "data/dev/historical/daily/uk/lse etfs/1/fb2e.uk.txt";  // Small local smoke-test sample.
    const trading::data::LoadRequest request(dataRoot, {"FB2E.UK"}, "20190502", "20190509");
    trading::data::OfflineDataLoader loader;

    const auto result = trading::data::LoadMarketData(loader, request);
    if (!result) {
        PrintLoadError(result.Error());
        return 1;
    }

    const auto& dataSet = result.Value();
    std::cout << "data load complete"
              << " symbols=" << dataSet.SymbolCount()
              << " bars=" << dataSet.BarCount()
              << std::endl;

    return 0;
}
