LIBS :=
ifeq ($(NOQA),)
	CFLAGS += -std=c89 -ansi -Wall -Wextra -Wpedantic -pedantic -Wshadow -Werror -Wconversion -Wformat -Wuninitialized -Wmissing-prototypes -Wmissing-declarations -Wstrict-prototypes -Wredundant-decls -Wfloat-equal -Wcast-qual -Wnested-externs -Wvla -Winline -Wmissing-format-attribute -Wmissing-noreturn -pedantic-errors
endif

SRC_DIR := src
OBJ_DIR := obj
BIN := trie

MAIN_SRC := $(SRC_DIR)/main.c
LIB_SRC_FILES := $(filter-out $(MAIN_SRC), $(wildcard $(SRC_DIR)/*.c))
LIB_OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(LIB_SRC_FILES))
MAIN_OBJ := $(OBJ_DIR)/main.o

PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
LIBDIR ?= $(PREFIX)/lib64
INCLUDEDIR ?= $(PREFIX)/include/libtrie

HEADER_FILES := $(wildcard $(SRC_DIR)/*.h)

$(BIN): $(MAIN_OBJ) libtrie
	$(CC) -o $@ $(MAIN_OBJ) -L. -ltrie $(LIBS) $(LDFLAGS)

libtrie: $(LIB_OBJ_FILES)
	$(CC) -shared -o $@.so $(CFLAGS) $(LIB_OBJ_FILES) $(LIBS) $(LDFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h | $(OBJ_DIR)
	$(CC) -c -fPIC -o $@ $(CFLAGS) $< $(LDFLAGS)

.PHONY: clean test

test: libtrie $(BIN)
	if [ $$(LD_LIBRARY_PATH="$(PWD)" ./trie model.bin tests/blog.ari.lt.txt seed 10 10 | sha256sum | awk '{print $$1}') = '3a3040ca2c903d0c54fcc4c84dc807048f962cfc889a4e46d4c7586af8248cfa' ]; then \
		echo 'Test passed.'; \
	else \
		echo 'Test failed' >&2; \
		exit 1; \
	fi

clean:
	rm -rf $(BIN) libtrie.so $(OBJ_DIR)

install: $(BIN) libtrie
	@echo "Installing binary, library, and headers..."
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(INCLUDEDIR)
	cp $(BIN) $(BINDIR)
	cp libtrie.so $(LIBDIR)
	cp $(HEADER_FILES) $(INCLUDEDIR)
	@echo "Installation complete."

uninstall:
	@echo "Removing binary, library, and headers..."
	rm -f $(BINDIR)/$(BIN)
	rm -f $(LIBDIR)/libtrie.so
	rm -rf $(INCLUDEDIR)
	@echo "Uninstallation complete."

.PHONY: install uninstall

strip:
	strip --remove-section=.note.gnu.gold-version --remove-section=.note --remove-section=.gnu.version --remove-section=.eh_frame --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag --strip-symbol=__gmon_start__ --remove-section=.comment --remove-section=.eh_frame_ptr --strip-unneeded --strip-all --strip-debug --merge-notes --strip-dwo --discard-all --discard-locals --verbose libtrie.so
