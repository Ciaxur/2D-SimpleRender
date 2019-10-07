build:
	g++ ./*.cpp ./includes/*.cpp -lglfw -lGLEW -lGL -o app

build-debug:
	g++ ./*.cpp ./includes/*.cpp -lglfw -lGLEW -lGL -g -o app

clean:
	rm ./app
