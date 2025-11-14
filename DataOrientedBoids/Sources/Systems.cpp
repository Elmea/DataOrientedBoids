#include <thread>
#include <atomic>
#include "Systems.h"

#include "parameters.h"

#include "components.h"

#include "raylib.h"
#include "raymath.h"
#include "Renderer.h"

using namespace components;

namespace systems
{
	Vector2 MovementSystem::AvoidObstacle(int idx, components::PositionComponent* positionComp, components::VelocityComponent* velocityComp,
		const components::PositionComponent& fixObstaclesPos, const components::RadiusComponent& fixObstaclesRadius, Vector2 avoidVector)
	{
		Vector2 avgPos{ 0,0 };
		Vector2 avgVelocity{ 0,0 };

		for (int j = 0; j < fixObstaclesPos.data.size(); j++)
		{
			if (j == idx)
				continue;

			float squaredDist = Vector2Distance(positionComp->data[idx], fixObstaclesPos.data[j]) - (fixObstaclesRadius.data[j]);


			if (squaredDist < BOIDS_VISUALRANGE)
			{
				if (squaredDist < BOIDS_PROTECTEDRANGE)
					avoidVector += (positionComp->data[idx] - fixObstaclesPos.data[j]) * BOIDS_AVOIDFACTOR_OBS;
				else 
					avoidVector += (positionComp->data[idx] - fixObstaclesPos.data[j]) * BOIDS_AVOIDFACTOR_OBS * 0.15f;
			}
		}

		return avoidVector;
	}

	Vector2 MovementSystem::AvoidBoids(int idx, components::PositionComponent* positionComp, 
										components::VelocityComponent* velocityComp, Vector2 avoidVector)
	{
		Vector2 avgPos{ 0,0 };
		Vector2 avgVelocity{ 0,0 };
		int neighbor = 0;

		for (int j = 0; j < positionComp->data.size(); j++)
		{
			if (j == idx)
				continue;
			
			float squaredDist = Vector2DistanceSqr(positionComp->data[idx], positionComp->data[j]);

			if (squaredDist < BOIDS_VISUALRANGE_SQR)
			{
				if (squaredDist < BOIDS_PROTECTEDRANGE_SQR)
				{
					avoidVector += (positionComp->data[idx] - positionComp->data[j]) * BOIDS_AVOIDFACTOR;
				}
				else
				{
					avgPos += positionComp->data[j];
					avgVelocity += velocityComp->data[j];
					neighbor++;
				}
			}
		}

		if (neighbor > 0)
		{
			avgPos = avgPos / (float)neighbor;
			avgVelocity = avgVelocity / (float)neighbor;

			velocityComp->data[idx] += (avgPos - positionComp->data[idx]) * BOIDS_CENTERINGFACTOR +
				(avgVelocity - velocityComp->data[idx]) * BOIDS_MATCHINGFACTOR;
		}

		return avoidVector;
	}

	void MovementSystem::UpdateMovements(PositionComponent* positionComp, VelocityComponent* velocityComp,
									const components::PositionComponent& fixObstaclesPos, const components::RadiusComponent& fixObstaclesRadius)
	{
		for (int idx = 0; idx < positionComp->data.size(); idx++)
		{
			Vector2* myPos = &positionComp->data[idx];
			Vector2* myVel = &velocityComp->data[idx];

			Vector2 boidAvoidVector = AvoidBoids(idx, positionComp, velocityComp, { 0,0 });
			Vector2 ObstacleAvoidVector = AvoidObstacle(idx, positionComp, velocityComp, fixObstaclesPos, fixObstaclesRadius, { 0,0 });

			*myVel += (boidAvoidVector * BOIDS_AVOIDFACTOR);
			*myVel += (ObstacleAvoidVector * BOIDS_AVOIDFACTOR_OBS);

			#if BOIDS_AVOID_LEFTRIGHT
			if (myPos->x + BOIDS_NEAR_EDGE > SCREEN_WIDTH)
				myVel->x -= BOIDS_OUTBOUNDS_TURNFAC;
			else if (myPos->x - BOIDS_NEAR_EDGE < 0)
				myVel->x += BOIDS_OUTBOUNDS_TURNFAC;
			#endif

			#if BOIDS_AVOID_TOPBOTTOM
			if (myPos->y + BOIDS_NEAR_EDGE > SCREEN_HEIGHT)
				myVel->y -= BOIDS_OUTBOUNDS_TURNFAC;
			else if (myPos->y - BOIDS_NEAR_EDGE < 0)
				myVel->y += BOIDS_OUTBOUNDS_TURNFAC;
			#endif

			float speed = Vector2Length(*myVel);
			if (speed > BOIDS_MAXSPEED)
			{
				velocityComp->data[idx].x = (velocityComp->data[idx].x / speed) * BOIDS_MINSPEED;
				velocityComp->data[idx].y = (velocityComp->data[idx].y / speed) * BOIDS_MINSPEED;
			}
			else if (speed < BOIDS_MINSPEED)
			{
				myVel->x = (myVel->x / speed) * BOIDS_MINSPEED;
				myVel->y = (myVel->y / speed) * BOIDS_MINSPEED;
			}

			Vector2 dir = Vector2Normalize(*myVel);
			float rotation = (float)(atan2(dir.y, dir.x));

			*myPos += *myVel * GetFrameTime();

			#if BOIDS_ALWAYSINBOUNDS
			if (myPos->x > SCREEN_WIDTH)
				myPos->x = 0;
			else if (myPos->x < 0)
				myPos->x = SCREEN_WIDTH;

			if (myPos->y > SCREEN_HEIGHT)
				myPos->y = 0;
			else if (myPos->y < 0)
				myPos->y = SCREEN_HEIGHT;
			#endif
		}
	}

	void BoidsRenderer::RenderBoids(const PositionComponent& positionComp, const VelocityComponent& velocityComp)
	{
		for (int idx = 0; idx < positionComp.data.size(); idx++)
			rendering::DrawBoid(positionComp.data[idx], velocityComp.data[idx]);
	}

	void ObstacleRenderer::RenderObstacle(const components::PositionComponent& pos, const components::RadiusComponent& r)
	{
		for (int idx = 0; idx < pos.data.size(); idx++)
			rendering::DrawObstacle(pos.data[idx], r.data[idx]);
	}
}