#pragma once
#include <iostream>
#include "util/ILogger.h"

class ConsoleLogger : public ILogger
{
public:
	explicit ConsoleLogger(LogLevel minLevel) : currentLogLevel(minLevel) {}

	// TODO: Can be refactored into  template function
	void Log(const std::string& message, LogLevel level = LogLevel::INFO) override
	{
		if (level < currentLogLevel)
			return;

		switch (level)
		{
			using enum LogLevel;
			case TRACE:
				std::cout << "[TRACE] " << message << std::endl;
				break;
			case INFO:
				std::cout << "[INFO] " << message << std::endl;
				break;
			case WARNING:
				std::cout << "[WARNING] " << message << std::endl;
				break;
			case ERROR:
				std::cerr << "[ERROR] " << message << std::endl;
				break;
			default:
				std::cerr << "[UNKNOWN] " << message << std::endl;
				break;
		}
	}
private:
	LogLevel currentLogLevel;
};