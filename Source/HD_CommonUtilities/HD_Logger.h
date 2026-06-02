#pragma once

#include "HD_Format.h"
#include "HD_GrowingArray.h"
#include "HD_Singleton.h"

#include <condition_variable>
#include <mutex>
#include <thread>

#define LOG_MESSAGE(aLogMessage) HD_Logger::GetInstance().Log(aLogMessage, eLogLevel_Log);
#define LOG_WARNING(aLogMessage) HD_Logger::GetInstance().Log(aLogMessage, eLogLevel_Warning);
#define LOG_ERROR(aLogMessage) HD_Logger::GetInstance().Log(aLogMessage, eLogLevel_Error);

#define LOG_MESSAGE_F(...) HD_Logger::GetInstance().Log(HD_Format(__VA_ARGS__).GetBuffer(), eLogLevel_Log);
#define LOG_WARNING_F(...) HD_Logger::GetInstance().Log(HD_Format(__VA_ARGS__).GetBuffer(), eLogLevel_Warning);
#define LOG_ERROR_F(...) HD_Logger::GetInstance().Log(HD_Format(__VA_ARGS__).GetBuffer(), eLogLevel_Error);

enum eLogLevel : char
{
	eLogLevel_Invalid,
	eLogLevel_Log,
	eLogLevel_Warning,
	eLogLevel_Error,
};

class HD_LogMessage
{
public:
	HD_LogMessage();
	HD_LogMessage(const HD_LogMessage& aLogMessage);
	HD_LogMessage(HD_LogMessage&& aLogMessage);
	HD_LogMessage(const char* aString);
	HD_LogMessage(const wchar_t* aString);
	~HD_LogMessage();

	HD_LogMessage& operator=(const HD_LogMessage& aLogMessage);
	HD_LogMessage& operator=(HD_LogMessage&& aLogMessage);

	const char* GetBuffer() const;
	const wchar_t* GetWBuffer() const;

	bool GetIsWide() const;

private:
	enum eStringMode : char
	{
		eStringMode_Invalid,
		eStringMode_NonWide,
		eStringMode_Wide,
	};

	void InitFromLogMessage(const HD_LogMessage& aLogMessage);

	char* myData;
	eStringMode myMode;
};

class HD_LogEntry
{
public:
	HD_LogEntry();
	HD_LogEntry(const HD_LogEntry& aLogEntry);
	HD_LogEntry(HD_LogEntry&& aLogEntry);
	HD_LogEntry(const char* aLogMessage, eLogLevel aLogLevel);
	HD_LogEntry(const wchar_t* aLogMessage, eLogLevel aLogLevel);

	HD_LogEntry& operator=(const HD_LogEntry& aLogEntry);
	HD_LogEntry& operator=(HD_LogEntry&& aLogEntry);

	HD_LogMessage myLogMessage;
	eLogLevel myLogLevel;
};

class HD_Logger : public HD_Singleton<HD_Logger>
{
public:
	HD_Logger();
	~HD_Logger();

	void Log(const char* aLogMessage, eLogLevel aLogLevel);
	void Log(const wchar_t* aLogMessage, eLogLevel aLogLevel);

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
