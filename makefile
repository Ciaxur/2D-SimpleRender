build:
	g++ -I ./dependencies/stb_image ./src/*.cpp ./src/includes/*.cpp -std=c++17 -lglfw -lGLEW -lGL -o app

build-debug:
	g++ -I ./dependencies/stb_image ./src/*.cpp ./src/includes/*.cpp -std=c++17 -lglfw -lGLEW -lGL -g -o app

clean:
	rm ./app
