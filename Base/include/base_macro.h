#pragma once
#include <iostream>
namespace TerraX
{
#define MAKE_INSTANCE(classname) \
public: \
	static classname& GetInstance() \
	{ \
		static classname T; \
		return T; \
	}

#define DISABLE_COPY(classname) \
public: \
	classname(const classname&) = delete; \
	classname& operator=(const classname &) = delete;

#define TO_STRING(classname) #classname

#ifdef _WIN32
#define FILENAME(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#elif defined(__GNUC__)
#define FILENAME(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#endif

#define LOG_ERROR(...) {\
	fprintf(stderr, "%s: Line %d;\t", FILENAME(__FILE__), __LINE__);\
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr,"\n"); \
}

#define LOG_INFO(...) {\
	fprintf(stdout, "%s: Line %d;\t", FILENAME(__FILE__), __LINE__);\
	fprintf(stdout, __VA_ARGS__); \
	fprintf(stdout,"\n"); \
}

//#define TerraxAssert(expr) { std::cout<< __FILE__ << __LINE__ << __FUNCTION__ << #expr << std::endl;}
}