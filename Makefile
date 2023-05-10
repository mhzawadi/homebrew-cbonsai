.POSIX:
CC	= cc
PKG_CONFIG	?= pkg-config
CFLAGS	+= -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -pedantic $(shell $(PKG_CONFIG) --cflags ncursesw panelw)
LDLIBS	= $(shell $(PKG_CONFIG) --libs ncursesw panelw || echo "-lncursesw -ltinfo -lpanelw")
PREFIX	= /usr/local
DATADIR	= $(PREFIX)/share
MANDIR	= $(DATADIR)/man
WITH_BASH	= 1

cbonsai: cbonsai.c

cbonsai.6: cbonsai.scd
ifeq ($(shell command -v scdoc 2>/dev/null),)
	$(warning Missing dependency: scdoc. The man page will not be generated.)
else
	scdoc <$< >$@
endif

install: cbonsai cbonsai.6
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANDIR)/man6
	install -m 0755 cbonsai $(DESTDIR)$(PREFIX)/bin/cbonsai
	[ ! -f cbonsai.6 ] || install -m 0644 cbonsai.6 $(DESTDIR)$(MANDIR)/man6/cbonsai.6
ifeq ($(WITH_BASH),1)
	mkdir -p $(DESTDIR)$(DATADIR)/bash-completion/completions
	install -m 0644 completions/bash/cbonsai.bash $(DESTDIR)$(DATADIR)/bash-completion/completions/cbonsai
endif

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/cbonsai
	rm -f $(DESTDIR)$(MANDIR)/man6/cbonsai.6
	rm -f $(DESTDIR)$(DATADIR)/bash-completion/completions/cbonsai

clean:
	rm -f cbonsai
	rm -f cbonsai.6

.PHONY: install uninstall clean
