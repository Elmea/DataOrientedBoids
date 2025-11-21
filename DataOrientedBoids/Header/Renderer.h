#pragma once

#include "opencv2/opencv.hpp"

struct Vector2; // From Raymath.h

namespace rendering
{
	void DrawBoid(Vector2 position, Vector2 velocity);

	void DrawBoid(Vector2 position, Vector2 velocity, cv::Mat frame, int vidWitdh, int vidHeight);

	void DrawObstacle(Vector2 position, float radius);
}