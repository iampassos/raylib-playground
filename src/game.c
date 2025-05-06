#include <raylib.h>
#include <raymath.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define ACCELERATION 10
#define MAX_VELOCITY 1000

typedef struct State {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float radius;
} State;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Playground");

    SetTargetFPS(60);

    Rectangle area = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    State state = {
        {SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f}, {0, 0}, {0, 1000}, 50};

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        state.velocity = Vector2Add(
            state.velocity, Vector2Scale(state.acceleration, deltaTime));

        state.velocity = Vector2Scale(state.velocity, 0.99f);

        if (fabs(state.velocity.x) < 1) {
            state.velocity.x = 0;
        }

        if (fabs(state.velocity.y) < 1) {
            state.velocity.y = 0;
        }

        if (state.position.y + state.radius >= area.height - 5) {
            state.velocity.y *= -1;
        }

        if (state.position.y - state.radius < area.y + 5) {
            state.velocity.y *= -1;
        }

        if (state.position.x + state.radius >= area.width - 5) {
            state.velocity.x *= -1;
        }

        if (state.position.x - state.radius < area.x + 5) {
            state.velocity.x *= -1;
        }

        state.position =
            Vector2Add(state.position, Vector2Scale(state.velocity, deltaTime));

        if (IsKeyDown(KEY_SPACE) &&
            state.position.y + state.radius >= area.height - 5) {
            state.velocity.y += MAX_VELOCITY;
        }

        if (IsKeyDown(KEY_A)) {
            state.velocity.x -= ACCELERATION;
        }
        if (IsKeyDown(KEY_D)) {
            state.velocity.x += ACCELERATION;
        }

        if (state.velocity.y > MAX_VELOCITY) {
            state.velocity.y = MAX_VELOCITY;
        }

        if (state.velocity.y < -MAX_VELOCITY) {
            state.velocity.y = -MAX_VELOCITY;
        }

        if (state.velocity.x > MAX_VELOCITY) {
            state.velocity.x = MAX_VELOCITY;
        }

        if (state.velocity.x < -MAX_VELOCITY) {
            state.velocity.x = -MAX_VELOCITY;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawCircleV(state.position, state.radius, SKYBLUE);

        DrawRectangleLinesEx(area, 5, DARKBLUE);

        DrawText(TextFormat("Position: (%.2f, %.2f)", state.position.x,
                            state.position.y),
                 10, 10, 20, WHITE);

        DrawText(TextFormat("Velocity: (%.2f, %.2f)", state.velocity.x,
                            state.velocity.y),
                 10, 35, 20, WHITE);

        DrawText(TextFormat("Acceleration: (%.2f, %.2f)", state.acceleration.x,
                            state.acceleration.y),
                 10, 60, 20, WHITE);

        EndDrawing();

        area.height = GetScreenHeight();
        area.width = GetScreenWidth();
    }

    CloseWindow();

    return 0;
}
