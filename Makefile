
all: elfinfo elfinfo-mmap  loader hello invert matrix qsort fact

elfinfo: elfinfo.c
	gcc -g -o elfinfo  elfinfo.c

elfinfo-mmap: elfinfo-mmap.c
	gcc -g -o elfinfo-mmap  elfinfo-mmap.c

loader: loader.c
	gcc -g -o loader loader.c

mylib.o: mylib.c
	gcc -c mylib.c

hello: hello.c mylib.o mapfile
	gcc -g -c hello.c
	ld -dn -e mystart -M mapfile hello.o mylib.o -o hello

invert: invert.c mylib.o mapfile
	gcc -g -c invert.c
	ld -dn -e mystart -M mapfile invert.o mylib.o -o invert

matrix: matrix.c mylib.o mapfile
	gcc -g -c matrix.c
	ld -dn -e mystart -M mapfile matrix.o mylib.o  libc.a -o matrix

qsort: qsort.c mylib.o mapfile
	gcc -g -c qsort.c
	ld -dn -e mystart -M mapfile qsort.o mylib.o libc.a -o qsort

fact: fact.c mylib.o mapfile
	gcc -g -c fact.c
	ld -dn -e mystart -M mapfile fact.o mylib.o libc.a -o fact

clean:
	rm -f *.o hello elfinfo elfinfo-mmap core loader invert matrix qsort fact *~ out.elfdump out.elfinfo out.diff
