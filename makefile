.PHONY: install
install: uninstall cbonsai.c
	mkdir ~/.local/bin/cbonsai
	gcc cbonsai.c -Wall -Wpedantic -l panel -l ncurses -o cbonsai
	cp cbonsai ~/.local/bin/cbonsai

.PHONY: uninstall
uninstall:
	rm -f cbonsai ~/.local/bin/cbonsai
