#pragma once
#include <string>

enum class LogLevel
{
	INFO = 0,
	WARNING,
	ERROR
};

struct ILogger
{
	virtual ~ILogger() = default;
	virtual void Log(const std::string& message, LogLevel level = LogLevel::INFO) = 0;
};