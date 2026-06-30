#include "HD_Random.h"

#include <random>

namespace HD_Random
{
	static std::random_device rndSeed;
	static std::mt19937 rndEngine(rndSeed());
	static std::uniform_real_distribution<float> rndDist(std::uniform_real_distribution<float>(0.f, 1.f));

	float GetRandomFloat(float aMin, float aMax)
	{
		return rndDist(rndEngine) * (aMax - aMin) + aMin;
	}
}
