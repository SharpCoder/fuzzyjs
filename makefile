OBJS = fuzzyjs.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

fuzzyjs.o: ./src/fjs.cpp
	$(CC) ./src/fjs.cpp

debug:
	$(CC) ./src/fjs.cpp
	./a.out

clean:
	rm a.out
