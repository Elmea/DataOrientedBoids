#pragma once

#include "components.h"
#include "raylib.h"
#include "raymath.h"

struct Boids
{
	int boidsCount = 0;

	components::PositionComponent positionsComponent;
	components::VelocityComponent velocityComponent;

	inline void AddBoid(Vector2 position, Vector2 velocity)
	{
		boidsCount++;

		positionsComponent.data.push_back(position);
		velocityComponent.data.push_back(velocity);
	}

	inline void Clear()
	{
		boidsCount = 0;

		positionsComponent.data.clear();
		velocityComponent.data.clear();
	}
};