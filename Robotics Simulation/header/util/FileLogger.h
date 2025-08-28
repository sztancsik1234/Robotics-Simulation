#pragma once
#include <fstream>
#include "ILogger.h"
#include <filesystem>

namespace fs = std::filesystem;

class FileLogger : public ILogger
{
public:
	/// <summary>
	/// Constructs a FileLogger that logs messages to a file in the specified directory.
	/// </summary>
	/// <param name="path">directory, where the log files are kept</param>
	FileLogger(fs::path path, LogLevel minLevel);
	~FileLogger() = default;

	void Log(const std::string message, LogLevel level = LogLevel::INFO) override;
private:
	std::ofstream logFile;
};