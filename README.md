# Chess 
> [!WARNING]
> Its playable..

## How to play
Played like normal chess, if you want to load a position, you can using FEN notation, a position is loaded from a file in the current directory called ```saved.txt``` using **L** and saved using **S**.
The FEN notation supported is rudimentary only the positions and whose turn.

You can find different example positions inside of ```positions/``` that I used during testing. Use at your own discretion, loading only checks if you provided the correct amount of pieces even if it is wrong!

## Build from Source
### Linux release
```console
cd chess/
make && ./build/main
```
### Linux debug 
```console
cd chess/
make debug && ./build/main
```

## TODO 
1. Remove possible moves if a piece is pinned.
2. King can not move to a place behind its position when its pinned by a queen, bishop or rook.

## Credits
- [Legal move generation](https://peterellisjones.com/posts/generating-legal-chess-moves-efficiently/)
