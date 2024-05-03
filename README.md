# 2D-SimpleRender
Simple and easy to use 2D OpenGL Engine.

## Dependencies
This project requires `GLFW3` and `GLEW` to be installed globally on your system.
This project also **ONLY SUPPORTS LINUX**.

## Build
Clone repository with submodules:
```sh
# Either clone the repo recursively, which will also clone submodules.
$ git clone <this repo url> --recursive

# Or if the repo has already been cloned, sync submodules.
$ git submodule update --init --recursive
```

To build the project, simply run:
```sh
$ make

# Then run the built binary
$ ./app
```