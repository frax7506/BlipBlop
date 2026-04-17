#include "stdafx.h"
#include "HD_ExeArgs.h"

#if !IS_RETAIL_BUILD

// C/blabla -arg1 intValue -arg2 floatValue -arg3 boolValue
// -arg4 0.1 0.2 -arg5 0.1 0.2 0.3 -arg6 stringValue

void HD_ExeArgs::Init(int argc, char* argv[])
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
	return myExeArgToValueMap.GetIfExists(aExeArgName);
}

#endif // !IS_RETAIL_BUILD
