#include "Random.h"

#include <random>

float Random::FloatIncExcl(float low, float high)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(low, high);
	return dis(gen);
}

double Random::DoubleIncExcl(double low, double high)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(low, high);
	return dis(gen);
}
