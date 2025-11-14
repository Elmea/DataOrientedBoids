#pragma once

struct Vector2; // From Raymath.h

namespace rendering
{
	void DrawBoid(Vector2 position, Vector2 velocity);

	void DrawObstacle(Vector2 position, float radius);
}