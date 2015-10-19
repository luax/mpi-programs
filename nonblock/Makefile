FLAGS = -std=c99 -O3 -Wall
FILES = non_blocking.c
FILES2 = sendreceive.c

all: non_blocking sendreceive

non_blocking: $(FILES)
	mpicc $(FLAGS) -o non_blocking $(FILES) -D=CODE_3

sendreceive: $(FILES2)
	mpicc $(FLAGS) -o ring $(FILES2)
