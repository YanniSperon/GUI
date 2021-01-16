#include "Converter.h"

#include <sstream>

unsigned long Converter::UncheckedStringToHex(const std::string& value)
{
	return std::stoul(value, nullptr, 16);
}

std::string Converter::HexToString(unsigned long value)
{
	std::stringstream stream;
	stream << std::hex << value;
	return std::string(stream.str());
}

std::string Converter::HexToBaseString(unsigned long value)
{
	std::stringstream stream;
	stream << std::hex << std::showbase << value;
	return std::string(stream.str());
}

unsigned long Converter::StringToHex(const std::string& value)
{
	std::string returnValue = value;
	returnValue.erase(std::remove_if(returnValue.begin(), returnValue.end(),
		[](const char& c) {
			return !((c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102));
		}),
		returnValue.end());
	return std::stoul(returnValue, nullptr, 16);
}

std::string Converter::CleanHexString(const std::string& value) {
	std::string returnValue = value;
	returnValue.erase(std::remove_if(returnValue.begin(), returnValue.end(),
		[](const char& c) {
			return !((c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102));
		}),
		returnValue.end());
	return returnValue;
}

std::string Converter::RGBToHex(const glm::ivec3 color)
{
	return HexToString(color.r) + HexToString(color.g) + HexToString(color.b);
}

std::string Converter::RGBToHex(const glm::vec3 color)
{
	return HexToString(glm::round(color.r * 255.0f)) + HexToString(glm::round(color.g * 255.0f)) + HexToString(glm::round(color.b * 255.0f));
}

glm::ivec3 Converter::HexToRGB(const std::string& color)
{
	std::string finalColorString = CleanHexString(color);
	if (finalColorString.size() == 3) {
		finalColorString.resize(6);
		finalColorString[5] = finalColorString[2];
		finalColorString[4] = finalColorString[2];
		finalColorString[3] = finalColorString[1];
		finalColorString[2] = finalColorString[1];
		finalColorString[1] = finalColorString[0];
		finalColorString[0] = finalColorString[0];
	}
	unsigned long value = UncheckedStringToHex(finalColorString);
	return glm::ivec3(((value >> 16) & 0xFF), ((value >> 8) & 0xFF), (value & 0xFF));
}

glm::vec3 Converter::HexToRGBF(const std::string& color)
{
	std::string finalColorString = CleanHexString(color);
	if (finalColorString.size() == 3) {
		finalColorString.resize(6);
		finalColorString[5] = finalColorString[2];
		finalColorString[4] = finalColorString[2];
		finalColorString[3] = finalColorString[1];
		finalColorString[2] = finalColorString[1];
		finalColorString[1] = finalColorString[0];
		finalColorString[0] = finalColorString[0];
	}
	unsigned long value = UncheckedStringToHex(finalColorString);
	return glm::vec3(((value >> 16) & 0xFF) / 255.0f, ((value >> 8) & 0xFF) / 255.0f, (value & 0xFF) / 255.0f);
}
