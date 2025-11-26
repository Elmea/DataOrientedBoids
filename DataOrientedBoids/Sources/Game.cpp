#include <random>

#include "parameters.h"
#include "raylib.h"
#include "Game.h"

#pragma region App
void Game::WindowSetup()
{
#if TRANSPARENTWINDOW
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
#endif
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Data oriented boids");
    SetWindowState(FLAG_WINDOW_UNDECORATED);

    SetTargetFPS(TARGETFPS);
}

void Game::SystemsSetup()
{
    movementSystem.InitMovementThreads(&boids.positionsComponent, &boids.velocityComponent,
        &fixObstacles.fixObstaclePositions, &fixObstacles.fixObstacleRadius);
}

void Game::Setup()
{
#if READ_VID
    CacheVideo();
#endif
    AddMouseToObstacle();

    AddRandomBoids(BOIDS_SPAWN);
    AddRandomObstacle(OBSTACLE_STARTCOUNT);

    SystemsSetup();

    if (!windowsSetuped)
    {
        WindowSetup();
        windowsSetuped = true;
    }
}

void Game::Close()
{
    movementSystem.Destroy();
    CloseWindow();
}

void Game::Run()
{
    while (!WindowShouldClose())
    {
        Update();
        Render();
    }

    Close();
}

void Game::CacheVideo()
{
    cv::VideoCapture cap(VIDEOPATH);

    if (!cap.isOpened()) {
        std::cout << "Error: Could not open video file." << std::endl;
        return;
    }
    else {
        std::cout << "Video file opened successfully!" << std::endl;
    }

    bool ret = true;
    while(ret)
    {
        cv::Mat frame;
        ret = cap.read(frame);

        if (!ret)
            break;

        frames.push_back(frame.clone());
    }

    vidFPS = cap.get(cv::CAP_PROP_FPS);
    vidHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    vidWitdh = cap.get(cv::CAP_PROP_FRAME_WIDTH);

    cap.release();
}
#pragma endregion

void Game::Restart()
{
    ClearGame();
    Setup();
}

void Game::ClearGame()
{
    boids.Clear();
    fixObstacles.Clear();
}

void Game::AddMouseToObstacle()
{
    mouseObsID = fixObstacles.fixObstaclePositions.data.size();

    fixObstacles.fixObstaclePositions.data.push_back({ 0, 0 });
    fixObstacles.fixObstacleRadius.data.push_back(MOUSE_OBSTACLE_RADIUS);
}

void Game::UpdateMouseObstacle()
{
    if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_RIGHT))
        fixObstacles.fixObstaclePositions.data[mouseObsID] = GetMousePosition();
    else
        fixObstacles.fixObstaclePositions.data[mouseObsID] = { -100000, -100000 };
}

void Game::AddRandomBoids(int count)
{
    for (int i = 0; i < count; i++)
    {
        float randX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * SCREEN_WIDTH;
        float randY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * SCREEN_HEIGHT;
        Vector2 randDir = Vector2Normalize({ static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f,
                                            static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f });

        float randSpeed = Lerp(BOIDS_MINSPEED, BOIDS_MAXSPEED,static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

        boids.AddBoid({ randX, randY }, randDir * randSpeed);
    }
}

void Game::AddRandomObstacle(int count)
{
    for (int i = 0; i < count; i++)
    {
        float randX = Clamp(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), OBSATCLE_BORDER_MINMULT, OBSATCLE_BORDER_MAXMULT) * SCREEN_WIDTH;
        float randY = Clamp(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), OBSATCLE_BORDER_MINMULT, OBSATCLE_BORDER_MAXMULT)* SCREEN_HEIGHT;

        float randRadius = Lerp(OBSTACLE_MINIMUMRADIUS, OBSTACLE_MAXIMUMRADIUS, static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

        fixObstacles.AddFixObstacle({ randX, randY }, randRadius);
    }
}

void Game::InputsHandling()
{
    if (IsKeyPressed(KeyboardKey::KEY_R))
        Restart();

    if (IsKeyPressed(KeyboardKey::KEY_O))
        fixObstacles.Clear();

    if (IsKeyPressed(KeyboardKey::KEY_P))
    {
        fixObstacles.Clear();
        AddMouseToObstacle();
        AddRandomObstacle(OBSTACLE_RANDSPAWN);
    }

    if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
        fixObstacles.AddFixObstacle(GetMousePosition(), OBSTACLE_PLAYERRADIUS);

    if (IsKeyPressed(KeyboardKey::KEY_B))
        drawTransparent = !drawTransparent;

}

void Game::Update()
{
    InputsHandling();
    UpdateMouseObstacle();
    movementSystem.UpdateMovements();
}

void Game::Render()
{
    BeginDrawing();

    if (drawTransparent)
        ClearBackground(BLANK);
    else 
        ClearBackground(BLACK);

    obstacleRenderer.RenderObstacle(fixObstacles.fixObstaclePositions, fixObstacles.fixObstacleRadius);

#if !READ_VID
    boidsRenderer.RenderBoids(boids.positionsComponent, boids.velocityComponent);
#else

    boidsRenderer.RenderBoidsVideo(boids.positionsComponent, boids.velocityComponent,                                    frames[currFrameId], vidWitdh, vidHeight);
    
    timeSinceLastVidFrame += GetFrameTime();
    if (timeSinceLastVidFrame > 1.0f / vidFPS)
    {
        timeSinceLastVidFrame = 0;
        currFrameId++;
        if (currFrameId >= frames.size())
            currFrameId = 0;
    }

    // DrawText(TextFormat("Vid frame: %i", currFrameId), 10, 60, 10, WHITE);
#endif

    DrawText(TextFormat("FPS: %i", GetFPS()), 10, 20, 20, GREEN);
    DrawText(TextFormat("Boids count: %i", BOIDS_SPAWN), 10, 40, 10, WHITE);

    EndDrawing();
}
