#pragma once

#include <bitset>

#define MAX_ENTITIES 5000
namespace Dot {

	using Entity = unsigned int;
	using ComponentType = std::uint8_t;
	using Signature = std::bitset<MAX_ENTITIES>;
}
