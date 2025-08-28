#include "util/FileLogger.h"
#include <chrono>

FileLogger::FileLogger(fs::path path, LogLevel minLevel) : ILogger(minLevel)
{
	if (path.has_extension())
	{
		throw std::invalid_argument("Path is not a directory");
	}

	// check if path exists
	if (!fs::exists(path))
	{
		//create directory
		if (!fs::create_directories(path))
		{
			throw std::runtime_error("Could not create directory");
		}
	}

	// create log file with current date and time as name
	auto now = std::chrono::system_clock::now();
	auto fileName = std::format("log_{:%Y-%m-%d_%H-%M-%S}.txt", now);
	logFile.open(path / fileName, std::ios::out);

	if (!logFile.is_open())
	{
		throw std::runtime_error("Could not open log file");
	}
}

void FileLogger::Log(const std::string message, const LogLevel level)
{
	using enum LogLevel;

	if (level < MinLogLevel)
	{
		return;
	}

	std::string levelStr = LogLevelToString(level);
	std::string timestamp = GetTimestamp();

	logFile << std::format("{}\t{}: {}", levelStr, timestamp, message) << std::endl;
}

