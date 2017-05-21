#pragma once

// planet warm terra with life
namespace terra
{
#define MAKE_INSTANCE(classname)    \
    \
public:                             \
    static classname& GetInstance() \
    {                               \
        static classname T;         \
        return T;                   \
    }

#define DISABLE_COPY(classname)                      \
    \
public:                                              \
    classname(const classname&) = delete;            \
    classname& operator=(const classname&) = delete; \
    classname(classname&&) = default;

#define TO_STRING(classname) #classname

#define FILENAME(x) strrchr(x, '/') ? strrchr(x, '/') + 1 : x

#define LEVEL_NULL "\033[0m"
#define LEVEL_INFO "\033[32m"
#define LEVEL_WARNING "\033[33m"
#define LEVEL_ERROR "\033[31m"
#define LEVEL_BLUE "\033[33m"
#define LEVEL_PURPLE "\033[33m"

#ifndef NDEBUG
#define LOG_INFO(level, ...)                                       \
    {                                                              \
        fprintf(stdout, level);                                    \
        fprintf(stdout, "[%s:%d]\t", FILENAME(__FILE__), __LINE__); \
        fprintf(stdout, __VA_ARGS__);                              \
        fprintf(stdout, LEVEL_NULL);                               \
        fprintf(stdout, "\n");                                     \
    }
#else
#define LOG_INFO(...) (void)(0)
#endif  // NDEBUG

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
}