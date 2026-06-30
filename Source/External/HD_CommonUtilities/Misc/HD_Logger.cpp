#include "HD_Logger.h"

#include "OptimizedWindowsInclude.h"
#include "HD_StringUtils.h"

#include <iostream>

HD_LogMessage::HD_LogMessage()
	: myData(nullptr)
	, myMode(eStringMode_Invalid)
{
}

HD_LogMessage::HD_LogMessage(const HD_LogMessage& aLogMessage)
{
	InitFromLogMessage(aLogMessage);
}

HD_LogMessage::HD_LogMessage(HD_LogMessage&& aLogMessage)
	: myData(aLogMessage.myData)
	, myMode(aLogMessage.myMode)
{
	aLogMessage.myData = nullptr;
	aLogMessage.myMode = eStringMode_Invalid;
}

HD_LogMessage::HD_LogMessage(const char* aString)
	: myMode(eStringMode_NonWide)
{
	int length = HD_Strlen(aString);
	myData = new char[length + 1] { 0 };
	memcpy(myData, aString, length);
}

HD_LogMessage::HD_LogMessage(const wchar_t* aString)
	: myMode(eStringMode_Wide)
{
	int length = HD_Strlen(aString);
	myData = new char[length * (sizeof(wchar_t)) + sizeof(wchar_t)] { 0 };
	memcpy(myData, aString, length * sizeof(wchar_t));
}

HD_LogMessage::~HD_LogMessage()
{
	HD_SafeDeleteArray(myData);
}

HD_LogMessage& HD_LogMessage::operator=(const HD_LogMessage& aLogMessage)
{
	HD_SafeDeleteArray(myData);
	InitFromLogMessage(aLogMessage);

	return *this;
}

HD_LogMessage& HD_LogMessage::operator=(HD_LogMessage&& aLogMessage)
{
	HD_SafeDeleteArray(myData);

	myData = aLogMessage.myData;
	myMode = aLogMessage.myMode;

	aLogMessage.myData = nullptr;
	aLogMessage.myMode = eStringMode_Invalid;

	return *this;
}

const char* HD_LogMessage::GetBuffer() const
{
	assert(!GetIsWide() && "HD_LogMessage::GetBuffer was called on a wide or invalid instance.");
	return myData;
}

const wchar_t* HD_LogMessage::GetWBuffer() const
{
	assert(GetIsWide() && "HD_LogMessage::GetWBuffer was called on a non-wide or invalid instance.");
	return reinterpret_cast<const wchar_t*>(myData);
}

bool HD_LogMessage::GetIsWide() const
{
	return myMode == eStringMode_Wide;
}

void HD_LogMessage::InitFromLogMessage(const HD_LogMessage& aLogMessage)
{
	myMode = aLogMessage.myMode;

	if (myMode == eStringMode_Invalid)
	{
		myData = nullptr;
	}
	else if (myMode == eStringMode_NonWide)
	{
		int length = HD_Strlen<char>(aLogMessage.myData);
		myData = new char[length + 1] { 0 };
		memcpy(myData, aLogMessage.myData, length);
	}
	else // myMode == eStringMode_Wide
	{
		int length = HD_Strlen<wchar_t>(reinterpret_cast<wchar_t*>(aLogMessage.myData));
		myData = new char[length * (sizeof(wchar_t)) + sizeof(wchar_t)] { 0 };
		memcpy(myData, aLogMessage.myData, length * sizeof(wchar_t));
	}
}

HD_LogEntry::HD_LogEntry()
	: myLogLevel(eLogLevel_Invalid)
{
}

HD_LogEntry::HD_LogEntry(const HD_LogEntry& aLogEntry)
	: myLogMessage(aLogEntry.myLogMessage)
	, myLogLevel(aLogEntry.myLogLevel)
{
}

HD_LogEntry::HD_LogEntry(HD_LogEntry&& aLogEntry)
	: myLogMessage(HD_Move(aLogEntry.myLogMessage))
	, myLogLevel(aLogEntry.myLogLevel)
{
	aLogEntry.myLogLevel = eLogLevel_Invalid;
}

