TARGET := app
CC := g++
COMPILER_MACROS = -D SPDLOG_COMPILED_LIB
OPTIMIZATIONS := -O3 -g
FLAGS := -std=c++17 -lGLEW -lGL -Wall -Wextra -Wno-unused-parameter $(COMPILER_MACROS) $(OPTIMIZATIONS)
INCLUDES := $(patsubst %,-I%, \
	./dependencies \
	./dependencies/glm/ \
	./dependencies/glfw/include \
	./dependencies/stb_image \
	./dependencies/spdlog/include \
	./dependencies/imgui \
	./src/includes/ \
	$(wildcard ./src/includes/**/) \
)


# ImGUI backend source paths
IMGUI_PATH := ./dependencies/imgui
IMGUI_BACKEND := $(IMGUI_PATH)/backends/imgui_impl_glfw.cpp $(IMGUI_PATH)/backends/imgui_impl_opengl3.cpp
IMGUI_SRCS := $(IMGUI_BACKEND) $(wildcard $(IMGUI_PATH)/*.cpp)

# spdlog source paths
SPDLOG_PATH := ./dependencies/spdlog
SPDLOG_SRCS := $(wildcard $(SPDLOG_PATH)/src/*.cpp)

# GLFW source paths
GLFW_PATH 				:= ./dependencies/glfw
GLFW_COMPILED_DIR := ./dependencies/glfw/build/src/CMakeFiles/glfw.dir
GLFW_BUILD 				:= ./dependencies/glfw/build
GLFW_BUILD_STAMP 	:= glfw-built
GLFW_OBJS 				:= $(wildcard $(GLFW_COMPILED_DIR)/*.o)

# Source files
SRCS 	:= $(wildcard ./src/**/*.cc ./src/**/**/*.cc ./src/*.cc) $(IMGUI_SRCS) $(SPDLOG_SRCS)
_OBJS := $(SRCS:.cc=.o)
OBJS 	:= $(_OBJS:.cpp=.o) $(GLFW_OBJS)


all: glfw $(TARGET)

# Main build binary target.
# $@ -> $(TARGET)
# $^ -> All pre-requisites, which are $(OBJS)
$(TARGET): $(OBJS)
	$(CC) $(INCLUDES) $(FLAGS) $^ -o $@

# Rule to compile source files to object files. This allows re-compiling modified source files.
# $< -> The %.cc's resolved filename
%.o: %.cc
	$(CC) $(INCLUDES) $(FLAGS) -c -o $@ $<
%.o: %.cpp
	$(CC) $(INCLUDES) $(FLAGS) -c -o $@ $<

clean-glfw:
	rm -rf $(GLFW_BUILD) $(GLFW_BUILD_STAMP)

glfw: $(GLFW_BUILD_STAMP)

$(GLFW_BUILD_STAMP):
	cd $(GLFW_PATH) && mkdir -p build && cd build && cmake .. && make
	touch $(GLFW_BUILD_STAMP)

clean: clean-glfw
	rm $(OBJS) $(TARGET)

debug:
	$(info Sources:)
	$(foreach src,$(SRCS),$(info - $(src)))

	$(info Objects:)
	$(foreach obj,$(OBJS),$(info - $(obj)))

	$(info Includes:)
	$(foreach file,$(INCLUDES),$(info - $(file)))

	$(info Flags:)
	$(foreach flag,$(FLAGS),$(info - $(flag)))

help:
	@echo "Available targets:"
	@echo "  all   				: Builds everything"
	@echo "  clean  			: Cleans up all built objects, cache, and binaries"
	@echo "  clean-glfw  	: Cleans up built glfw files"
	@echo "  debug  			: Prints debug menu, which contains resolved variables"
	@echo "  glfw  				: Builds glfw dependency"
	@echo "  help   			: Prints this menu"