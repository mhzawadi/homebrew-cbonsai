.POSIX:
CC	= cc
PKG_CONFIG	?= pkg-config
CFLAGS	+= -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -pedantic $(shell $(PKG_CONFIG) --cflags ncursesw panelw)
LDLIBS	= $(shell $(PKG_CONFIG) --libs ncursesw panelw || echo "-lncursesw -ltinfo -lpanelw")
PREFIX	= /usr/local
MANDIR	= $(PREFIX)/share/man

cbonsai: cbonsai.c

cbonsai.1: cbonsai.scd
ifeq ($(shell command -v scdoc 2>/dev/null),)
	$(warning Missing dependency: scdoc. The man page will not be generated.)
else
	scdoc <$< >$@
endif

install: cbonsai cbonsai.1
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	install -m 0755 cbonsai $(DESTDIR)$(PREFIX)/bin/cbonsai
	[ ! -f cbonsai.1 ] || install -m 0644 cbonsai.1 $(DESTDIR)$(MANDIR)/man1/cbonsai.1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/cbonsai
	rm -f $(DESTDIR)$(MANDIR)/man1/cbonsai.1

clean:
	rm -f cbonsai
	rm -f cbonsai.1

.PHONY: install uninstall clean
