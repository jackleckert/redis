# Redis Clone

This project is a simplified clone of the Redis server, implemented in C++. It provides a basic key-value store with support for strings, sorted sets (zsets), and time-to-live (TTL) expiration for keys.


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

## Project Structure

The repository is organized into the following directories and key files:

-   `src/`: Contains the C++ source code.
    -   `server.cpp`: The main server logic, including networking, command parsing, and data handling.
    -   `client.cpp`: A simple command-line client to interact with the server.
    -   `hashtable.cpp`, `avl.cpp`, `zset.cpp`, `heap.cpp`, `list.cpp`: Implementations of the core data structures.
    -   `thread_pool.cpp`: A basic thread pool for handling concurrent tasks.
-   `include/`: Contains the header files for the data structures and common utilities.
-   `Makefile`: The build script for compiling the server and client.
-   `README.md`: This file.

## Building the Project

You can build the server and client using the provided `Makefile`.

### Prerequisites

-   A C++ compiler that supports C++14 (like `g++` or `clang++`).
-   `make`

### Compilation

To build both the `server` and `client` binaries, run:

```bash
make all
```

Alternatively, you can build them individually:

```bash
# Build only the server
make server

# Build only the client
make client
```

## Running the Server

Once built, you can start the server:

```bash
./server
```

The server will start listening for connections on `localhost:1234`. If you see a `bind()` error, it means another process is already using that port. You can find and stop it using `lsof -i :1234` and `kill <PID>`.

## Using the Client

The client is a command-line tool for sending commands to the server.

### Syntax

```bash
./client <command> [arg1] [arg2] ...
```

### Supported Commands

Here are the commands supported by the server:

#### String Operations

-   **`set <key> <value>`**: Set a string value for a key.
    ```bash
    ./client set mykey "hello world"
    ```

-   **`get <key>`**: Get the string value of a key.
    ```bash
    ./client get mykey
    ```

-   **`del <key>`**: Delete a key.
    ```bash
    ./client del mykey
    ```

#### Key Operations

-   **`keys`**: List all keys in the database.
    ```bash
    ./client keys
    ```

-   **`pexpire <key> <milliseconds>`**: Set a time-to-live (TTL) on a key in milliseconds.
    ```bash
    ./client pexpire mykey 5000
    ```

-   **`pttl <key>`**: Get the remaining time-to-live of a key in milliseconds.
    ```bash
    ./client pttl mykey
    ```

#### Sorted Set (ZSet) Operations

-   **`zadd <zset_key> <score> <member>`**: Add a member with a score to a sorted set.
    ```bash
    ./client zadd myzset 10 "member1"
    ./client zadd myzset 20 "member2"
    ```

-   **`zrem <zset_key> <member>`**: Remove a member from a sorted set.
    ```bash
    ./client zrem myzset "member1"
    ```

-   **`zscore <zset_key> <member>`**: Get the score of a member in a sorted set.
    ```bash
    ./client zscore myzset "member2"
    ```

-   **`zquery <zset_key> <score> <name> <offset> <limit>`**: Query for members in a sorted set starting from a given score and name, with offset and limit.
    ```bash
    # Query for up to 5 members starting from score 0, name ""
    ./client zquery myzset 0 "" 0 10
    ```

