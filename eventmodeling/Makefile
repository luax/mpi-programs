FLAGS = -std=c99 -O3 -Wall
FILES = dist_event.c

all: dist_event

dist_event: $(FILES)
	mpicc $(FLAGS) -o dist_event $(FILES)
