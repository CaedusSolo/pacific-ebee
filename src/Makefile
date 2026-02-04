# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -g -lrt
LDFLAGS =

# Directories
SRC_DIR = .
OBJ_DIR = build
BIN_DIR = bin
DEP_DIR = $(OBJ_DIR)

# Source files
CLIENT_SOURCES = client.cpp renderer.cpp game_client_manager.cpp
SERVER_SOURCES = ServerConnection.cpp server.cpp server_game_manager.cpp
SHARED_SOURCES = Connection.cpp vector2d.cpp serializable.cpp messages.cpp Cell.c Battlefield.c

# Object files
CLIENT_OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(CLIENT_SOURCES)) $(patsubst %.c,$(OBJ_DIR)/%.o,$(filter %.c,$(SHARED_SOURCES)))
SERVER_OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SERVER_SOURCES)) $(patsubst %.c,$(OBJ_DIR)/%.o,$(filter %.c,$(SHARED_SOURCES)))
SHARED_OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(filter %.cpp,$(SHARED_SOURCES))) $(patsubst %.c,$(OBJ_DIR)/%.o,$(filter %.c,$(SHARED_SOURCES)))

# Dependency files
CLIENT_DEPS = $(patsubst %.cpp,$(DEP_DIR)/%.d,$(CLIENT_SOURCES)) $(patsubst %.c,$(DEP_DIR)/%.d,$(filter %.c,$(SHARED_SOURCES)))
SERVER_DEPS = $(patsubst %.cpp,$(DEP_DIR)/%.d,$(SERVER_SOURCES)) $(patsubst %.c,$(DEP_DIR)/%.d,$(filter %.c,$(SHARED_SOURCES)))
SHARED_DEPS = $(patsubst %.cpp,$(DEP_DIR)/%.d,$(filter %.cpp,$(SHARED_SOURCES))) $(patsubst %.c,$(DEP_DIR)/%.d,$(filter %.c,$(SHARED_SOURCES)))

# Targets
CLIENT_TARGET = $(BIN_DIR)/client
SERVER_TARGET = $(BIN_DIR)/server

# Default target
all: $(CLIENT_TARGET) $(SERVER_TARGET)

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Build client
$(CLIENT_TARGET): $(CLIENT_OBJS) $(SHARED_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Build server
$(SERVER_TARGET): $(SERVER_OBJS) $(SHARED_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files and generate dependencies
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	gcc -std=c99 -g -MMD -MP -c -o $@ $^

# Individual targets
client: $(CLIENT_TARGET)
server: $(SERVER_TARGET)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all client server clean
