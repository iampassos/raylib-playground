#include "common.h"
#include <raylib.h>
#include <time.h>

#define ACCELERATION 10
#define MAX_VELOCITY 1000

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: client <ip>\n");
        return 1;
    }

    srand(time(NULL));
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    socklen_t len = sizeof(server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Playground");
    SetTargetFPS(60);

    Rectangle area = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    State state = {0};

    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangleLinesEx(area, 5, DARKBLUE);
    EndDrawing();

    int id = rand();

    while (!WindowShouldClose()) {
        // INPUT

        InputPacket packet = {id,
                              IsKeyDown(KEY_SPACE),
                              IsKeyDown(KEY_A),
                              IsKeyDown(KEY_D),
                              GetFrameTime(),
                              0};

        // SEND INPUT

        sendto(socketfd, &packet, sizeof(packet), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));

        // RECEIVE STATE

        recvfrom(socketfd, &state, sizeof(state), 0,
                 (struct sockaddr *)&server_addr, &len);

        // DRAW

        Ball ball;
        for (int i = 0; i < state.playing; i++) {
            if (state.balls[i].id == id) {
                ball = state.balls[i];
                break;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        for (int i = 0; i < state.playing; i++) {
            if (state.balls[i].active) {
                DrawCircleV(state.balls[i].position, state.balls[i].radius,
                            state.balls[i].color);
            }
        }

        DrawRectangleLinesEx(area, 5, DARKBLUE);

        DrawText(TextFormat("Position: (%.2f, %.2f)", ball.position.x,
                            ball.position.y),
                 10, 10, 20, WHITE);

        DrawText(TextFormat("Velocity: (%.2f, %.2f)", ball.velocity.x,
                            ball.velocity.y),
                 10, 35, 20, WHITE);

        DrawText(TextFormat("Acceleration: (%.2f, %.2f)", ball.acceleration.x,
                            ball.acceleration.y),
                 10, 60, 20, WHITE);

        EndDrawing();
    }

    InputPacket packet = {id, 0, 0, 0, 0, 1};

    sendto(socketfd, &packet, sizeof(packet), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    CloseWindow();

    return 0;
}
