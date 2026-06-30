#pragma once

// * Important note:
//		This does not enforce true single-ness. It's merely an interface
//		that enforces some degree of single-ness, but mostly provides the
//		GetInstance function. Many different instances of
//		Singleton-children can for instance be default constructed.
//		This means that some responsibility is put on the user to make
//		sure objects that are meant to be singletons are only accessed
//		through the GetInstance function.
//
//		The singleton pattern and inheritance do *not* play well together.
//		Not having this class at all is also a solution, and instead letting
//		all classes that are supposed to be singletons be responsible for
//		their own single-ness implementation. However it is nice to have
//		a common interface, including its flaws. And I feel like the most
//		likely way to break it is to break it intentionally, which make
//		its drawbacks less bad.

template<typename T>
class HD_Singleton
{
public:
	static T& GetInstance()
	{
		static T ourInstance;
		return ourInstance;
	}

protected:
	HD_Singleton() = default;
	~HD_Singleton() = default;

	HD_Singleton(const HD_Singleton& aSingleton) = delete;
	HD_Singleton& operator=(const HD_Singleton& aSingleton) = delete;
};
