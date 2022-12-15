CC = clang++
CSTD = c++17

EXEC_NAME = a.out
ASM_NAME = asm.s

SRC_DIR = ./source
INCLUDE_DIR = ./include
BUILD_DIR = ./build
SRCS = main.cpp 

CFLAGS = $(SRCS) -std=$(CSTD) -Wall -Wpedantic -Wextra $(ADD_FLAGS) -I $(INCLUDE_DIR)

all: build
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(EXEC_NAME)

build:
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

ifdef ADD_FLAGS
	 $(info Additional compiler flags: $(ADD_FLAGS))
endif

