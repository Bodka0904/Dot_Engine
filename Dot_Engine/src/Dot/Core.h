#pragma once


#ifdef D_DEBUG
	#define D_ENABLE_ASSERTS
#endif

#ifdef D_ENABLE_ASSERTS
	#define D_ASSERT(x, ...) { if(!(x)) { LOG_ERR("Assertion Failed: %s", __VA_ARGS__); __debugbreak(); } }
#else
	#define D_ASSERT(x, ...)
	#define D_CORE_ASSERT(x, ...)
#endif

