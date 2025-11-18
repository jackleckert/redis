# redis
Building redis from scratch using C/C++.

## Introduction

Computer networking protocols have different layers: Application layer (Application protocols (e.g. HTTP)) > Transport Layer (TCP/UDP) > Internet Layer (IP and port) > Network Access Layer (Ethernet). Typical apps handle messages, but TCP/UDP provide byte streams. TCP is used when data must be correct and complete. UDP is faster but does not guarantee delivery of useful information.
![TCP/IP Diagram](tcp_ip_diagram.png)

Redis, HTTP/1.1, and most RPC protocols are request-response protocols. Each request message is paired with a response message. If the messages are not both reliable & ordered, there will be troubles pairing a response with a request. Thus, most request-response protocols are based on TCP.


## What is a socket?

A socket is a handle to refer to a connection. The API for networking is called the socket API, which is similar on different operating systems. A handle is an opaque integer used to refer to things that cross an API boundary. On Linux, a handle is called a file descriptor (fd) and it’s local to the process. The name “file descriptor” is just a name; it has nothing to do with files, nor does it describe anything.

Listening is telling the OS that an app is ready to accept TCP connections from a given port. The OS then returns a socket handle for apps to refer to that port. From the listening socket, apps can retrieve (accept) incoming TCP connections, which is also represented as a socket handle. So there are 2 types of handles: listening socket & connection socket.


Listening TCP socket:
- `bind()` & `listen()`
- `accept()`
- `close()`

Using a TCP socket:
- `read()`
- `write()`
- `close()`

Create a TCP connection: `connect()`


## Build and install

To build and and use the TCP server and client, use the following commands:

```
g++ -Wall -Wextra -O2 -g src/client.cpp -o client
g++ -Wall -Wextra -O2 -g src/server.cpp -o server
```

To run a session, run the `./server` in a first terminal, and then the `./client` in a second terminal.

