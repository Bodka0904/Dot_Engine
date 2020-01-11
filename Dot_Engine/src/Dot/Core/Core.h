#pragma once
#include <memory>

#ifdef D_DEBUG
	#define D_ENABLE_ASSERTS
#endif

#ifdef D_ENABLE_ASSERTS
	#define D_ASSERT(x, ...) { if(!(x)) { LOG_ERR("Assertion Failed: %s", __VA_ARGS__); __debugbreak(); } }
#else
	#define D_ASSERT(x, ...)
#endif

//0th argument stands for Function to call,
//1th must be called because it is class member function,
//2th is placeholder for agument
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
#define BIT(x) 1 << x

#ifndef __FUNCTION__
#define __FUNCTION__ "Function:"
#endif


namespace Dot {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}