#pragma once

#include <filesystem>
#include <fstream>
#include <string>

namespace trading::infra {

// Writes application messages to console and the runtime temp directory.
class Logger {
public:
    explicit Logger(std::filesystem::path logPath);
    ~Logger();

    // message: Human-readable application event.
    void Info(const std::string& message);

    // message: Human-readable failure detail.
    void Error(const std::string& message);

private:
    // level: Short severity label.
    // message: Human-readable application event.
    void write(const std::string& level, const std::string& message);

    std::filesystem::path logPath_;     // File destination under the runtime temp directory.
    std::ofstream logFile_;             // Append-only output stream for local diagnostics.
};

}  // namespace trading::infra
