# Release targets
TARGET_EXEC ?= retch

# Directory controls
BIN_DIR ?= bin
OBJ_DIR ?= build
SRC_DIR ?= src
INC_DIRS ?= src include
RESOURCE_DIR ?= data

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
release: copy_resources $(OBJS)
	@$(MKDIR_P) $(BIN_DIR)
	$(CXX) $(OBJS) -o $(BIN_DIR)/$(TARGET_EXEC) $(LD_FLAGS)

# Collect all resource files in the bin directors
copy_resources:
	@$(MKDIR_P) $(BIN_DIR)
	cp -r $(RESOURCE_DIR) $(BIN_DIR)

# Build everything
all: release

# Clean, be careful with this
.PHONY: clean
clean:
	@$(RM) -rv $(OBJ_DIR)

# Include dependencies
-include $(DEPS)

# Make directory
MKDIR_P ?= mkdir -p
