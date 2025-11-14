#pragma once

#include <vector>
#include <atomic>

struct Vector2; // From Raymath.h

namespace components
{
	struct PositionComponent { std::vector<Vector2> data; };
	struct VelocityComponent { std::vector<Vector2> data; };
	struct SpeedComponent { std::vector<float> data; };
	struct RadiusComponent { std::vector<float> data; };
}