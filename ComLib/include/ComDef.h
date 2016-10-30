#pragma once

namespace TerraX
{
#define MAKEINSTANCE(classname) \
public: \
	static classname& GetInstance() \
	{ \
		static classname T; \
		return T; \
	}

#define NOCOPY(classname) \
public: \
	classname(const classname&) = delete; \
	classname& operator=(const classname &) = delete;

}