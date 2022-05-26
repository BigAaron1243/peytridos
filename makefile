main: main.c
	gcc -o out.out main.c -lglfw -lGL -I"./glad/include" ./glad/src/glad.c -Wall -lm
	 
