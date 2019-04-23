# Release targets
TARGET_EXEC ?= bin/retch

# Directory controls
OBJ_DIR ?= build
SRC_DIR ?= src
INC_DIRS ?= src include

# Compiler configuration
CXX := g++
COMPILE_FLAGS ?= -MMD -MP -m64 -std=c++14 -Wall -O3
INC_FLAGS ?= -Iinclude -Isrc
LD_FLAGS ?= -loptparse -lgltools -lGLEW -lglfw -lGL

# Enumerate sources
SRCS := $(shell find $(SRC_DIR) -name *.cpp)
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Object compilation
$(OBJ_DIR)/%.cpp.o: %.cpp
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(COMPILE_FLAGS) $(INC_FLAGS) -c $< -o $@

# Build target
retch: move_shaders $(OBJS)
	@$(MKDIR_P) $(dir $(TARGET_EXEC))
	$(CXX) $(OBJS) -o $(TARGET_EXEC) $(LD_FLAGS)

# Simple target, collect glsl files in the shaders folder
GLSL_SRCS := $(shell find $(SRC_DIR) -name *.glsl)
SHADER_BIN_DIR := bin/shaders
move_shaders:
	@$(MKDIR_P) $(SHADER_BIN_DIR)
	cp $(GLSL_SRCS) $(SHADER_BIN_DIR)

# Build everything
all: retch

# Clean, be careful with this
.PHONY: clean
clean:
	@$(RM) -rv $(OBJ_DIR)

# Include dependencies
-include $(DEPS)

# Make directory
MKDIR_P ?= mkdir -p
