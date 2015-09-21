FLAGS = -std=c99 -O3 -Wall
FILES = parallel_data.c
FILES2 = pi.c

all: parallel_data pi

parallel_data: $(FILES)
	mpicc $(FLAGS) -o parallel_data $(FILES)

pi: $(FILES2)
	mpicc $(FLAGS) -o pi $(FILES2)
