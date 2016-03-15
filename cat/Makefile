all: cat

hello_world: cat.o
	gcc -s cat.o -o cat

hello_world.o: cat.c
	gcc -c cat.c -o cat.o

clean:
	rm -f cat cat.o
