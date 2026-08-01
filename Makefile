CC = gcc
FLAGS = -Wall -Wextra -ggdb -std=c99
OUTPUT = build/main
SRC = src/main.c src/board.c src/draw.c src/control.c
INCLUDE = -Iinclude/ -Iassets/ -Iraylib/include

linux: build
	${CC} $(SRC) ${FLAGS} -O3 -o ${OUTPUT} ${INCLUDE} -L raylib/lib -l:libraylib.a -lm

win: build
	${CC} $(SRC) ${FLAGS} -o build\main.exe ${INCLUDE} -Wl,--subsystem,windows -Wl,--entry,mainCRTStartup -L raylib\lib -l:libraylib_win.a -lgdi32 -lwinmm -lm

debug: build
	${CC} $(SRC) ${FLAGS} -o ${OUTPUT} ${INCLUDE} -DDEBUG -L raylib/lib -l:libraylib.a -lm

build:
	mkdir build/

clean:
	rm -rf build/
