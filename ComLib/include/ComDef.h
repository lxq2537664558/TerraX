#pragma once

namespace TerraX
{
#define CreateInstance(classname) \
public: \
	static classname& GetInstance() \
	{ \
		static classname T; \
		return T; \
	}
}