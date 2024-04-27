CC:=gcc
C_FILES := $(shell find ./src -not -name "main.c" -and -name '*.c')

TEST_FILES := $(shell find ./tests -not -name "main_test.c" -and -name '*.c')
C_LIBS := -lm
USER_SET_MACRO ?= -DTESTING
BUILD_DIR := ./build
OBJ_FILES := $(C_FILES:%=$(BUILD_DIR)/%.o)
OUTPUT := rx
TEST_OUT := ./test
C_FLAGS := -I./include

all: $(BUILD_DIR)/$(OUTPUT)

test: $(BUILD_DIR)/$(TEST_OUT)

$(BUILD_DIR)/$(TEST_OUT): $(OBJ_FILES) tests/main_test.c
	$(CC) $(C_FLAGS) -o $@ $^ $(C_LIBS) -g -DTESTING

$(BUILD_DIR)/$(OUTPUT): $(OBJ_FILES) src/main.c
	$(CC) $(C_FLAGS) -o $@ $^ $(C_LIBS) -g

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@ -g $(USER_SET_MACRO)

clean:
	rm -r $(BUILD_DIR)/$(SRC_DIR)
	mkdir -p $(BUILD_DIR)
