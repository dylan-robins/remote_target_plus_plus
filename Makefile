PROJECT_NAME := direplicate

# PATHS
BUILD_DIR := ./build
OBJ_DIR   := $(BUILD_DIR)/obj
SRC_DIR   := ./src
LIB_DIR   := ./lib

# Compiler
CC := clang++
# Preprocessor flags
CPPFLAGS := 
# gcc flags
CFLAGS := -g -std=c++17 -Ilib/ -Ilib/argparse/include
# Linker flags
LDFLAGS := -static -static-libgcc -static-libstdc++
# Linker libraries
LDLIBS :=

# FILES
SRC  := $(patsubst %, $(SRC_DIR)/%, main.cpp config.cpp rsync.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
LIBS := $(patsubst %, $(OBJ_DIR)/%, ini.o INIReader.o)


all: $(BUILD_DIR) $(OBJ_DIR) $(BUILD_DIR)/$(PROJECT_NAME)

# BUILD MAIN PROGRAM
$(BUILD_DIR)/$(PROJECT_NAME): $(OBJS) $(LIBS)
	$(CC) $(LDFLAGS) $(LDLIBS) -Wall -o $@ $^

# BUILD LIBRARIES
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.hpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/ini.o: $(LIB_DIR)/inih/ini.c $(LIB_DIR)/inih/ini.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/INIReader.o: $(LIB_DIR)/inih/cpp/INIReader.cpp $(LIB_DIR)/inih/cpp/INIReader.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR): $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean