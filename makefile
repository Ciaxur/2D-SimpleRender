build:
	g++ ./src/*.cpp ./src/includes/*.cpp -lglfw -lGLEW -lGL -o app

build-debug:
	g++ ./src/*.cpp ./src/includes/*.cpp -lglfw -lGLEW -lGL -g -o app

clean:
	rm ./app
