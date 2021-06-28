#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position pos;
    int width;
} Sand;

void updateSand(Sand* sand) {
    sand->pos.y++;
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    Sand particles[50];
    for (int i = 0; i < 50; i++) {
        particles[i].pos.x = 5*i + 200;
        particles[i].pos.y = 300;
        particles[i].width = 5;
    }

    InitWindow(screenWidth, screenHeight, "Test Window");
    SetTargetFPS(60);


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Yo", 190, 200, 200, LIGHTGRAY);

        for (int i = 0; i < 50; i++) {
            updateSand(&particles[i]);
            DrawRectangle(particles[i].pos.x, particles[i].pos.y, particles[i].width, particles[i].width, YELLOW);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
