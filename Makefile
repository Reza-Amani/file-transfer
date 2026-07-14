# Optional / legacy. Canonical Windows build is build.ps1 (links Winsock).
CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -pedantic -O2
LDFLAGS = -lws2_32

TARGET  = file-transfer
BUILD   = build

SRCS = \
	src/main.c \
	src/server/server.c \
	src/client/client.c \
	src/common/protocol.c \
	src/common/net.c \
	src/common/path.c

.PHONY: all clean run

all: $(BUILD)/$(TARGET)

$(BUILD)/$(TARGET): $(SRCS) | $(BUILD)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LDFLAGS)

$(BUILD):
	mkdir -p $(BUILD)

run: all
	./$(BUILD)/$(TARGET)

clean:
	rm -rf $(BUILD)
