#pragma once
#include "util/ILogger.h"

// Mock logger for testing
class MockLogger : public ILogger {
public:
    MockLogger() : ILogger(LogLevel::TRACE) {}
    
    std::string lastMessage;
    LogLevel lastLevel;
    bool logCalled = false;

    void Log(const std::string message, LogLevel level) override;
};