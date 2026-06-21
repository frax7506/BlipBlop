#include "HD_Time.h"

#include <chrono>

Time_t HD_Time::GetCurrentTimeMS()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

Time_t HD_Time::GetCurrentTimeMicroseconds()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}
