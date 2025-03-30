compile: build run

build:
	gcc $(wildcard code_c/*.c)  -o ./out_game/bomberman.exe -lSDL3 -lSDL3_image

run:
	cmd /c out_game\bomberman.exe