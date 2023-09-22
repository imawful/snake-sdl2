# SNAKE GAME
a simple snake game written in C using simple direct media layer 2.0
## Controls
* W,A,S,D for movement.
* ESC to exit
* R to restart game (after game ends).
* Window dimensions, Snake Size, and FPS can be changed in source.

## LINUX
make sure SDL2 is installed 
```sh
$ sudo apt install libsdl2-dev libsdl2-2.0-0
```
build using gcc
```sh
$ gcc main.c -lSDL2
```
## WINDOWS
To build on windows make sure you have sdl2 installed somewhere on your system.
You can compile using gcc you just have to link and include the sdl2 library
```sh
$ gcc gcc -Ipath/to/SDL2/include/ -Lpath/to/SDL2/lib main.c -lmingw32 -lSDL2main -lSDL2
```
Running the program on windows requires you to have the SDL2.dll in project folder.

## PREVIEW 
![snake demo](demo.gif)

