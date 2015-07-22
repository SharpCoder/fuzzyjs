OBJS = fuzzyjs.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)

fuzzyjs.o: ./src/fjs.cpp
	@$(CC) $(CFLAGS) ./src/fjs.cpp
	@ar rvs fjs.a fjs.o
	@rm fjs.o
	
debug:
	@$(CC) ./src/fjs.cpp
	@./a.out

test:
	@cxxtestgen --error-printer -o ./tests/runner.cpp ./tests/LanguageTests.h
	@g++ -o ./tests/runner ./tests/runner.cpp
	@./tests/runner
	@rm ./tests/runner.cpp
	@rm ./tests/runner

clean:
	@rm *.out
	@rm *.a
