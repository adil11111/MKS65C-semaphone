all: write.c control.c
	gcc -g write.c
	gcc control.c -o control.out

setup:
	./control.out $(args)

run:
	./a.out

clean:
	rm -rf *.o *.out
