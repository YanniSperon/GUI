#pragma once

#include <glm.hpp>

#include <string>

class Converter {
private:
	static unsigned long UncheckedStringToHex(const std::string& value);
public:
	static std::string HexToString(unsigned long value);
	static std::string HexToBaseString(unsigned long value);
	static unsigned long StringToHex(const std::string& value);
	static std::string CleanHexString(const std::string& value);
	static std::string RGBToHex(const glm::ivec3 color);
	static std::string RGBToHex(const glm::vec3 color);
	static glm::ivec3 HexToRGB(const std::string& color);
	static glm::vec3 HexToRGBF(const std::string& color);
};