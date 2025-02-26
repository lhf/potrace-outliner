I= letters.pbm
O= out.eps

all:
	cc -O2 -pedantic -Wall -Wextra bitmap_io.c curve.c decompose.c main.c
	./a.out < $I > $O

clean:
	-rm -f *.o a.out $O
