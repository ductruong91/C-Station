main: main.c lbr/jval.c lbr/jrb.c lbr/dllist.c
	gcc -o main main.c lbr/jval.c lbr/jrb.c lbr/dllist.c
run:
	./main
gdb:
	gcc -o main main.c lbr/jval.c lbr/jrb.c lbr/dllist.c -g && gdb ./main