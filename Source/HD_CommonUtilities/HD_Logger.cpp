#include "stdafx.h"
#include "HD_Logger.h"

#include "OptimizedWindowsInclude.h"

#include <iostream>

HD_Logger::HD_Logger()
	: myIsRunning(true)
	, myStdErrHandle(GetStdHandle(STD_ERROR_HANDLE))
{
	myLogThread = std::thread(&HD_Logger::LogThread, this);
}

HD_Logger::~HD_Logger()
{
	myIsRunning = false;
	myLogEntriesConditionVariable.notify_all();
	myLogThread.join();
}

void HD_Logger::Log(const char* aLogMessage, LogLevel aLogLevel)
{
	LogEntry logEntry;
	logEntry.myLogMessage = aLogMessage;
	logEntry.myLogLevel = aLogLevel;

	{
		std::scoped_lock lock(myLogEntriesMutex);
		myLogEntryQueue.PushBack(HD_Move(logEntry));
	}

	myLogEntriesConditionVariable.notify_one();
}

void HD_Logger::LogThread()
{
	HD_GrowingArray<LogEntry> localLogEntryQueue;

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

void HD_Logger::PrintLogEntry(const LogEntry& aLogEntry) const
{
	SetConsoleTextAttribute(myStdErrHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (aLogEntry.myLogLevel)
	{
	case LogLevel_Log:
	{
		SetConsoleTextAttribute(myStdErrHandle, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		std::cerr << "[   LOG   ]";
		SetConsoleTextAttribute(myStdErrHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	}
	case LogLevel_Warning:
	{
		SetConsoleTextAttribute(myStdErrHandle, BACKGROUND_RED | BACKGROUND_GREEN);
		std::cerr << "[ WARNING ]";
		SetConsoleTextAttribute(myStdErrHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	}
	case LogLevel_Error:
	{
		SetConsoleTextAttribute(myStdErrHandle, BACKGROUND_RED);
		std::cerr << "[  ERROR  ]";
		SetConsoleTextAttribute(myStdErrHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	}
	case LogLevel_Invalid:
		break;
	}

	std::cerr << " " << aLogEntry.myLogMessage.GetBuffer() << '\n';
	std::cerr.flush();

	SetConsoleTextAttribute(myStdErrHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
