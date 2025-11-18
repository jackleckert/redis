/*
CLIENT
- connect() create connection socket to server
- write() message to server
- read() message from the server
- close() socket
*/


#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>


static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234); // port
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("connect");
    }

    const size_t BUFFER_SIZE = 4096;  // Much larger buffer size
    char *msg = (char *)malloc(BUFFER_SIZE);
    char *rbuf = (char *)malloc(BUFFER_SIZE);
    
    if (msg == NULL || rbuf == NULL) {
        die("malloc");
    }

    printf("Connected to the server. Type your messages below (type 'exit' to quit):\n");

    while (true) {
        // Get user input
        printf("You: ");
        if (fgets(msg, BUFFER_SIZE, stdin) == NULL) {
            die("fgets");
        }

        // Remove the newline character if present
        size_t len = strlen(msg);
        if (len > 0 && msg[len - 1] == '\n') {
            msg[len - 1] = '\0';
        }

        // Exit condition
        if (strcmp(msg, "exit") == 0) {
            printf("Closing connection...\n");
            break;
        }

        // Send message to the server: send()
        write(fd, msg, strlen(msg));

        // Read server response: recv()
        memset(rbuf, 0, BUFFER_SIZE);
        ssize_t n = read(fd, rbuf, BUFFER_SIZE - 1);
        if (n < 0) {
            die("read");
        }
        printf("Server: %s\n", rbuf);
    }

    free(msg);
    free(rbuf);
    close(fd);
    return 0;
}