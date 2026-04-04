PREFIX ?= /usr/local
DESTDIR ?=

CC = gcc
CFLAGS = -Wall -Wextra -O3 -DNDEBUG
LFLAGS = -lm

TARGET = quixmath
TARGET_ALIAS = qm

SRC_DIR  = src
BIN_DIR  = bin
OBJ_DIR  = obj
TEST_DIR = tests

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(BIN_DIR)/$(TARGET) $(BIN_DIR)/$(TARGET_ALIAS)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR)/$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) $(LFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/$(TARGET_ALIAS): $(BIN_DIR)/$(TARGET)
	ln -sf $(TARGET) $(BIN_DIR)/$(TARGET_ALIAS)

test: $(BIN_DIR)/$(TARGET)
	@passed=0; failed=0; \
	for f in $(TEST_DIR)/*.txt; do \
		first_line=$$(head -n 1 "$$f"); \
		flags=$$(echo "$$first_line" | cut -d'|' -f1 | xargs); \
		equation=$$(echo "$$first_line" | cut -d'|' -f2 | xargs); \
		tail -n +2 "$$f" > /tmp/$(TARGET)_expected; \
		if [ -n "$$flags" ] && [ "$$flags" != "none" ]; then \
			./$(BIN_DIR)/$(TARGET) $$flags "$$equation" > /tmp/$(TARGET)_actual 2>/dev/null; \
		else \
			./$(BIN_DIR)/$(TARGET) "$$equation" > /tmp/$(TARGET)_actual 2>/dev/null; \
		fi; \
		if diff -q /tmp/$(TARGET)_expected /tmp/$(TARGET)_actual > /dev/null 2>&1; then \
			echo "PASS: $$flags | $$equation"; \
			passed=$$((passed + 1)); \
		else \
			echo "FAIL: $$flags | $$equation"; \
			diff /tmp/$(TARGET)_expected /tmp/$(TARGET)_actual; \
			failed=$$((failed + 1)); \
		fi; \
	done; \
	echo ""; \
	echo "$$passed passed, $$failed failed"

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

debug: CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -g -O0 -DDEBUG
debug: clean all

install: $(BIN_DIR)/$(TARGET)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(BIN_DIR)/$(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)
	ln -sf $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET_ALIAS)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET_ALIAS)

.PHONY: all test clean install debug uninstall
