#pragma once

// A simple class for a string that contains either 1b or 2b characters.
	// * Normally, HD_String or HD_WString are used, but in some cases a class
	//		like this can be useful.
	// * As of now, this class is only used in HD_Logger. In UI, tools or debug
	//		code the regular string classes should be used. I'm keeping the
	//		interface of this class intentionally scaled back because I want
	//		this to be as simple as it can be. At the moment this is all that
	//		the logger class needs.

class HD_HybridString
{
public:
	HD_HybridString();
	HD_HybridString(const char* aString);
	HD_HybridString(const wchar_t* aString);
	~HD_HybridString();

	const char* GetBuffer() const;
	const wchar_t* GetWBuffer() const;

	bool GetIsWide() const;

private:
	char* myData;
	bool myIsWide;
};
