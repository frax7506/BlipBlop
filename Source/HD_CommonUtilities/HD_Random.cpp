#include "stdafx.h"
#include "HD_Random.h"

float HD_Random::GetRandomFloat(float aMin, float aMax)
{
	HD_Random& random = GetInstance();
	return random.myRndDist(random.myRndEngine) * (aMax - aMin) + aMin;
}

HD_Random::HD_Random()
	: myRndEngine(myRndSeed())
	, myRndDist(std::uniform_real_distribution<float>(0.f, 1.f))
{
}

HD_Random& HD_Random::GetInstance()
{
	static HD_Random random;
	return random;
}
