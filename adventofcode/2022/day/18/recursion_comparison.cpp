/*
deltas = {{+1,0,0}, {0,+1,0}, {0,0,+1}};
for(int x{0}; x<N; x++) for(int y{0}; y<N; y++)
    flood(x,y,0);

for(int x{0}; x<N; x++) for(int y{0}; y<N; y++)
{
    flood(x, y,   0, ::p(0,0,+1));
    flood(x, y, N-1, ::p(0,0,-1));

    flood(x, y,   0, ::p(+1,0,0));
    flood(x, y,   0, -::p(0,+1,0));
}
*/

#include <chrono>
#include <thread>

#include "raylib.h"
#include "raymath.h"

int main()
{
    Camera3D camera = {
        .position   = {0, 0, 100},
        .target     = {0, 0, 0},
        .up         = {0, 1, 0},
        .fovy       = 90,
        .projection = CAMERA_PERSPECTIVE,
    };

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Cubes");
    const int CUBE_SIZE = 2;

    size_t iteration = 0;
    while (!WindowShouldClose()) {
        iteration++;

        BeginDrawing();
            ClearBackground(DARKGRAY);
            BeginMode3D(camera);
                Vector3 position = Vector3Zero();
                size_t num_cubes_drawn = 0;
                for (int i = -30; i < 20 and num_cubes_drawn < iteration; i++) {
                    for (int j = -10; j < 20 and num_cubes_drawn < iteration; j++) {
                        for (int k = 0; k < 30 and num_cubes_drawn < iteration; k++) {
                            position.x = CUBE_SIZE * i;
                            position.y = CUBE_SIZE * j;
                            position.z = CUBE_SIZE * k;
                            DrawCube(position, CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, BEIGE);
                            DrawCubeWires(position, CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, DARKBROWN);
                            num_cubes_drawn++;
                        }
                    }
                }
            EndMode3D();
        EndDrawing();
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(std::chrono::duration(0.01s));
    }
    CloseWindow();
}
