make:
	tcc main.c -g -o tifftastic -I/usr/include/SDL2 -lSDL2 -lGL -lGLEW -DSDL_DISABLE_IMMINTRIN_H

gcc:
	gcc main.c -g -o tifftastic -I/usr/include/SDL2 -lSDL2 -lGL -lGLEW -DSDL_DISABLE_IMMINTRIN_H
