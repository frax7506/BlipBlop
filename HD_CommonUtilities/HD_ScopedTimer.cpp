#include "stdafx.h"
#include "HD_ScopedTimer.h"

#include "HD_Time.h"

HD_ScopedTimer::HD_ScopedTimer(const char* aTitle)
{
	memset(myTitle, 0, TITLE_BUFFER_SIZE);
	memcpy(myTitle, aTitle, strlen(aTitle));

	myStartTimestampMicroseconds = HD_Time::GetCurrentTimeMicroseconds();
}

HD_ScopedTimer::~HD_ScopedTimer()
{
	Stop();
}

void HD_ScopedTimer::Stop()
{
	const unsigned long long endTimestampMicroseconds = HD_Time::GetCurrentTimeMicroseconds();

	const unsigned long long duration = endTimestampMicroseconds - myStartTimestampMicroseconds;
	const double ms = duration / 1000.0;

	printf("HD_ScopedTimer: %s - %.3f ms\n", myTitle, ms);
}
