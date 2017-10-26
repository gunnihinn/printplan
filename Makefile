BIN=printplan
SRC=main.c
INSTALL_DIR=$(HOME)/Applications

CFLAGS+=-std=c99
CFLAGS+=-Wall
CFLAGS+=-Wextra

$(BIN): $(SRC)
	$(CC) -o $(BIN) $(CFLAGS) $(SRC)

.PHONY: install
install: $(BIN)
	install -C $(BIN) $(INSTALL_DIR)

.PHONY: clean
clean:
	rm -f $(BIN)
