TARGET := app
CC := g++
COMPILER_MACROS = -D SPDLOG_COMPILED_LIB
FLAGS := -std=c++17 -lglfw -lGLEW -lGL -Wall -O3 $(COMPILER_MACROS)
INCLUDES := -I ./dependencies -I ./dependencies/stb_image -I ./dependencies/spdlog/include -I ./dependencies/imgui

# ImGUI backend source paths
IMGUI_PATH := ./dependencies/imgui
IMGUI_BACKEND := $(IMGUI_PATH)/backends/imgui_impl_glfw.cpp $(IMGUI_PATH)/backends/imgui_impl_opengl3.cpp
IMGUI_SRCS := $(IMGUI_BACKEND) $(wildcard $(IMGUI_PATH)/*.cpp)

# spdlog source paths
SPDLOG_PATH := ./dependencies/spdlog
SPDLOG_SRCS := $(wildcard $(SPDLOG_PATH)/src/*.cpp)

# Source files
SRCS := $(wildcard ./src/**/*.cc ./src/*.cc) $(IMGUI_SRCS) $(SPDLOG_SRCS)
_OBJS := $(SRCS:.cc=.o)
OBJS := $(_OBJS:.cpp=.o)


all: $(TARGET)

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

clean:
	rm $(OBJS) $(TARGET)

# DEBUG: Debug logs - Useful for printing deps.
debug:
	$(info Sources:)
	$(foreach src,$(SRCS),$(info - $(src)))

	$(info Objects:)
	$(foreach obj,$(OBJS),$(info - $(obj)))

	$(info Includes:)
	$(foreach file,$(INCLUDES),$(info - $(file)))

	$(info Flags:)
	$(foreach flag,$(FLAGS),$(info - $(flag)))
