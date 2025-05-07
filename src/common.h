#include <arpa/inet.h>
#include <netinet/in.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PORT 8000
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

#define ACCELERATION 10
#define MAX_VELOCITY 1000

typedef struct Ball {
    int id;
    int active;
    Color color;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float radius;
    float deltaTime;
} Ball;

typedef struct State {
    Ball balls[12];
    int playing;
} State;

typedef struct InputPacket {
    int id;
    int backspace;
    int a;
    int d;
    float deltaTime;
    int end;
} InputPacket;
