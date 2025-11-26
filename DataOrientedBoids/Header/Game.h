#pragma once

#include "opencv2/opencv.hpp"

#include "parameters.h"
#include "Boids.h"
#include "FixObstacle.h"
#include "Systems.h"

class Game
{
private:

    bool windowsSetuped = false;
    bool drawTransparent = true;
    Boids boids;
    FixObstacle fixObstacles;

    systems::MovementSystem movementSystem;
    systems::BoidsRenderer boidsRenderer;
    systems::ObstacleRenderer obstacleRenderer;

    std::vector<cv::Mat> frames;
    int currFrameId = 0;
    int vidWitdh, vidHeight, vidFPS;
    float timeSinceLastVidFrame = 0;
    int mouseObsID;

public:

private:
#pragma region App
    void WindowSetup();
    void SystemsSetup();
    void CacheVideo();
    void Setup();
    void Close();
#pragma endregion
    
#pragma region Utilities
    void ClearGame();
    void Restart();

    void AddMouseToObstacle();
    void AddRandomBoids(int);
    void AddRandomObstacle(int);
#pragma endregion

    void InputsHandling();

    void UpdateMouseObstacle();
    void Update();
    void Render();

public:

    void Run();
};