#pragma once

template<typename First, typename Second>
class HD_Pair
{
public:
	HD_Pair()
		: myFirst()
		, mySecond()
	{
	}

	HD_Pair(const First& aFirst, const Second& aSecond)
		: myFirst(aFirst)
		, mySecond(aSecond)
	{
	}

	First myFirst;
	Second mySecond;
};
