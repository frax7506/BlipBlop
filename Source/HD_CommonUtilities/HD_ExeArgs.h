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
	void Init(int argc, const char* argv[]);

	bool IsPresent(const char* aExeArgName) const;

	int GetValueInt(const char* aExeArgIntName) const;
	float GetValueFloat(const char* aExeArgFloatName) const;
	bool GetValueBool(const char* aExeArgBoolName) const;
	HD_Vector2f GetValueVector2f(const char* aExeArgVector2fName) const;
	HD_Vector3f GetValueVector3f(const char* aExeArgVector3fName) const;
	HD_String GetValueString(const char* aExeArgStringName) const;

private:
	HD_HashMap<HD_String, HD_String> myExeArgToValueMap;
};

#endif // !IS_RETAIL_BUILD
