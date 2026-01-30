.phony all:
all: pman test inf args

pman: main.c linked_list.c
	gcc -Wall main.c linked_list.c -o pman

test: test.c 
	gcc -Wall test.c -o test

inf: inf.c
	gcc -Wall inf.c -o inf

args: args.c
	gcc -Wall args.c -o args

.PHONY clean:
clean:
	-rm -f pman test inf args *.o *.exe