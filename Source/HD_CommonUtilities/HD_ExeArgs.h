#pragma once

#include "HD_PreprocessorMacros.h"

#if !IS_RETAIL_BUILD

#include "HD_HashMap.h"
#include "HD_Singleton.h"
#include "HD_String.h"
#include "HD_Vector2.h"
#include "HD_Vector3.h"

class HD_ExeArgs : public HD_Singleton<HD_ExeArgs>
{
public:
	void Init(int argc, char* argv[]);

	bool IsPresent(const char* aExeArgName) const;

	template<typename T>
	T GetValue(const char* aExeArgName) const;

private:
	HD_HashMap<HD_String, HD_String> myExeArgToValueMap;
};

template<>
int HD_ExeArgs::GetValue(const char* aExeArgName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgName);
	if (!value)
		return 0;

	int result = 0;
	sscanf(value->GetBuffer(), "%i", &result);
	return result;
}

template<>
float HD_ExeArgs::GetValue(const char* aExeArgName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgName);
	if (!value)
		return 0.f;

	float result = 0.f;
	sscanf(value->GetBuffer(), "%f", &result);
	return result;
}

template<>
bool HD_ExeArgs::GetValue(const char* aExeArgName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgName);
	if (!value)
		return false;

	// A bool as an exe arg is given as 0 or 1
	int result = 0;
	sscanf(value->GetBuffer(), "%i", &result);
	return result;
}

template<>
HD_Vector2f HD_ExeArgs::GetValue(const char* aExeArgName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgName);
	if (!value)
		return HD_Vector2f(0.f, 0.f);

	float x = 0.f;
	float y = 0.f;
	sscanf(value->GetBuffer(), "%f %f", &x, &y);
	return HD_Vector2f(x, y);
}

template<>
HD_Vector3f HD_ExeArgs::GetValue(const char* aExeArgName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgName);
	if (!value)
		return HD_Vector3f(0.f, 0.f, 0.f);

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	sscanf(value->GetBuffer(), "%f %f %f", &x, &y, &z);
	return HD_Vector3f(x, y, z);
}

template<>
HD_String HD_ExeArgs::GetValue(const char* aExeArgName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgName);
	if (!value)
		return "";

	char buffer[64]{ 0 };
	sscanf(value->GetBuffer(), "%s", buffer);
	return HD_String(buffer);
}

#endif // !IS_RETAIL_BUILD
