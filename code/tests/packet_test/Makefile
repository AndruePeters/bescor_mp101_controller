INC := "../../shared/packet/"
CFLAGS := -Wall

all: clean packet_test.c
	gcc -I $(INC) $(CFLAGS) packet_test.c -o packet_test.out

clean:
	rm -f *.out
