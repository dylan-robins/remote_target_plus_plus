PROJECT_NAME := remote_target

# PATHS
BUILD_DIR := ./build
OBJ_DIR   := $(BUILD_DIR)/obj
SRC_DIR   := ./src
LIB_DIR   := ./lib

# Compiler
CC := g++
# Preprocessor flags
CPPFLAGS := 
# gcc flags
CFLAGS := -g -std=c++17 -Ilib/ -Ilib/argparse/include
# Linker flags
LDFLAGS := #-static -static-libgcc -static-libstdc++
# Linker libraries
LDLIBS :=

# FILES
SRC  := $(patsubst %, $(SRC_DIR)/%, main.cpp rsync.cpp remoteConfig.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))


all: $(BUILD_DIR) $(OBJ_DIR) $(BUILD_DIR)/$(PROJECT_NAME)

# BUILD MAIN PROGRAM
$(BUILD_DIR)/$(PROJECT_NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(LDLIBS) -Wall -o $@ $^

# BUILD LIBRARIES
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.hpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR): $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR)

test: $(BUILD_DIR)/$(PROJECT_NAME)
	for ex in examples/*.remote;\
	do\
		echo "############### Running $$ex ###############" ;\
		$(BUILD_DIR)/$(PROJECT_NAME) --config $$ex;\
		echo "" ;\
	done

.PHONY: clean test