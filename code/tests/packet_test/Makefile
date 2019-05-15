INC := "../../shared/packet/"

all: clean packet_test.c
	gcc -I $(INC) packet_test.c -o packet_test.out

clean:
	rm -f *.out
