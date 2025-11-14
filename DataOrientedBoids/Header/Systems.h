#pragma once

#include "components.h"

namespace systems
{
	struct MovementSystem
	{
		Vector2 AvoidObstacle(int index, components::PositionComponent*, components::VelocityComponent*,
						const components::PositionComponent&, const components::RadiusComponent&, Vector2 avoidVector);

		Vector2 AvoidBoids(int index, components::PositionComponent*, components::VelocityComponent*, Vector2 avoidVector);

		void UpdateMovements(components::PositionComponent*, components::VelocityComponent*,
							const components::PositionComponent&, const components::RadiusComponent&);
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