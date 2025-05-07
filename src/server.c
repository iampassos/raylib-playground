#include "common.h"
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>

#define COLORS                                                                 \
    (Color[]) {                                                                \
        SKYBLUE, WHITE, YELLOW, BROWN, GRAY, GOLD, PINK, RED, LIME, GREEN,     \
            VIOLET, PURPLE                                                     \
    }

void *server(void *arg) {
    Args *args = (Args *)arg;

    State state = {0};

    socklen_t len = sizeof(*(args->client_addr));

    Client clients[MAX_CLIENTS] = {0};

    while (1) {
        InputPacket input = {0};

        int n = recvfrom(args->socketfd, &input, sizeof(input), 0,
                         (struct sockaddr *)args->client_addr, &len);

        if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            continue;
        }

        int found = 0;
        int j = -1;

        for (int i = 0; i < state.playing; i++) {
            if (state.balls[i].id == input.id) {
                found = 1;
                j = i;
                if (input.end) {
                    state.playing--;
                    clients[i].active = 0;
                    printf("%d has left the game\n", state.balls[i].id);
                } else {
                    state.balls[i].deltaTime = input.deltaTime;
                    clients[i].active = 1;
                }
                break;
            }
        }

        if (!found) {
            if (state.playing >= MAX_CLIENTS) {
                printf("Max players reached\n");
                continue;
            }

            Ball ball = {input.id,
                         COLORS[state.playing],
                         {SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f},
                         {0, 0},
                         {0, 1000},
                         50,
                         0};
            Client client = {input.id, 1, *(args->client_addr)};
            clients[state.playing] = client;
            state.balls[state.playing++] = ball;
            printf("%d has joined the game\n", ball.id);
        }

        for (int i = 0; i < state.playing; i++) {
            Ball *ball = &state.balls[i];
            ball->velocity =
                Vector2Add(ball->velocity,
                           Vector2Scale(ball->acceleration, ball->deltaTime));
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

            ball->position = Vector2Add(
                ball->position, Vector2Scale(ball->velocity, ball->deltaTime));

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

        for (int i = 0; i < state.playing; i++) {
            if (clients[i].active) {
                sendto(args->socketfd, &state, sizeof(state), 0,
                       (struct sockaddr *)&clients[i].sockaddr,
                       sizeof(clients[i].sockaddr));
            }
        }

        // usleep(16600);
    }

    return 0;
}
