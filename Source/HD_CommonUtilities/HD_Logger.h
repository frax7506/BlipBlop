#pragma once

#include "HD_Format.h"
#include "HD_GrowingArray.h"
#include "HD_Singleton.h"

#include <condition_variable>
#include <mutex>
#include <thread>

#define LOG_MESSAGE(aLogMessage) HD_Logger::GetInstance().Log(aLogMessage, HD_LogLevel_Log);
#define LOG_WARNING(aLogMessage) HD_Logger::GetInstance().Log(aLogMessage, HD_LogLevel_Warning);
#define LOG_ERROR(aLogMessage) HD_Logger::GetInstance().Log(aLogMessage, HD_LogLevel_Error);

#define LOG_MESSAGE_F(...) HD_Logger::GetInstance().Log(HD_Format(__VA_ARGS__).GetBuffer(), HD_LogLevel_Log);
#define LOG_WARNING_F(...) HD_Logger::GetInstance().Log(HD_Format(__VA_ARGS__).GetBuffer(), HD_LogLevel_Warning);
#define LOG_ERROR_F(...) HD_Logger::GetInstance().Log(HD_Format(__VA_ARGS__).GetBuffer(), HD_LogLevel_Error);

enum HD_LogLevel : char
{
	HD_LogLevel_Invalid,
	HD_LogLevel_Log,
	HD_LogLevel_Warning,
	HD_LogLevel_Error,
};

class HD_LogMessage
{
public:
	HD_LogMessage();
	HD_LogMessage(const char* aString);
	HD_LogMessage(const wchar_t* aString);
	~HD_LogMessage();

	HD_LogMessage& operator=(HD_LogMessage&& aLogMessage);

	const char* GetBuffer() const;
	const wchar_t* GetWBuffer() const;

	bool GetIsWide() const;

private:
	enum StringMode : char
	{
		StringMode_Invalid,
		StringMode_NonWide,
		StringMode_Wide,
	};

	char* myData;
	StringMode myMode;
};

class HD_LogEntry
{
public:
	HD_LogEntry();
	HD_LogEntry(const char* aLogMessage, HD_LogLevel aLogLevel);
	HD_LogEntry(const wchar_t* aLogMessage, HD_LogLevel aLogLevel);

	HD_LogEntry& operator=(HD_LogEntry&& aLogEntry);

	HD_LogMessage myLogMessage;
	HD_LogLevel myLogLevel;
};

class HD_Logger : public HD_Singleton<HD_Logger>
{
public:
	HD_Logger();
	~HD_Logger();

	void Log(const char* aLogMessage, HD_LogLevel aLogLevel);
	void Log(const wchar_t* aLogMessage, HD_LogLevel aLogLevel);

private:
	void LogThread();

	void PrintLogEntry(const HD_LogEntry& aLogEntry) const;

	std::thread myLogThread;
	bool myIsRunning;

	HD_GrowingArray<HD_LogEntry> myLogEntryQueue;
	std::mutex myLogEntriesMutex;
	std::condition_variable myLogEntriesConditionVariable;

	void* myStdErrHandle;
};
