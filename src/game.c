#include "client.c"
#include "server.c"
#include <pthread.h>

int main(int argc, char *argv[]) {
    if (argc == 2) {
        client(argv[1]);
    } else {
        int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
        fcntl(socketfd, F_SETFL, O_NONBLOCK);

        if (socketfd < 0) {
            printf("Error creating socket");
            return 1;
        }

        struct sockaddr_in server_addr, client_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        int result = bind(socketfd, (struct sockaddr *)&server_addr,
                          sizeof(server_addr));

        if (result < 0) {
            client("127.0.0.1");
            return 1;
        }

        printf("Listening on port %d\n", PORT);

        pthread_t thread;

        Args args = {socketfd, &server_addr, &client_addr};

        pthread_create(&thread, NULL, &server, &args);

        client("127.0.0.1");
    }
}
