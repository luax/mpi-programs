FLAGS = -std=c99 -O3 -Wall
FILES = broadcasting.c
FILES2 = ring.c

all: broadcasting ring

broadcasting: $(FILES)
	mpicc $(FLAGS) -o broadcasting $(FILES) -D=CODE_3

ring: $(FILES2)
	mpicc $(FLAGS) -o ring $(FILES2) -D=CODE_2
