#pragma once

#include <thread>

#include "parameters.h"
#include "components.h"

namespace systems
{
	struct MovementSystem
	{
		std::vector<std::unique_ptr<std::thread>> threads;
		const int coreCount = std::thread::hardware_concurrency() / 4;
		const int boidPerCore = BOIDS_SPAWN / coreCount;
		std::atomic<bool>* updateThreadSignals;
		std::atomic<int> threadCompleted;
		std::atomic<bool> freeThreads;

		Vector2 AvoidObstacle(int index, components::PositionComponent*, components::VelocityComponent*,
						const components::PositionComponent&, const components::RadiusComponent&, Vector2 avoidVector);

		Vector2 AvoidBoids(int index, components::PositionComponent*, components::VelocityComponent*, Vector2 avoidVector);

		void UpdateMovements();

		void InitMovementThreads(components::PositionComponent*, components::VelocityComponent*,
			components::PositionComponent*, components::RadiusComponent*);

		void UpdateBoidThread(components::PositionComponent*, components::VelocityComponent*,
			components::PositionComponent*, components::RadiusComponent*, int id);

		void Destroy();
	};

	struct BoidsRenderer
	{

		void RenderBoids(const components::PositionComponent&, const components::VelocityComponent&);
	};

	struct ObstacleRenderer
	{
		void RenderObstacle(const components::PositionComponent&, const components::RadiusComponent&);
	};
}