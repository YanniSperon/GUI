#pragma once

class Random {
private:

public:
	// A float including low excluding high
	static float FloatIncExcl(float low, float high);
	// A double including low excluding high
	static double DoubleIncExcl(double low, double high);
};