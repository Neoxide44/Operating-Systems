all: shell

shell:
	gcc -std=c99 -Wall -g -pedantic main.c scanner.c shell.c -o shell

bonus:
	gcc -std=c99 -Wall -g -DEXT_PROMPT -pedantic main.c scanner.c shell.c -o shell

clean:
	rm -f *~
	rm -f *.o
	rm -f shell
	rm -f test

build: clean shell 

two: clean bonus