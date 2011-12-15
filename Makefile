main: main.o binarization.o selfdescriptiveness.o
	gcc -o main main.o binarization.o selfdescriptiveness.o -lm -O3

main.o: main.c
	gcc -c main.c -O3 -lm
	
binarization.o: binarization.c selfdescriptiveness.o
	gcc -c binarization.c selfdescriptiveness.o -O3 -lm
	
selfdescriptiveness.o: selfdescriptiveness.c
	gcc -c selfdescriptiveness.c -O3 -lm
clean:
	rm -rf *o main