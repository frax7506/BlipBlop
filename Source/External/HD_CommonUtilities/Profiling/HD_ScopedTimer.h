#pragma once

#define TITLE_BUFFER_SIZE 64

class HD_ScopedTimer
{
public:
	HD_ScopedTimer(const char* aTitle);
	~HD_ScopedTimer();

private:
	void Stop();

	char myTitle[TITLE_BUFFER_SIZE];
	unsigned long long myStartTimestampMicroseconds;
};
