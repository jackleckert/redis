/*
SERVER
- listen() creating the socket

- accept() client connection
- read() client message
- write() response to client
- close() listen socket
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


static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

static void do_something(int connfd) {
    const size_t BUFFER_SIZE = 4096;  // Much larger buffer size
    char *rbuf = (char *)malloc(BUFFER_SIZE);
    char *wbuf = (char *)malloc(BUFFER_SIZE);
    
    if (rbuf == NULL || wbuf == NULL) {
        msg("malloc() error");
        if (rbuf) free(rbuf);
        if (wbuf) free(wbuf);
        return;
    }

    while (true) {
        // Read client message
        memset(rbuf, 0, BUFFER_SIZE);
        ssize_t n = read(connfd, rbuf, BUFFER_SIZE - 1);
        if (n <= 0) {
            msg("Client disconnected or read() error");
            break;
        }
        fprintf(stderr, "Client: %s\n", rbuf);

        // Respond to the client
        printf("You (server): ");
        if (fgets(wbuf, BUFFER_SIZE, stdin) == NULL) {
            msg("fgets() error");
            break;
        }

        // Remove the newline character if present
        size_t len = strlen(wbuf);
        if (len > 0 && wbuf[len - 1] == '\n') {
            wbuf[len - 1] = '\0';
        }
        
        // send message
        write(connfd, wbuf, strlen(wbuf));
    }
    
    free(rbuf);
    free(wbuf);
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    // set socket options. this is needed for most server applications
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234); // port
    addr.sin_addr.s_addr = ntohl(0);    // wildcard address 0.0.0.0
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }

    // listen
    rv = listen(fd, SOMAXCONN);
    if (rv) {
        die("listen()");
    }

    printf("Server is running and waiting for connections...\n");

    while (true) {
        // accept
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        // accept
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if (connfd < 0) {
            continue;   // error
        }

        printf("Client connected. Waiting for messages...\n");
        do_something(connfd); //
        close(connfd); // close
        printf("Client disconnected.\n");
    }

    return 0;
}