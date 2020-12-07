#pragma once

#include <vector>
#include <string>
#include <chrono>

#include <glfw3.h>

class Global {
public:
	static void Initialize();
	static std::chrono::steady_clock::time_point& GetStartTime();
	static std::vector<std::string>& GetConsoleLog();
	static void Update();
	static void Cleanup();
};
