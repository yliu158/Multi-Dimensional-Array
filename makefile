CPPFLAGS= -Wall -Wextra -pedantic -g

all:
	g++ -std=c++17 $(CPPFLAGS) Test_Array.cpp -o Test_Array

run:
	./Test_Array

checkmem:
	valgrind --leak-check=yes -v ./Test_Array

clean:
	rm Test_Array
