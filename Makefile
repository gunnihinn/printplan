BIN=printplan
SRC=main.c

CFLAGS+=-std=c99
CFLAGS+=-Wall
CFLAGS+=-Wextra

$(BIN): $(SRC)
	$(CC) -o $(BIN) $(CFLAGS) $(SRC)

clean:
	rm -f $(BIN)
