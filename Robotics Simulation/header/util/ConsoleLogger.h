#pragma once
#include "util/ILogger.h"
#include <iostream>

class ConsoleLogger : public ILogger
{
public:
	explicit ConsoleLogger(LogLevel minLevel) : ILogger(minLevel) {}

	// Can not be refactored into template function easily.
	void Log(const std::string message, LogLevel level = LogLevel::INFO) override
	{
		if (level < MinLogLevel)
			return;


		std::cout
			<< LogLevelToString(level) << "\t"
			<< GetTimestamp() << ": "
			<< message << std::endl;

	}
};