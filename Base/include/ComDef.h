#pragma once

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

}