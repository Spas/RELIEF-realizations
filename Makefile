main: main.o selfdescriptiveness.o selfdescriptiveness_test.o
	cc -o main main.o selfdescriptiveness.o selfdescriptiveness_test.o -lm

main.o: main.c
	cc -c main.c

selfdescriptiveness.o: selfdescriptiveness.c
	cc -c selfdescriptiveness.c
	
selfdescriptiveness_test.o: selfdescriptiveness_test.c
	cc -c selfdescriptiveness_test.c
	
clean:
	rm -rf *o main