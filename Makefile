BIN=printplan
INSTALL_DIR=$(HOME)/Applications

SRC=src/main.c
HELP_SRC=src/help.txt
HELP_H=src/help.h

CFLAGS+=-std=c99
CFLAGS+=-Wall
CFLAGS+=-Wextra

$(BIN): $(SRC) $(HELP_H)
	$(CC) -o $(BIN) $(CFLAGS) $(SRC)

$(HELP_H): $(HELP_SRC)
	xxd -i $(HELP_SRC) > $(HELP_H)

.PHONY: install
install: $(BIN)
	install -C $(BIN) $(INSTALL_DIR)

.PHONY: clean
clean:
	rm -f $(BIN) $(HELP_H)
