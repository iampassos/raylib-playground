#include "common.h"
#include <unistd.h>

#define COLORS                                                                 \
    (Color[]) {                                                                \
        SKYBLUE, WHITE, YELLOW, BROWN, GRAY, GOLD, PINK, RED, LIME, GREEN,     \
            VIOLET, PURPLE                                                     \
    }

int main() {
    State state = {0};

    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketfd < 0) {
        printf("Error creating socket");
        exit(1);
    }

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    int result =
        bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (result < 0) {
        printf("Error binding to socket");
        exit(1);
    }

    printf("Listening on port %d\n", PORT);

    socklen_t len = sizeof(client_addr);

    while (1) {
        // RECEIVE INPUT

        InputPacket input = {0};

        int n = recvfrom(socketfd, &input, sizeof(input), 0,
                         (struct sockaddr *)&client_addr, &len);

        if (n < 0) {
            printf("Error receiving message from client");
            exit(1);
        }

        int found = 0;
        int j = -1;
        for (int i = 0; i < state.playing; i++) {
            if (state.balls[i].id == input.id) {
                found = 1;
                j = i;
                if (input.end) {
                    state.playing--;
                    break;
                }
                state.balls[i].deltaTime = input.deltaTime;
                state.balls[i].active -= input.end;
                break;
            }
        }

        if (found == 0) {
            Ball ball = {input.id,
                         1,
                         COLORS[state.playing],
                         {SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f},
                         {0, 0},
                         {0, 1000},
                         50,
                         input.deltaTime};
            state.balls[state.playing++] = ball;
        }

        // UPDATE

        for (int i = 0; i < state.playing; i++) {
            Ball *ball = &state.balls[i];
            if (ball->active) {
                ball->velocity =
                    Vector2Add(ball->velocity, Vector2Scale(ball->acceleration,
                                                            ball->deltaTime));

                ball->velocity = Vector2Scale(ball->velocity, 0.99f);

                if (fabs(ball->velocity.x) < 1) {
                    ball->velocity.x = 0;
                }

                if (fabs(ball->velocity.y) < 1) {
                    ball->velocity.y = 0;
                }

                if (ball->position.y + ball->radius >= SCREEN_HEIGHT - 5) {
                    ball->velocity.y *= -1;
                }

                if (ball->position.y - ball->radius < 5) {
                    ball->velocity.y *= -1;
                }

                if (ball->position.x + ball->radius >= SCREEN_WIDTH - 5) {
                    ball->velocity.x *= -1;
                }

                if (ball->position.x - ball->radius < 5) {
                    ball->velocity.x *= -1;
                }

                ball->position =
                    Vector2Add(ball->position,
                               Vector2Scale(ball->velocity, ball->deltaTime));

                if (j > -1 && j == i) {
                    if (input.backspace &&
                        ball->position.y + ball->radius >= SCREEN_HEIGHT - 5) {
                        ball->velocity.y += MAX_VELOCITY;
                    }

                    if (input.a) {
                        ball->velocity.x -= ACCELERATION;
                    }
                    if (input.d) {
                        ball->velocity.x += ACCELERATION;
                    }
                }

                if (ball->velocity.y > MAX_VELOCITY) {
                    ball->velocity.y = MAX_VELOCITY;
                }

                if (ball->velocity.y < -MAX_VELOCITY) {
                    ball->velocity.y = -MAX_VELOCITY;
                }

                if (ball->velocity.x > MAX_VELOCITY) {
                    ball->velocity.x = MAX_VELOCITY;
                }

                if (ball->velocity.x < -MAX_VELOCITY) {
                    ball->velocity.x = -MAX_VELOCITY;
                }
            }
        }

        // SEND STATE

        sendto(socketfd, &state, sizeof(state), 0,
               (struct sockaddr *)&client_addr, len);
    }

    return 0;
}