HD_LogEntry::HD_LogEntry(const char* aLogMessage, eLogLevel aLogLevel)
	: myLogMessage(aLogMessage)
	, myLogLevel(aLogLevel)
{
}

HD_LogEntry::HD_LogEntry(const wchar_t* aLogMessage, eLogLevel aLogLevel)
	: myLogMessage(aLogMessage)
	, myLogLevel(aLogLevel)
{
}

HD_LogEntry& HD_LogEntry::operator=(const HD_LogEntry& aLogEntry)
{
	myLogMessage = aLogEntry.myLogMessage;
	myLogLevel = aLogEntry.myLogLevel;

	return *this;
}

HD_LogEntry& HD_LogEntry::operator=(HD_LogEntry&& aLogEntry)
{
	myLogMessage = HD_Move(aLogEntry.myLogMessage);
	myLogLevel = aLogEntry.myLogLevel;

	aLogEntry.myLogLevel = eLogLevel_Invalid;

	return *this;
}

void HD_Logger::Init()
{
	myIsRunning = true;
	myStdErrHandle = GetStdHandle(STD_ERROR_HANDLE);
	myLogThread = std::thread(&HD_Logger::LogThread, this);
}

void HD_Logger::DeInit()
{
	myIsRunning = false;
	myLogEntriesConditionVariable.notify_all();
	myLogThread.join();
}

void HD_Logger::Log(const char* aLogMessage, eLogLevel aLogLevel)
{
	HD_LogEntry logEntry(aLogMessage, aLogLevel);

	{
		std::scoped_lock lock(myLogEntriesMutex);
		myLogEntryQueue.PushBack(HD_Move(logEntry));
	}

	myLogEntriesConditionVariable.notify_one();
}

void HD_Logger::Log(const wchar_t* aLogMessage, eLogLevel aLogLevel)
{
	HD_LogEntry logEntry(aLogMessage, aLogLevel);

	{
		std::scoped_lock lock(myLogEntriesMutex);
		myLogEntryQueue.PushBack(HD_Move(logEntry));
	}

	myLogEntriesConditionVariable.notify_one();
}

void HD_Logger::LogThread()
{
	HD_GrowingArray<HD_LogEntry> localLogEntryQueue;

	while (myIsRunning)
	{
		{
			std::unique_lock lock(myLogEntriesMutex);

			myLogEntriesConditionVariable.wait(lock, [this]
			{
				return !myIsRunning || !myLogEntryQueue.GetIsEmpty();
			});

			if (!myIsRunning && myLogEntryQueue.GetIsEmpty())
			{
				break;
			}

			HD_Swap(localLogEntryQueue, myLogEntryQueue);
		}

		for (int i = 0; i < localLogEntryQueue.GetSize(); i++)
		{
			PrintLogEntry(localLogEntryQueue[i]);
		}

		localLogEntryQueue.RemoveAll();
	}
}

void HD_Logger::PrintLogEntry(const HD_LogEntry& aLogEntry) const
{
	SetConsoleTextAttribute(myStdErrHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (aLogEntry.myLogLevel)
	{
	case eLogLevel_Log:
	{
		SetConsoleTextAttribute(myStdErrHandle, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		std::cerr << "[   LOG   ]";
		SetConsoleTextAttribute(myStdErrHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	}
	case eLogLevel_Warning:
	{
		SetConsoleTextAttribute(myStdErrHandle, BACKGROUND_RED | BACKGROUND_GREEN);
		std::cerr << "[ WARNING ]";
		SetConsoleTextAttribute(myStdErrHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	}
	case eLogLevel_Error:
	{
		SetConsoleTextAttribute(myStdErrHandle, BACKGROUND_RED);
		std::cerr << "[  ERROR  ]";
		SetConsoleTextAttribute(myStdErrHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	}
	case eLogLevel_Invalid:
		break;
	}

	if (aLogEntry.myLogMessage.GetIsWide())
	{
		std::wcerr << L" " << aLogEntry.myLogMessage.GetWBuffer() << '\n';
	}
	else
	{
		std::cerr << " " << aLogEntry.myLogMessage.GetBuffer() << '\n';
	}

	std::cerr.flush();

	SetConsoleTextAttribute(myStdErrHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
