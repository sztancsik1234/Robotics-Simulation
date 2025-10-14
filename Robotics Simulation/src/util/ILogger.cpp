#include "util/ILogger.h"
#include <chrono>

const std::string ILogger::LogLevelToString(LogLevel level)
{
	using enum LogLevel;
	switch (level)
	{
		case TRACE: return		"[TRACE]";
		case INFO: return		"[INFO]";
		case WARNING: return	"[WARN]";
		case ERROR: return		"[ERROR]";
		default: return			"[UNKNOWN]";
	}
}

std::string ILogger::GetTimestamp()
{
	auto now = std::chrono::system_clock::now();
	auto time_t = std::chrono::system_clock::to_time_t(now);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		now.time_since_epoch()) % 1000;
	
	return std::format("{:%H:%M:%S}", 
		std::chrono::system_clock::from_time_t(time_t));
}