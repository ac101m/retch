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
DEPS := $(SUB_OBJS:.o=.d)

# Object compilation
$(OBJ_DIR)/%.cpp.o: %.cpp
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(COMPILE_FLAGS) $(INC_FLAGS) -c $< -o $@

# Build target
retch: $(OBJS)
	@$(MKDIR_P) $(dir $(TARGET_EXEC))
	$(CXX) $(OBJS) -o $(TARGET_EXEC) $(LD_FLAGS)

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
