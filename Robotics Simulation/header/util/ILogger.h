#pragma once
#include <string>
#include <format>
#include <chrono>

enum class LogLevel : char
{
	TRACE = 0,
	INFO,
	WARNING,
	ERROR
};

struct ILogger
{
	ILogger(LogLevel minLevel) : MinLogLevel(minLevel), startTime(std::chrono::steady_clock::now()) {}
	virtual ~ILogger() = default;
	virtual void Log(const std::string message, LogLevel level = LogLevel::INFO) = 0;
protected:
	LogLevel MinLogLevel;
	const std::string LogLevelToString(LogLevel level);
	std::string GetTimestamp();
private:
	std::chrono::time_point<std::chrono::steady_clock> startTime;
};