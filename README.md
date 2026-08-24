# Chess 
> [!WARNING]
> Its playable..

## How to play
Played like normal chess. Illegal moves are not possible (probably).

## Loading and saving
if you want to load a position, you can using FEN notation, a position is loaded from a file in the current directory called ```saved.txt``` using **L** and saved using **S**. You can also drop the positions file onto the window and it should load. The FEN notation supported is rudimentary only the positions and whose turn are accepted.

You can find different example positions inside of ```positions/``` that I used during testing. Use at your own discretion, loading only checks if you provided the correct amount of pieces even if it is wrong!

## Build from Source
## Windows
Make sure to change the **CC_WIN** variable to a desired windows compiler inside of the makefile.
```console
cd chess
make win && .\build\main.ese
```

## Linux
To build for windows you need to have **x86_64-w64-mingw32-gcc**.
### debug 
```console
cd chess/
make && ./build/main
```
### release
```console
cd chess/
make release && ./build/main
```

## Credits
- [Legal move generation](https://peterellisjones.com/posts/generating-legal-chess-moves-efficiently/)
