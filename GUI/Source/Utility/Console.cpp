#include "Console.h"
#include "Global.h"

#include <cstdio>
#include <cstdarg>
#include <chrono>
#include <mutex>
#include <string>

#ifdef AD_PLATFORM_WINDOWS
#include <Windows.h>
#endif

static std::mutex printMutex;

void Console::Success(const char* fmt, ...)
{
	printMutex.lock();
	va_list args;
	va_start(args, fmt);
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - Global::GetStartTime());
	auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration - hours);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration - hours - minutes);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration - hours - minutes - seconds);
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration - hours - minutes - seconds - milliseconds);
#ifdef AD_PLATFORM_WINDOWS
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
#endif
	char* timestamp = new char[256];
	snprintf(timestamp, 256, "%02ld:%02ld:%02lld.%03lld%03lld --    SUCCESS:    ", hours.count(), minutes.count(), seconds.count(), milliseconds.count(), microseconds.count());
	char* message = new char[1024];
	vsnprintf(message, 1024, fmt, args);
	std::string returnValue = std::string(timestamp) + std::string(message);
	printf("%s\n", returnValue.c_str());
	Global::GetConsoleLog().push_back(returnValue);
	if (Global::GetConsoleLog().size() > 50) {
		Global::GetConsoleLog().erase(Global::GetConsoleLog().begin());
	}
	va_end(args);
	delete[] timestamp;
	delete[] message;
	printMutex.unlock();
}

void Console::Info(const char* fmt, ...)
{
	printMutex.lock();
	va_list args;
	va_start(args, fmt);
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - Global::GetStartTime());
	auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration - hours);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration - hours - minutes);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration - hours - minutes - seconds);
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration - hours - minutes - seconds - milliseconds);
#ifdef AD_PLATFORM_WINDOWS
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
#endif
	char* timestamp = new char[256];
	snprintf(timestamp, 256, "%02ld:%02ld:%02lld.%03lld%03lld --      INFO:     ", hours.count(), minutes.count(), seconds.count(), milliseconds.count(), microseconds.count());
	char* message = new char[1024];
	vsnprintf(message, 1024, fmt, args);
	std::string returnValue = std::string(timestamp) + std::string(message);
	printf("%s\n", returnValue.c_str());
	Global::GetConsoleLog().push_back(returnValue);
	if (Global::GetConsoleLog().size() > 50) {
		Global::GetConsoleLog().erase(Global::GetConsoleLog().begin());
	}
	va_end(args);
	delete[] timestamp;
	delete[] message;
	printMutex.unlock();
}

void Console::Warning(const char* fmt, ...)
{
	printMutex.lock();
	va_list args;
	va_start(args, fmt);
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - Global::GetStartTime());
	auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration - hours);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration - hours - minutes);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration - hours - minutes - seconds);
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration - hours - minutes - seconds - milliseconds);
#ifdef AD_PLATFORM_WINDOWS
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
#endif
	char* timestamp = new char[256];
	snprintf(timestamp, 256, "%02ld:%02ld:%02lld.%03lld%03lld --    WARNING:    ", hours.count(), minutes.count(), seconds.count(), milliseconds.count(), microseconds.count());
	char* message = new char[1024];
	vsnprintf(message, 1024, fmt, args);
	std::string returnValue = std::string(timestamp) + std::string(message);
	printf("%s\n", returnValue.c_str());
	Global::GetConsoleLog().push_back(returnValue);
	if (Global::GetConsoleLog().size() > 50) {
		Global::GetConsoleLog().erase(Global::GetConsoleLog().begin());
	}
	va_end(args);
	delete[] timestamp;
	delete[] message;
	printMutex.unlock();
}

void Console::Error(const char* fmt, ...)
{
	printMutex.lock();
	va_list args;
	va_start(args, fmt);
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - Global::GetStartTime());
	auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration - hours);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration - hours - minutes);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration - hours - minutes - seconds);
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration - hours - minutes - seconds - milliseconds);
#ifdef AD_PLATFORM_WINDOWS
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
#endif
	char* timestamp = new char[256];
	snprintf(timestamp, 256, "%02ld:%02ld:%02lld.%03lld%03lld --     ERROR:     ", hours.count(), minutes.count(), seconds.count(), milliseconds.count(), microseconds.count());
	char* message = new char[1024];
	vsnprintf(message, 1024, fmt, args);
	std::string returnValue = std::string(timestamp) + std::string(message);
	printf("%s\n", returnValue.c_str());
	Global::GetConsoleLog().push_back(returnValue);
	if (Global::GetConsoleLog().size() > 50) {
		Global::GetConsoleLog().erase(Global::GetConsoleLog().begin());
	}
	va_end(args);
	delete[] timestamp;
	delete[] message;
	printMutex.unlock();
}

void Console::FatalError(const char* fmt, ...)
{
	printMutex.lock();
	va_list args;
	va_start(args, fmt);
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - Global::GetStartTime());
	auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration - hours);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration - hours - minutes);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration - hours - minutes - seconds);
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration - hours - minutes - seconds - milliseconds);
#ifdef AD_PLATFORM_WINDOWS
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 79);
#endif
	char* timestamp = new char[256];
	snprintf(timestamp, 256, "%02ld:%02ld:%02lld.%03lld%03lld --  FATAL ERROR:  ", hours.count(), minutes.count(), seconds.count(), milliseconds.count(), microseconds.count());
	char* message = new char[1024];
	vsnprintf(message, 1024, fmt, args);
	std::string returnValue = std::string(timestamp) + std::string(message);
	printf("%s\n", returnValue.c_str());
	Global::GetConsoleLog().push_back(returnValue);
	if (Global::GetConsoleLog().size() > 50) {
		Global::GetConsoleLog().erase(Global::GetConsoleLog().begin());
	}
	va_end(args);
	delete[] timestamp;
	delete[] message;
	printMutex.unlock();
}

void Console::Assert(bool value, const char* fmt, ...)
{
#ifdef AD_DEBUG
	if (!value) {
		va_list args;
		va_start(args, fmt);
		FatalError(fmt, args);
		va_end(args);
	}
#endif
}
