#pragma once
#include <string>

enum class LogLevel
{
	TRACE = 0,
	INFO,
	WARNING,
	ERROR
};

struct ILogger
{
	virtual ~ILogger() = default;
	virtual void Log(const std::string& message, LogLevel level = LogLevel::INFO) = 0;
};