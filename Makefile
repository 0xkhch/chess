CC = gcc 
FLAGS = -Wall -Wextra -ggdb -std=c99 
OUTPUT = build/main
src = main.c

all: build main 

main:
	${CC} $(src) ${FLAGS} -o ${OUTPUT} -I raylib-5.5_linux_amd64/include -L raylib-5.5_linux_amd64/lib -l:libraylib.a -lm

build:
	mkdir build/

clean:
	rm -rf build/
