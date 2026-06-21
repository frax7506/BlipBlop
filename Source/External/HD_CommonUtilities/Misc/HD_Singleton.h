#pragma once

// * Important note:
//		This does not enforce true single-ness. It's merely an interface
//		that enforces some degree of single-ness, but mostly provides the
//		GetInstance function. Many different instances of
//		Singleton-children can for instance be default constructed.
//		This means that some responsibility is put on the user to make
//		sure objects that are meant to be singletons are only accessed
//		through the GetInstance function.

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
