#include <random>

#include "parameters.h"
#include "raylib.h"
#include "Game.h"

#pragma region App
void Game::WindowSetup()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Data oriented boids");
    SetTargetFPS(TARGETFPS);
}

void Game::SystemsSetup()
{

}

void Game::Setup()
{
    SystemsSetup();

    if (!windowsSetuped)
    {
        WindowSetup();
        windowsSetuped = true;
    }

    AddRandomBoids(BOIDS_SPAWN);
    AddRandomObstacle(OBSTACLE_STARTCOUNT);
}

void Game::Close()
{
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
        AddRandomObstacle(OBSTACLE_STARTCOUNT);
    }

    if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
        fixObstacles.AddFixObstacle(GetMousePosition(), OBSTACLE_PLAYERRADIUS);
}

void Game::Update()
{
    InputsHandling();

    movementSystem.UpdateMovements(&boids.positionsComponent, &boids.velocityComponent,
                        fixObstacles.fixObstaclePositions, fixObstacles.fixObstacleRadius);
}

void Game::Render()
{
    BeginDrawing();

    ClearBackground(BLACK);

    obstacleRenderer.RenderObstacle(fixObstacles.fixObstaclePositions, fixObstacles.fixObstacleRadius);
    boidsRenderer.RenderBoids(boids.positionsComponent, boids.velocityComponent);

    DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, GREEN);

    EndDrawing();
}
