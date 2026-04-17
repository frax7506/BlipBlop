#pragma once

#include <random>

class HD_Random
{
public:
	static float GetRandomFloat(float aMin, float aMax);

private:
	HD_Random();

	static HD_Random& GetInstance();

	std::random_device myRndSeed;
	std::mt19937 myRndEngine;
	std::uniform_real_distribution<float> myRndDist;
};
