#pragma once

namespace constants::UBO
{
	inline constexpr unsigned int MATRICES_UNIFORM_BLOCK_INDEX = 0;
	inline constexpr unsigned int LIGHTS_UNIFORM_BLOCK_INDEX = 1;
	inline constexpr unsigned int VIEW_UNIFORM_BLOCK_INDEX = 2;
}

namespace constants::light
{
	inline constexpr unsigned int MAX_DIRECTIONAL_LIGHTS = 2;
	inline constexpr unsigned int MAX_SPOT_LIGHTS = 4;
	inline constexpr unsigned int MAX_POINT_LIGHTS = 8;
}
