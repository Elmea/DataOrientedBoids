#pragma once

#include "components.h"
#include "raylib.h"
#include "raymath.h"

struct FixObstacle
{
	components::PositionComponent fixObstaclePositions;
	components::RadiusComponent fixObstacleRadius;

	inline void AddFixObstacle(Vector2 position, float radius)
	{
		fixObstaclePositions.data.push_back(position);
		fixObstacleRadius.data.push_back(radius);
	}

	inline void Clear()
	{
		fixObstaclePositions.data.clear();
		fixObstacleRadius.data.clear();
	}
};