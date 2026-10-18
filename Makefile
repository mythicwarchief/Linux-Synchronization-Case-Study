
CC = gcc
CFLAGS = -Wall -Wextra -pthread
SRC_DIR = src
BIN_DIR = bin

# Add all project source files here, even if some are not ready yet.
CANDIDATE_SOURCES = \
	$(SRC_DIR)/race_condition.c \
	$(SRC_DIR)/mutex_demo.c \
	$(SRC_DIR)/semaphore_demo.c \
	$(SRC_DIR)/spinlock_demo.c \
	$(SRC_DIR)/benchmark.c

# Build only non-empty source files.
SOURCES := $(foreach f,$(CANDIDATE_SOURCES),$(if $(shell test -s $(f) && echo yes),$(f)))
PROGRAMS := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(SOURCES))

.PHONY: all clean list

all: $(PROGRAMS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

list:
	@echo "Available programs:"
	@for program in $(PROGRAMS); do echo "  $$program"; done

clean:
	rm -rf $(BIN_DIR)
