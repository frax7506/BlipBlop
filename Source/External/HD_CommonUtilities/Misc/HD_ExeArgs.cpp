#include "HD_ExeArgs.h"

#if !IS_RETAIL_BUILD

// Exe args example:
// C/exePath -arg1 intValue -arg2 floatValue -arg3 boolValue -arg4 0.1 0.2 -arg5 0.1 0.2 0.3 -arg6 stringValue

void HD_ExeArgs::Init(int argc, const char* argv[])
{
	HD_String lastExeArgName;
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			// +1 in order to skip the '-'
			lastExeArgName = argv[i] + 1;
			myExeArgToValueMap[lastExeArgName];
		}
		else
		{
			HD_String& argValue = myExeArgToValueMap[lastExeArgName];

			if (argValue.GetLength() > 0)
			{
				argValue.Append(" ");
			}

			argValue.Append(argv[i]);
		}
	}
}

bool HD_ExeArgs::IsPresent(const char* aExeArgName) const
{
	return myExeArgToValueMap.GetIfExists(aExeArgName) != nullptr;
}

int HD_ExeArgs::GetValueInt(const char* aExeArgIntName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgIntName);
	if (!value)
		return 0;

	int result = 0;
	sscanf(value->GetBuffer(), "%i", &result);
	return result;
}

float HD_ExeArgs::GetValueFloat(const char* aExeArgFloatName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgFloatName);
	if (!value)
		return 0.f;

	float result = 0.f;
	sscanf(value->GetBuffer(), "%f", &result);
	return result;
}

bool HD_ExeArgs::GetValueBool(const char* aExeArgBoolName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgBoolName);
	if (!value)
		return false;

	// A bool as an exe arg is given as 0 or 1
	int result = 0;
	sscanf(value->GetBuffer(), "%i", &result);
	return result;
}

HD_Vector2f HD_ExeArgs::GetValueVector2f(const char* aExeArgVector2fName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgVector2fName);
	if (!value)
		return HD_Vector2f(0.f, 0.f);

	float x = 0.f;
	float y = 0.f;
	sscanf(value->GetBuffer(), "%f %f", &x, &y);
	return HD_Vector2f(x, y);
}

HD_Vector3f HD_ExeArgs::GetValueVector3f(const char* aExeArgVector3fName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgVector3fName);
	if (!value)
		return HD_Vector3f(0.f, 0.f, 0.f);

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	sscanf(value->GetBuffer(), "%f %f %f", &x, &y, &z);
	return HD_Vector3f(x, y, z);
}

HD_String HD_ExeArgs::GetValueString(const char* aExeArgStringName) const
{
	const HD_String* value = myExeArgToValueMap.GetIfExists(aExeArgStringName);
	if (!value)
		return "";

	char buffer[64]{ 0 };
	sscanf(value->GetBuffer(), "%s", buffer);
	return HD_String(buffer);
}

#endif // !IS_RETAIL_BUILD
