# Redis Server and Client Makefile

# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11
SRCDIR = src
INCLUDEDIR = include

# Source files
CLIENT_SRC = $(SRCDIR)/client.cpp
SERVER_SRC = $(SRCDIR)/server.cpp

# Output binaries
CLIENT_BIN = client
SERVER_BIN = server

# Default target
.PHONY: all clean test help

all: $(CLIENT_BIN) $(SERVER_BIN)

# Build client
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -I$(INCLUDEDIR) $< -o $@

# Build server
$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -I$(INCLUDEDIR) $< -o $@

# Individual targets for building just one binary
client: $(CLIENT_BIN)

server: $(SERVER_BIN)

# Clean build artifacts
clean:
	rm -f $(CLIENT_BIN) $(SERVER_BIN)
	rm -rf *.dSYM/

# Basic test to ensure binaries were built correctly
test: all
	@echo "Testing that binaries were built..."
	@if [ -f $(CLIENT_BIN) ]; then \
		echo "✓ Client binary exists"; \
	else \
		echo "✗ Client binary missing"; \
		exit 1; \
	fi
	@if [ -f $(SERVER_BIN) ]; then \
		echo "✓ Server binary exists"; \
	else \
		echo "✗ Server binary missing"; \
		exit 1; \
	fi
	@echo "✓ All tests passed"

# Install binaries to /usr/local/bin (requires sudo)
install: all
	sudo cp $(CLIENT_BIN) /usr/local/bin/redis-client
	sudo cp $(SERVER_BIN) /usr/local/bin/redis-server
	@echo "Installed redis-client and redis-server to /usr/local/bin"

# Uninstall binaries
uninstall:
	sudo rm -f /usr/local/bin/redis-client
	sudo rm -f /usr/local/bin/redis-server
	@echo "Uninstalled redis-client and redis-server from /usr/local/bin"

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build both client and server (default)"
	@echo "  client   - Build only the client"
	@echo "  server   - Build only the server"
	@echo "  clean    - Remove built binaries"
	@echo "  test     - Run basic tests"
	@echo "  install  - Install binaries to /usr/local/bin (requires sudo)"
	@echo "  uninstall- Remove binaries from /usr/local/bin (requires sudo)"
	@echo "  help     - Show this help message"

# Individual targets for building just one binary
client: $(CLIENT_BIN)

server: $(SERVER_BIN)
