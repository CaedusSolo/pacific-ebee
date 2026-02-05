CC = gcc
CFLAGS = -g -std=c99
LDFLAGS = -lrt -lm
OBJDIR = obj
BINDIR = bin

# Source files
CLIENT_SRC = client.c renderer.c game_client_manager.c
SERVER_SRC = server.c server_connection.c server.c server_game_manager.c
SHARED_SRC = vector2d.c connection.c cell.c battlefield.c

# Object files
CLIENT_OBJ = $(CLIENT_SRC:%.c=$(OBJDIR)/%.o) $(SHARED_SRC:%.c=$(OBJDIR)/%.o)
SERVER_OBJ = $(SERVER_SRC:%.c=$(OBJDIR)/%.o) $(SHARED_SRC:%.c=$(OBJDIR)/%.o)

CLIENT_TARGET = $(BINDIR)/client
SERVER_TARGET = $(BINDIR)/server

# Targets
all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJ) | $(BINDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(SERVER_TARGET): $(SERVER_OBJ) | $(BINDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)/client $(BINDIR)/server

.PHONY: all clean
