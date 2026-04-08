#pragma once

template<typename T>
class HD_Singleton
{
public:
	HD_Singleton(const HD_Singleton& aSingleton) = delete;
	HD_Singleton& operator=(const HD_Singleton& aSingleton) = delete;

	static T& GetInstance()
	{
		static T ourInstance;
		return ourInstance;
	}

protected:
	HD_Singleton() = default;
	~HD_Singleton() = default;
};
