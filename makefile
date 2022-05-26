main: main.c
	gcc -o out main.c -lglfw -lGL -I"./glad/include" ./glad/src/glad.c -Wall -lm
	 
