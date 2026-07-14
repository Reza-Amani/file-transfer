CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -pedantic
LDFLAGS =

TARGET  = file-transfer
BUILD   = build
SRC     = src/main.c

.PHONY: all clean run

all: $(BUILD)/$(TARGET)

$(BUILD)/$(TARGET): $(SRC) | $(BUILD)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD):
	mkdir -p $(BUILD)

run: all
	./$(BUILD)/$(TARGET)

clean:
	rm -rf $(BUILD)
