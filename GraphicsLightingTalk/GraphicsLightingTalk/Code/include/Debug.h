#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <stdarg.h>
#include <bitset>
#include <csignal>
#include <Windows.h>
#include "InputManager.h"

#ifdef DEBUG
#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: " << message << " in file " << (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__) << " at line " << __LINE__ << std::endl; \
            __debugbreak(); \
        } \
    } while(0)

#else

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: " << message << " in file " << (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__) << " at line " << __LINE__ << std::endl; \
            abort(); \
        } \
    } while(0)

#endif // DEBUG


#define DEBUG_LOG(debugMessage, ...) \
	do { \
			std::string file = (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__); \
			std::string line = std::to_string(__LINE__); \
			DebugLog(file, line, debugMessage, ## __VA_ARGS__); \
	} while (0)

class Log 
{
public:
	static Log& Get();
	void OpenFile(std::filesystem::path const& filename);
	void Print(const char* format, ...);
	void Destroy();

private:
	Log();
	~Log();
	inline static Log* instance = NULL;
	std::ofstream file;
	bool isOpened;
};

void DebugLog(std::string file, std::string line, const char* debugMessage, ...);