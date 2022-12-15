CC = clang++
CSTD = c++17

EXEC_NAME = a.out

SRC_DIR = ./source
INCLUDE_DIR = ./include
BUILD_DIR = ./build
SRCS = main.cpp 

CFLAGS = $(SRCS) -std=$(CSTD) -Wall -Wpedantic -Wextra $(ADD_FLAGS) -I $(INCLUDE_DIR)

all: build

build:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(EXEC_NAME)

clean:
	rm -rf $(BUILD_DIR)

ifdef ADD_FLAGS
	 $(info Additional compiler flags: $(ADD_FLAGS))
endif

