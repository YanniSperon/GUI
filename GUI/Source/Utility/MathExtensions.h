#pragma once

#include "Vendor/glm/glm.hpp"

class Math {

	template <class T>
	static T Round(const T& input) {
		return T + 0.5;
	}

	template <class T>
	static T Map(const T& input, const T& inputRangeStart, const T& inputRangeEnd, const T& outputRangeStart, const T& outputRangeEnd) {
		return outputRangeStart + ((outputRangeEnd - outputRangeStart) / (inputRangeEnd - inputRangeStart)) * (input - inputRangeStart);
	}

	template<>
	static int Map(const int& input, const int& inputRangeStart, const int& inputRangeEnd, const int& outputRangeStart, const int& outputRangeEnd) {
		return outputRangeStart + Math::Round(((static_cast<double>(outputRangeEnd) - static_cast<double>(outputRangeStart)) / (static_cast<double>(inputRangeEnd) - static_cast<double>(inputRangeStart))) * (static_cast<double>(input) - static_cast<double>(inputRangeStart)));
	}

	template<>
	static glm::ivec2 Map(const glm::ivec2& input, const glm::ivec2& inputRangeStart, const glm::ivec2& inputRangeEnd, const glm::ivec2& outputRangeStart, const glm::ivec2& outputRangeEnd) {
		return glm::ivec2(glm::round(glm::dvec2(outputRangeStart) + ((glm::dvec2(outputRangeEnd) - glm::dvec2(outputRangeStart)) / (glm::dvec2(inputRangeEnd) - glm::dvec2(inputRangeStart))) * (glm::dvec2(input) - glm::dvec2(inputRangeStart))));
	}

	template<>
	static glm::ivec3 Map(const glm::ivec3& input, const glm::ivec3& inputRangeStart, const glm::ivec3& inputRangeEnd, const glm::ivec3& outputRangeStart, const glm::ivec3& outputRangeEnd) {
		return glm::ivec3(glm::round(glm::dvec3(outputRangeStart) + ((glm::dvec3(outputRangeEnd) - glm::dvec3(outputRangeStart)) / (glm::dvec3(inputRangeEnd) - glm::dvec3(inputRangeStart))) * (glm::dvec3(input) - glm::dvec3(inputRangeStart))));
	}

	template<>
	static glm::ivec4 Map(const glm::ivec4& input, const glm::ivec4& inputRangeStart, const glm::ivec4& inputRangeEnd, const glm::ivec4& outputRangeStart, const glm::ivec4& outputRangeEnd) {
		return glm::ivec4(glm::round(glm::dvec4(outputRangeStart) + ((glm::dvec4(outputRangeEnd) - glm::dvec4(outputRangeStart)) / (glm::dvec4(inputRangeEnd) - glm::dvec4(inputRangeStart))) * (glm::dvec4(input) - glm::dvec4(inputRangeStart))));
	}



	template <class T>
	static T Lerp(const double& time, const T& startValue, const T& endValue) {
		return startValue * (1.0 - time) + endValue * time;
	}

	template<>
	static int Lerp(const double& time, const int& startValue, const int& endValue) {
		return static_cast<double>(startValue) * (1.0 - time) + static_cast<double>(endValue) * time;
	}

	template<>
	static glm::ivec2 Lerp(const double& time, const glm::ivec2& startValue, const glm::ivec2& endValue) {
		return glm::ivec2(glm::round(glm::dvec2(startValue) * (1.0 - time) + glm::dvec2(endValue) * time));
	}

	template<>
	static glm::ivec3 Lerp(const double& time, const glm::ivec3& startValue, const glm::ivec3& endValue) {
		return glm::ivec3(glm::round(glm::dvec3(startValue) * (1.0 - time) + glm::dvec3(endValue) * time));
	}

	template<>
	static glm::ivec4 Lerp(const double& time, const glm::ivec4& startValue, const glm::ivec4& endValue) {
		return glm::ivec4(glm::round(glm::dvec4(startValue) * (1.0 - time) + glm::dvec4(endValue) * time));
	}
};
