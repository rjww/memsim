COMPILER=gcc
FLAGS=-std=c11

memsim: headers/*.h src/*.c
	$(COMPILER) -o $@ src/*.c $(FLAGS)