
CC = gcc
CFLAGS = -Wall -Wextra -O3 -DNDEBUG
LFLAGS = -lm

TARGET = qm

SRC_DIR  = src
BIN_DIR  = bin
OBJ_DIR  = obj

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR)/$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) $(LFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

debug: CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -g -O0 -DDEBUG
debug: clean all

# install: $(BIN_DIR)/$(TARGET)
# 	install -d $(DESTDIR)$(PREFIX)/bin
# 	install -m 755 $(BIN_DIR)/$(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)
# 	ln -sf $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET_ALIAS)

# uninstall:
# 	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET_ALIAS)

.PHONY: all clean
