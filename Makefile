LDFLAGS = -lX11 -lImlib2 -lconfig -lm
CC = gcc
CFLAGS = -O2
SRC = ./src/*.c
BIN = xawp
BUILD_DIR = ./build/
CONF_DIR = ./.config/xawp/
INSTALL_DIR = /usr/bin/
CONF_FILE = xawp.conf

$(BIN):
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(BUILD_DIR)$(BIN)

# TODO: refactor this mess
install:
	install -t $(INSTALL_DIR) --owner=$(shell stat -c "%U" $(INSTALL_DIR)) --group=$(shell stat -c "%G" $(INSTALL_DIR)) -m 775 $(BUILD_DIR)$(BIN)

# TODO: refactor this mess
install-config:
	for f in /home/*/; do \
		install -d --owner=$$(stat -c "%U" $$f) --group=$$(stat -c "%G" $$f) -m 755 "$$f$(CONF_DIR).." ; \
		install -d --owner=$$(stat -c "%U" $$f) --group=$$(stat -c "%G" $$f) -m 755 "$$f$(CONF_DIR)" ; \
		install -t "$$f$(CONF_DIR)" --owner=$$(stat -c "%U" $$f) --group=$$(stat -c "%G" $$f) -m 755 "$(CONF_DIR)$(CONF_FILE)" ; \
	done

all: $(BIN) install install-config

uninstall:
	rm -f $(INSTALL_DIR)$(BIN)

clean:
	rm -rf $(BUILD_DIR)

TESTDIR = test/
test: $(BIN)
	$(BUILD_DIR)$(BIN)

.PHONY: all install uninstall clean
