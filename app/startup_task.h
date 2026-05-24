#pragma once

namespace trading::app {

// Common entry point for application startup steps.
class StartupTask {
public:
    virtual ~StartupTask() = default;

    virtual int Start() = 0;
};

}  // namespace trading::app
