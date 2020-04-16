NAME = app
CC = g++
FLAGS = -std=c++17 -lglfw -lGLEW -lGL
INCLUDES = -I ./dependencies -I ./dependencies/stb_image 


build:
	$(CC) $(INCLUDES) ./src/*.cpp ./src/includes/*.cpp $(FLAGS) -o $(NAME)

build-prod:
	$(CC) $(INCLUDES) ./src/*.cpp ./src/includes/*.cpp $(FLAGS) -o $(NAME) -O3

build-debug:
	$(CC) $(INCLUDES) ./src/*.cpp ./src/includes/*.cpp $(FLAGS) -o $(NAME) -g

clean:
	rm ./$(NAME)
