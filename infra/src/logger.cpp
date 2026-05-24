#include "logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace trading::infra {
namespace {

std::string currentTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime{};
#if defined(_WIN32)
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    std::ostringstream stream;
    stream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return stream.str();
}

}  // namespace

Logger::Logger(std::filesystem::path logPath)
    : logPath_(std::move(logPath)) {
    if (logPath_.has_parent_path()) {
        std::filesystem::create_directories(logPath_.parent_path());
    }
    logFile_.open(logPath_, std::ios::app);
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.flush();
    }
}

void Logger::Info(const std::string& message) {
    write("INFO", message);
}

void Logger::Error(const std::string& message) {
    write("ERROR", message);
}

void Logger::write(const std::string& level, const std::string& message) {
    const std::string line = currentTimestamp() + " [" + level + "] " + message;
    if (level == "ERROR") {
        std::cerr << line << std::endl;
    } else {
        std::cout << line << std::endl;
    }

    if (logFile_.is_open()) {
        logFile_ << line << std::endl;
    }
}

}  // namespace trading::infra
