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
    void WindowSetup();
    void SystemsSetup();
    void ClearGame();

    void Restart();

    void Close();

    void AddMouseToObstacle();
    void AddRandomBoids(int);
    void AddRandomObstacle(int);

    void InputsHandling();

    void UpdateMouseObstacle();
    void Update();

    void Render();
    void CacheVideo();

public:
    void Setup();

    void Run();
};