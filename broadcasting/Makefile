FLAGS = -std=c99 -O3 -Wall
FILES = broadcasting.c

all: broadcasting

broadcasting: $(FILES)
	mpicc $(FLAGS) -o broadcasting $(FILES) -D=CODE_3
