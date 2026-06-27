CC = gcc
FLAGS = -Wall -Wextra -ggdb -std=c99
OUTPUT = build/main
SRC = src/main.c src/board.c src/draw.c src/control.c
INCLUDE = -Iinclude/ -Iassets/ -Iraylib-5.5_linux_amd64/include

all: build main 

main:
	${CC} $(SRC) ${FLAGS} -O3 -o ${OUTPUT} ${INCLUDE} -L raylib-5.5_linux_amd64/lib -l:libraylib.a -lm

debug: build
	${CC} $(SRC) ${FLAGS} -o ${OUTPUT} ${INCLUDE} -DDEBUG -L raylib-5.5_linux_amd64/lib -l:libraylib.a -lm

build:
	mkdir build/

clean:
	rm -rf build/
