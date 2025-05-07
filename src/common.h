#ifndef COMMON_H
#define COMMON_H

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
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

#define MAX_CLIENTS 12

typedef struct Ball {
    int id;
    Color color;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float radius;
    float deltaTime;
} Ball;

typedef struct Client {
    int id;
    int active;
    struct sockaddr_in sockaddr;
} Client;

typedef struct State {
    Ball balls[MAX_CLIENTS];
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

typedef struct Args {
    int socketfd;
    struct sockaddr_in *server_addr;
    struct sockaddr_in *client_addr;
} Args;

#endif
