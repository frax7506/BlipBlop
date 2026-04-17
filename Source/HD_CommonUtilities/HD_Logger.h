#pragma once

#include "HD_GrowingArray.h"
#include "HD_Singleton.h"
#include "HD_String.h"

#include <condition_variable>
#include <mutex>
#include <thread>

#define LOG_MESSAGE(aLogMessage) HD_Logger::GetInstance().Log(aLogMessage, HD_Logger::LogLevel_Log);
#define LOG_WARNING(aLogMessage) HD_Logger::GetInstance().Log(aLogMessage, HD_Logger::LogLevel_Warning);
#define LOG_ERROR(aLogMessage) HD_Logger::GetInstance().Log(aLogMessage, HD_Logger::LogLevel_Error);

class HD_Logger : public HD_Singleton<HD_Logger>
{
public:
	enum LogLevel : char
	{
		LogLevel_Invalid,
		LogLevel_Log,
		LogLevel_Warning,
		LogLevel_Error,
	};

	HD_Logger();
	~HD_Logger();

	void Log(const char* aLogMessage, LogLevel aLogLevel);

private:
	struct LogEntry
	{
		LogEntry& operator=(LogEntry&& aLogEntry)
		{
			myLogMessage = HD_Move(aLogEntry.myLogMessage);
			myLogLevel = aLogEntry.myLogLevel;

			return *this;
		}

		HD_String myLogMessage;
		LogLevel myLogLevel;
	};

	void LogThread();

	void PrintLogEntry(const LogEntry& aLogEntry) const;

	std::thread myLogThread;
	bool myIsRunning;

	HD_GrowingArray<LogEntry> myLogEntryQueue;
	std::mutex myLogEntriesMutex;
	std::condition_variable myLogEntriesConditionVariable;

	void* myStdErrHandle;
};
