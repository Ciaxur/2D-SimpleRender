NAME = app
CC = g++
FLAGS = -std=c++17 -lglfw -lGLEW -lGL -Wall 
INCLUDES = -I ./dependencies -I ./dependencies/stb_image -I ./dependencies/spdlog/include -I ./dependencies/imgui
MISC = ./dependencies/imgui/*.cpp ./dependencies/imgui/examples/*.cpp
OUT = build

build: pre-compile cache-all
	$(CC) $(INCLUDES) $(OUT)/cache/*.o ./src/main.cpp $(FLAGS) -o $(OUT)/$(NAME)

build-prod: pre-compile cache-all
	$(CC) $(INCLUDES) $(OUT)/cache/*.o ./src/main.cpp $(FLAGS) -o $(OUT)/$(NAME) -O3

build-debug:
	$(CC) $(INCLUDES) $(MISC) ./src/*.cpp ./src/includes/*.cpp $(FLAGS) -o $(OUT)/$(NAME) -g

clean:
	rm -rf $(OUT)



#########################
# PRE-COMPILE & CACHING #
#########################

# Gets Paths ready for Compilation
pre-compile:
ifeq (,$(wildcard $(OUT)))			# Create Build Directory
	mkdir $(OUT); \
	mkdir $(OUT)/cache;
else
	$(info Directory exists)
endif

# Executes all Caching
cache-all: pre-compile cache-imgui cache-libs
cache-clear:
	rm $(OUT)/cache/*.o

# ImGUI Static Building
ImGUIPath = ./dependencies/imgui
ImGuiBackend = $(ImGUIPath)/examples/imgui_impl_glfw.cpp $(ImGUIPath)/examples/imgui_impl_opengl3.cpp

cache-imgui: imgui.o
imgui.o: pre-compile $(ImGUIPath)/imgui.cpp $(ImGUIPath)/imgui.h
ifeq (,$(wildcard $(OUT)/cache/imgui*.o))
	cd $(OUT)/cache; \
	$(CC) -I ../../$(ImGUIPath) ../../$(ImGUIPath)/*.cpp $(addprefix ../../,$(ImGuiBackend)) -c
else
	$(info Already Cached ImGUI)
endif

cache-libs: pre-compile BufferData.o SimpleRender.o Texture.o
BufferData.o: cache-imgui
ifeq (,$(wildcard $(OUT)/cache/BufferData.o))
	$(CC) $(INCLUDES) $(OUT)/cache/*.o ./src/includes/BufferData.cpp $(FLAGS) -c -O3 -o $(OUT)/cache/BufferData.o
else
	$(info No Caching for BufferData)
endif

SimpleRender.o: cache-imgui
ifeq (,$(wildcard $(OUT)/cache/SimpleRender.o))
	$(CC) $(INCLUDES) $(OUT)/cache/*.o ./src/includes/SimpleRender.cpp $(FLAGS) -c -O3 -o $(OUT)/cache/SimpleRender.o
else
	$(info No Caching for SimpleRender)
endif

Texture.o: cache-imgui
ifeq (,$(wildcard $(OUT)/cache/Texture.o))
	$(CC) $(INCLUDES) $(OUT)/cache/*.o ./src/includes/Texture.cpp $(FLAGS) -c -O3 -o $(OUT)/cache/Texture.o
else
	$(info No Caching for Texture)
endif
