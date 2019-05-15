INC := "../../shared/node/"
CFLAGS := "-Wall"

all: clean node_test.c
	gcc -I $(INC) $(CFLAGS) node_test.c -o node_test.out

clean:
	rm -f *.out
