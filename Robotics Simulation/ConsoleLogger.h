#pragma once
#include "ILogger.h"
#include <iostream>

class ConsoleLogger : public ILogger
{
public:
	void Log(const std::string& message, LogLevel level) override
	{
		if (level == LogLevel::WARNING)
			std::cout << "[WARNING] " << message << std::endl;
		else if (level == LogLevel::ERROR)
			std::cerr << "[ERROR] " << message << std::endl;
		else
			std::cout << "[INFO] " << message << std::endl;
	}
};