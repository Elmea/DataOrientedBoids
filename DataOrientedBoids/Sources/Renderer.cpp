#include "Renderer.h"

#include "parameters.h"
#include "raylib.h"
#include "raymath.h"

void rendering::DrawBoid(Vector2 position, Vector2 velocity)
{
	Vector2 dir = Vector2Normalize(velocity);
	float rotation = (float)(atan2(dir.y, dir.x));

	Vector2 v1 = Vector2Rotate({ 1.0f, 0.0f },		rotation) * BOIDS_SIZE;
	Vector2 v2 = Vector2Rotate({ -0.5f, -0.35f },	rotation) * BOIDS_SIZE;
	Vector2 v3 = Vector2Rotate({ -0.5f, 0.35f },	rotation) * BOIDS_SIZE;

	float speed = Vector2Length(velocity);
	Color color = ColorLerp(BOIDS_MINSPEEDCOLOR, BOIDS_MAXSPEEDCOLOR, (speed - BOIDS_MINSPEED) / (BOIDS_MAXSPEED - BOIDS_MINSPEED));

	DrawTriangleLines(v1 + position, v2 + position, v3 + position, color);
}

void rendering::DrawBoid(Vector2 position, Vector2 velocity, cv::Mat frame, int vidWitdh, int vidHeight)
{
	Vector2 dir = Vector2Normalize(velocity);
	float rotation = (float)(atan2(dir.y, dir.x));

	Vector2 v1 = Vector2Rotate({ 1.0f, 0.0f }, rotation) * BOIDS_SIZE;
	Vector2 v2 = Vector2Rotate({ -0.5f, -0.35f }, rotation) * BOIDS_SIZE;
	Vector2 v3 = Vector2Rotate({ -0.5f, 0.35f }, rotation) * BOIDS_SIZE;

	Color color;
	float vidPosX = position.x / SCREEN_WIDTH * vidWitdh;
	float vidPosY = position.y / SCREEN_HEIGHT * vidHeight;
	if (vidPosX > 0 && vidPosX < vidWitdh && vidPosY > 0 && vidPosY < vidHeight)
	{
		cv::Vec3b col = frame.at<cv::Vec3b>(cv::Point(vidPosX, vidPosY));
		color = { col[2], col[1], col[0], 255 };
	}
	else
	{
		color = { 30, 30, 30, 255 };
	}

	DrawTriangle(v1 + position, v2 + position, v3 + position, color);
}

void rendering::DrawObstacle(Vector2 position, float radius)
{
	DrawCircleV(position, radius, DARKGREEN);
}