NAME = app
CC = g++
FLAGS = -std=c++17 -lglfw -lGLEW -lGL -Wall 
INCLUDES = -I ./dependencies -I ./dependencies/stb_image -I ./dependencies/spdlog -I /dependencies/imgui
MISC = ./dependencies/imgui/*.cpp ./dependencies/imgui/examples/*.cpp


build:
	$(CC) $(INCLUDES) $(MISC) ./src/*.cpp ./src/includes/*.cpp $(FLAGS) -o $(NAME)

build-prod:
	$(CC) $(INCLUDES) $(MISC) ./src/*.cpp ./src/includes/*.cpp $(FLAGS) -o $(NAME) -O3

build-debug:
	$(CC) $(INCLUDES) $(MISC) ./src/*.cpp ./src/includes/*.cpp $(FLAGS) -o $(NAME) -g


build-fast:
	$(CC) $(INCLUDES) ./build/*.o ./src/*.cpp $(FLAGS) -o $(NAME)

build-static:
	mkdir build; $(CC) $(INCLUDES) $(MISC) ./src/includes/*.cpp $(FLAGS) -c -O3 && mv *.o ./build

clean:
	rm ./$(NAME)
	rm ./build/*
