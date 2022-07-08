LDFLAGS = -lX11 -lImlib2 -lconfig
CC = gcc
CFLAGS = -O2
SRC = src/main.c
BIN = xawp
DESTDIR = build/
INSTALLDIR = /usr/bin/

$(BIN):
	mkdir -p $(DESTDIR)
	 $(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(DESTDIR)$(BIN)

install:
	mkdir -p $(DESTDIR)
	cp -f $(DESTDIR)$(BIN) $(INSTALLDIR)
	chmod 755 $(INSTALLDIR)$(BIN)

all: $(BIN) install

uninstall:
	rm -f $(INSTALLDIR)$(BIN)

TESTDIR = test/
test: $(BIN) 
	$(DESTDIR)$(BIN)

.PHONY: all install uninstall
