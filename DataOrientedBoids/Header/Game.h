#pragma once

#include "Boids.h"
#include "FixObstacle.h"
#include "Systems.h"

class Game
{
private:

    bool windowsSetuped = false;
    Boids boids;
    FixObstacle fixObstacles;

    systems::MovementSystem movementSystem;
    systems::BoidsRenderer boidsRenderer;
    systems::ObstacleRenderer obstacleRenderer;

public:

private:
    void WindowSetup();
    void SystemsSetup();
    void ClearGame();

    void Restart();

    void Close();

    void AddRandomBoids(int);
    void AddRandomObstacle(int);

    void InputsHandling();

    void Update();

    void Render();

public:
    void Setup();

    void Run();
};