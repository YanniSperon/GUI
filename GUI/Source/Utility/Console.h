#pragma once

class Console {
public:
	static void Success(const char* fmt, ...);
	static void Info(const char* fmt, ...);
	static void Warning(const char* fmt, ...);
	static void Error(const char* fmt, ...);
	static void FatalError(const char* fmt, ...);
	static void Assert(bool value, const char* fmt, ...);
};