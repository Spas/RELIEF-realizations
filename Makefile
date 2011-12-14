main: main.o binarization.o
	gcc -o main main.o binarization.o -lm -O3

main.o: main.c
	gcc -c main.c -O3 -lm
	
binarization.o: binarization.c
	gcc -c binarization.c -O3 -lm

clean:
	rm -rf *o main