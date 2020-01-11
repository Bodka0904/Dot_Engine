#pragma once

#include <bitset>

#define MAX_ENTITIES 5000
#define MAX_SHADERS 300
#define MAX_COMPONENTS 32

namespace Dot {

	using Entity = unsigned int;
	using ComponentType = std::uint8_t;
	using Signature = std::bitset<MAX_COMPONENTS>;
	using ShaderID = unsigned int;
}
