#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// global variables
int branches = 0;
int shoots = 0;
int trunks = 1;
int shootsMax = 0;
int shootCounter;

WINDOW *baseWin, *treeWin, *messageBorderWin, *messageWin;
PANEL *myPanels[4];

struct config {
	int live;
	int infinite;
	int screensaver;
	int printTree;
	int verbosity;
	int lifeStart;
	int multiplier;
	int baseType;
	int seed;
	int leavesSize;

	double timeWait;
	double timeStep;

	char* message;
	char* leavesInput;
	char* leaves[100];
};

void finish(void) {
	clear();
	refresh();
	endwin();	// delete ncurses screen
	curs_set(1);
}

void printHelp(struct config conf) {
	printf("Usage: cbonsai [OPTION]...\n");
	printf("\n");
	printf("cbonsai is a beautifully random bonsai tree generator.\n");
	printf("\n");
	printf("Options:\n");
	printf("  -l, --live             live mode: show each step of growth\n");
	printf("  -t, --time=TIME        in live mode, wait TIME secs between\n");
	printf("                           steps of growth (must be larger than 0) [default: %.2f]\n", conf.timeStep);
	printf("  -i, --infinite         infinite mode: keep growing trees\n");
	printf("  -w, --wait=TIME        in infinite mode, wait TIME between each tree\n");
	printf("                           generation [default: %.2f]\n", conf.timeWait);
	printf("  -S, --screensaver      screensaver mode; equivalent to -li and\n");
	printf("                           quit on any keypress\n");
	printf("  -m, --message=STR      attach message next to the tree\n");
	printf("  -b, --base=INT         ascii-art plant base to use, 0 is none\n");
	printf("  -c, --leaf=LIST        list of comma-delimited strings randomly chosen\n");
	printf("                           for leaves\n");
	printf("  -M, --multiplier=INT   branch multiplier; higher -> more\n");
	printf("                           branching (0-20) [default: %i]\n", conf.multiplier);
	printf("  -L, --life=INT         life; higher -> more growth (0-200) [default: %i]\n", conf.lifeStart);
	printf("  -p, --print            print tree to terminal when finished\n");
	printf("  -s, --seed=INT         seed random number generator\n");
	printf("  -v, --verbose          increase output verbosity\n");
	printf("  -h, --help             show help	\n");
}

void drawWins(int baseType, WINDOW* *baseWinPtr, WINDOW* *treeWinPtr) {
	int baseWidth = 0;
	int baseHeight = 0;
	int rows, cols;

	switch(baseType) {
		case 1:
			baseWidth = 31;
			baseHeight = 4;
			break;
		case 2:
			baseWidth = 15;
			baseHeight = 3;
			break;
	}

	// calculate where base should go
	getmaxyx(stdscr, rows, cols);
	int baseOriginY = (rows - baseHeight);
	int baseOriginX = (cols / 2) - (baseWidth / 2);

	// create windows
	*baseWinPtr = newwin(baseHeight, baseWidth, baseOriginY, baseOriginX);
	*treeWinPtr = newwin(rows - baseHeight, cols, 0, 0);

	WINDOW *baseWin = *baseWinPtr;
	WINDOW *treeWin = *treeWinPtr;

	// add windows to array of panels
	myPanels[0] = new_panel(baseWin);
	myPanels[1] = new_panel(treeWin);

	// draw art
	switch(baseType) {
		case 1:
			wattron(baseWin, A_BOLD | COLOR_PAIR(8));
			wprintw(baseWin, "%s", ":");
			wattron(baseWin, COLOR_PAIR(2));
			wprintw(baseWin, "%s", "___________");
			wattron(baseWin, COLOR_PAIR(11));
			wprintw(baseWin, "%s", "./~~~\\.");
			wattron(baseWin, COLOR_PAIR(2));
			wprintw(baseWin, "%s", "___________");
			wattron(baseWin, COLOR_PAIR(8));
			wprintw(baseWin, "%s", ":");

			mvwprintw(baseWin, 1, 0, "%s", " \\                           / ");
			mvwprintw(baseWin, 2, 0, "%s", "  \\_________________________/ ");
			mvwprintw(baseWin, 3, 0, "%s", "  (_)                     (_)");

			wattroff(baseWin, A_BOLD);
			break;
		case 2:
			wattron(baseWin, COLOR_PAIR(8));
			wprintw(baseWin, "%s", "(");
			wattron(baseWin, COLOR_PAIR(2));
			wprintw(baseWin, "%s", "---");
			wattron(baseWin, COLOR_PAIR(11));
			wprintw(baseWin, "%s", "./~~~\\.");
			wattron(baseWin, COLOR_PAIR(2));
			wprintw(baseWin, "%s", "---");
			wattron(baseWin, COLOR_PAIR(8));
			wprintw(baseWin, "%s", ")");

			mvwprintw(baseWin, 1, 0, "%s", " (           ) ");
			mvwprintw(baseWin, 2, 0, "%s", "  (_________)  ");
			break;
	}
}

// roll (randomize) a given die
void roll(int *dice, int mod) { *dice = rand() % mod; }

// check for 'q' key press
void checkKeyPress(int screensaver) {
	if ((screensaver && wgetch(stdscr) != ERR) || (wgetch(stdscr) == 'q')) {
		finish();
		exit(0);
	}
}

void branch(struct config conf, int y, int x, int type, int life) {
	branches++;
	int dx = 0;
	int dy = 0;
	int dice = 0;
	int age = 0;
	int shootCooldown = conf.multiplier;

	while (life > 0) {
		checkKeyPress(conf.screensaver);
		life--;		// decrement remaining life counter
		age = conf.lifeStart - life;

		switch (type) {
			case 0: // trunk

				// new or dead trunk
				if (age <= 2 || life < 4) {
					dy = 0;
					dx = (rand() % 3) - 1;
				}
				// young trunk should grow wide
				else if (age < (conf.multiplier * 3)) {

					// every (multiplier * 0.8) steps, raise tree to next level
					if (age % (int) (conf.multiplier * 0.5) == 0) dy = -1;
					else dy = 0;

					roll(&dice, 10);
					if (dice >= 0 && dice <=0) dx = -2;
					else if (dice >= 1 && dice <= 3) dx = -1;
					else if (dice >= 4 && dice <= 5) dx = 0;
					else if (dice >= 6 && dice <= 8) dx = 1;
					else if (dice >= 9 && dice <= 9) dx = 2;
				}
				// middle-aged trunk
				else {
					roll(&dice, 10);
					if (dice > 2) dy = -1;
					else dy = 0;
					dx = (rand() % 3) - 1;
				}
				break;

			case 1: // left shoot: trend left and little vertical movement
				roll(&dice, 10);
				if (dice >= 0 && dice <= 1) dy = -1;
				else if (dice >= 2 && dice <= 7) dy = 0;
				else if (dice >= 8 && dice <= 9) dy = 1;

				roll(&dice, 10);
				if (dice >= 0 && dice <=1) dx = -2;
				else if (dice >= 2 && dice <= 5) dx = -1;
				else if (dice >= 6 && dice <= 8) dx = 0;
				else if (dice >= 9 && dice <= 9) dx = 1;
				break;

			case 2: // right shoot: trend right and little vertical movement
				roll(&dice, 10);
				if (dice >= 0 && dice <= 1) dy = -1;
				else if (dice >= 2 && dice <= 7) dy = 0;
				else if (dice >= 8 && dice <= 9) dy = 1;

				roll(&dice, 10);
				if (dice >= 0 && dice <=1) dx = 2;
				else if (dice >= 2 && dice <= 5) dx = 1;
				else if (dice >= 6 && dice <= 8) dx = 0;
				else if (dice >= 9 && dice <= 9) dx = -1;
				break;

			case 3: // dying: discourage vertical growth(?); trend left/right (-3,3)
				roll(&dice, 10);
				if (dice >= 0 && dice <=1) dy = -1;
				else if (dice >= 2 && dice <=8) dy = 0;
				else if (dice >= 9 && dice <=9) dy = 1;

				roll(&dice, 15);
				if (dice >= 0 && dice <=0) dx = -3;
				else if (dice >= 1 && dice <= 2) dx = -2;
				else if (dice >= 3 && dice <= 5) dx = -1;
				else if (dice >= 6 && dice <= 8) dx = 0;
				else if (dice >= 9 && dice <= 11) dx = 1;
				else if (dice >= 12 && dice <= 13) dx = 2;
				else if (dice >= 14 && dice <= 14) dx = 3;
				break;

			case 4: // dead: fill in surrounding area
				roll(&dice, 10);
				if (dice >= 0 && dice <= 2) dy = -1;
				else if (dice >= 3 && dice <= 6) dy = 0;
				else if (dice >= 7 && dice <= 9) dy = 1;
				dx = (rand() % 3) - 1;
				break;
		}

		int maxY, maxX;
		(void) maxX;
		getmaxyx(treeWin, maxY, maxX);
		if (dy > 0 && y > (maxY - 2)) dy--; // reduce dy if too close to the ground

		// near-dead branch should branch into a lot of leaves
		if (life < 3) branch(conf, y, x, 4, life);

		// dying trunk should branch into a lot of leaves
		else if (type == 0 && life < (conf.multiplier + 2)) branch(conf, y, x, 3, life);

		// dying shoot should branch into a lot of leaves
		else if ((type == 1 || type == 2) && life < (conf.multiplier + 2)) branch(conf, y, x, 3, life);

		// trunks should re-branch if not close to ground AND either randomly, or upon every <multiplier> steps
		/* else if (type == 0 && ( \ */
		/* 		(rand() % (conf.multiplier)) == 0 || \ */
		/* 		(life > conf.multiplier && life % conf.multiplier == 0) */
		/* 		) ) { */
		else if (type == 0 && (((rand() % 3) == 0) || (life % conf.multiplier == 0))) {

			// if trunk is branching and not about to die, create another trunk with random life
			if ((rand() % 8 == 0) && life > 7) {
				shootCooldown = conf.multiplier * 2;	// reset shoot cooldown
				trunks++;
				branch(conf, y, x, 0, life + (rand() % 5 - 2));
			}

			// otherwise create a shoot
			else if (shootCooldown <= 0) {
				shootCooldown = conf.multiplier * 2;	// reset shoot cooldown

				int shootLife = (life + conf.multiplier);

				// first shoot is randomly directed
				shoots++;
				shootCounter++;
				if (conf.verbosity) mvwprintw(treeWin, 4, 5, "shoots: %02d", shoots);

				// create shoot
				branch(conf, y, x, (shootCounter % 2) + 1, shootLife);
			}
			if (conf.verbosity) mvwprintw(treeWin, 10, 5, "trunks: %02i", trunks);
		}
		shootCooldown--;

		// move in x and y directions
		if (conf.verbosity > 0) {
			mvwprintw(treeWin, 5, 5, "dx: %02d", dx);
			mvwprintw(treeWin, 6, 5, "dy: %02d", dy);
			mvwprintw(treeWin, 7, 5, "type: %d", type);
			mvwprintw(treeWin, 8, 5, "shootCooldown: % 3d", shootCooldown);
		}
		x = (x + dx);
		y = (y + dy);

		// choose color
		switch(type) {
			case 0:
			case 1:
			case 2: // trunk or shoot
				if (rand() % 2 == 0) wattron(treeWin, A_BOLD | COLOR_PAIR(11));
				else wattron(treeWin, COLOR_PAIR(3));
				break;

			case 3: // dying
				if (rand() % 10 == 0) wattron(treeWin, A_BOLD | COLOR_PAIR(2));
				else wattron(treeWin, COLOR_PAIR(2));
				break;

			case 4: // dead
				if (rand() % 3 == 0) wattron(treeWin, A_BOLD | COLOR_PAIR(10));
				else wattron(treeWin, COLOR_PAIR(10));
				break;
		}

		// choose string to use for this branch
		char *branchChar = malloc(100);
		strcpy(branchChar, "?");	// fallback character
		if (life < 4 || type >= 3) strcpy(branchChar, conf.leaves[rand() % conf.leavesSize]);
		else {
			switch(type) {
				case 0: // trunk
					if (dy == 0) strcpy(branchChar, "/~");
					else if (dx < 0) strcpy(branchChar, "\\|");
					else if (dx == 0) strcpy(branchChar, "/|\\");
					else if (dx > 0) strcpy(branchChar, "|/");
					break;
				case 1: // left shoot
					if (dy > 0) strcpy(branchChar, "\\");
					else if (dy == 0) strcpy(branchChar, "\\_");
					else if (dx < 0) strcpy(branchChar, "\\|");
					else if (dx == 0) strcpy(branchChar, "/|");
					else if (dx > 0) strcpy(branchChar, "/");
					break;
				case 2: // right shoot
					if (dy > 0) strcpy(branchChar, "/");
					else if (dy == 0) strcpy(branchChar, "_/");
					else if (dx < 0) strcpy(branchChar, "\\|");
					else if (dx == 0) strcpy(branchChar, "/|");
					else if (dx > 0) strcpy(branchChar, "/");
					break;
			}
		}

		mvwprintw(treeWin, y, x, "%s", branchChar);
		free(branchChar);
		wattroff(treeWin, A_BOLD);

		// if live, show progress
		if (conf.live) {
			// display changes
			update_panels();
			doupdate();

			// convert given time into seconds and nanoseconds and sleep
			struct timespec ts;
			ts.tv_sec = conf.timeStep / 1;
			ts.tv_nsec = (conf.timeStep - ts.tv_sec) * 1000000000;
			nanosleep(&ts, NULL);	// sleep for given time
		}

	}
}

void addSpaces(int count, int *linePosition, int maxWidth) {
	// add spaces if there's enough space
	if (*linePosition < (maxWidth - count)) {
		/* if (verbosity) mvwprintw(treeWin, 12, 5, "inserting a space: linePosition: %02d", *linePosition); */

		// add spaces up to width
		for (int j = 0; j < count; j++) {
			wprintw(messageWin, "%s", " ");
			(*linePosition)++;
		}
	}
}

int drawMessage(struct config conf) {
	if (conf.message == NULL) return 1;

	// determine dimensions of window box
	int maxY, maxX;
	getmaxyx(stdscr, maxY, maxX);
	int boxWidth = 0;
	int boxHeight = 0;
	if (strlen(conf.message) + 3 <= (0.25 * maxX)) {
		boxWidth = strlen(conf.message) + 1;
		boxHeight = 1;
	} else {
		boxWidth = 0.25 * maxX;
		boxHeight = (strlen(conf.message) / boxWidth) + (strlen(conf.message) / boxWidth);
	}
	if (conf.verbosity) mvwprintw(treeWin, 8, 5, "boxWidth: %0d", boxWidth);

	// create separate box for message border
	messageBorderWin = newwin(boxHeight + 2, boxWidth + 4, (maxY * 0.7) - 1, (maxX * 0.7) - 2);
	messageWin = newwin(boxHeight, boxWidth + 1, maxY * 0.7, maxX * 0.7);

	// draw box
	wattron(messageBorderWin, COLOR_PAIR(8));
	box(messageBorderWin, 0, 0);

	// assign new windows to array of panels
	myPanels[2] = new_panel(messageBorderWin);
	myPanels[3] = new_panel(messageWin);

	// word wrap message as it is written
	unsigned int i = 0;
	int linePosition = 0;
	int wordLength = 0;
	char wordBuffer[500];
	wordBuffer[0] = '\0';
	char thisChar;
	int messageBoxWidth = boxWidth - 1;
	while (true) {
		thisChar = conf.message[i];
		if (conf.verbosity) {
			mvwprintw(treeWin, 9, 5, "index: %03d", i);
			mvwprintw(treeWin, 10, 5, "linePosition: %02d", linePosition);
		}

		// if char is not a space or null char
		if (!(isspace(thisChar) || thisChar == '\0')) {
			strncat(wordBuffer, &thisChar, 1); // append thisChar to wordBuffer
			wordLength++;
			linePosition++;
		}

		// if char is space or null char
		else if (isspace(thisChar) || thisChar == '\0') {

			// if current line can fit word, add word to current line
			if (linePosition <= messageBoxWidth) {
				wprintw(messageWin, "%s", wordBuffer);	// print word
				wordLength = 0;		// reset word length
				wordBuffer[0] = '\0';	// clear word buffer

				switch (thisChar) {
					case ' ':
						addSpaces(1, &linePosition, messageBoxWidth);
						break;
					case '\t':
						addSpaces(1, &linePosition, messageBoxWidth);
						break;
					case '\n':
						waddch(messageWin, thisChar);
						linePosition = 0;
						break;
				}

			}

			// if word can't fit within a single line, just print it
			else if (wordLength > messageBoxWidth) {
				wprintw(messageWin, "%s ", wordBuffer);	// print word
				wordLength = 0;		// reset word length
				wordBuffer[0] = '\0';	// clear word buffer

				// our line position on this new line is the x coordinate
				int y;
				(void) y;
				getyx(messageWin, y, linePosition);
			}

			// if current line can't fit word, go to next line
			else {
				if (conf.verbosity) mvwprintw(treeWin, (i / 24) + 28, 5, "couldn't fit word. linePosition: %02d, wordLength: %02d", linePosition, wordLength);
				wprintw(messageWin, "\n%s ", wordBuffer); // print newline, then word
				linePosition = wordLength;	// reset line position
				wordLength = 0;		// reset word length
				wordBuffer[0] = '\0';	// clear word buffer
			}
		}
		else {
			printf("%s", "Error while parsing message");
			return 1;
		}

		if (conf.verbosity >= 2) {
			update_panels();
			doupdate();

			// sleep for a little bit
			struct timespec ts;
			ts.tv_sec = 1;
			ts.tv_nsec = 0;
			nanosleep(&ts, NULL);

			mvwprintw(treeWin, 11, 5, "word buffer: |% 15s|", wordBuffer);
		}
		if (thisChar == '\0') break;	// quit when we reach the end of the message
		i++;
	}
	return 0;
}

void init(struct config conf) {
	savetty();	// save terminal settings
	initscr();	// init ncurses screen
	noecho();	// don't echo input to screen
	curs_set(0);	// make cursor invisible
	cbreak();	// don't wait for new line to grab user input
	nodelay(stdscr, TRUE);	// force getch to be a non-blocking call

	// if terminal has color capabilities, use them
	if (has_colors()) {
		start_color();

		// define color pairs
		if (use_default_colors() != ERR) {
			init_pair(0, 0, -1);
			init_pair(1, 1, -1);
			init_pair(2, 2, -1);
			init_pair(3, 3, -1);
			init_pair(4, 4, -1);
			init_pair(5, 5, -1);
			init_pair(6, 6, -1);
			init_pair(7, 7, -1);
			init_pair(8, 8, -1);
			init_pair(9, 9, -1);
			init_pair(10, 10, -1);
			init_pair(11, 11, -1);
			init_pair(12, 12, -1);
			init_pair(13, 13, -1);
			init_pair(14, 14, -1);
			init_pair(15, 15, -1);
		} else {
		init_pair(0, 0, COLOR_BLACK);
		init_pair(1, 1, COLOR_BLACK);
		init_pair(2, 2, COLOR_BLACK);
		init_pair(3, 3, COLOR_BLACK);
		init_pair(4, 4, COLOR_BLACK);
		init_pair(5, 5, COLOR_BLACK);
		init_pair(6, 6, COLOR_BLACK);
		init_pair(7, 7, COLOR_BLACK);
		init_pair(8, 8, COLOR_BLACK);
		init_pair(9, 9, COLOR_BLACK);
		init_pair(10, 10, COLOR_BLACK);
		init_pair(11, 11, COLOR_BLACK);
		init_pair(12, 12, COLOR_BLACK);
		init_pair(13, 13, COLOR_BLACK);
		init_pair(14, 14, COLOR_BLACK);
		init_pair(15, 15, COLOR_BLACK);
		}
	} else {
		printf("%s", "Warning: terminal does not have color support.");
	}

	// define and draw windows, then create panels
	drawWins(conf.baseType, &baseWin, &treeWin);
	drawMessage(conf);
}

void growTree(struct config conf) {
	int maxY, maxX;
	getmaxyx(treeWin, maxY, maxX);

	branches = 0;
	shoots = 0;
	shootCounter = rand();

	if (conf.verbosity > 0) {
		mvwprintw(treeWin, 2, 5, "maxX: %03d, maxY: %03d", maxX, maxY);
	}
	branch(conf, maxY - 1, (maxX / 2), 0, conf.lifeStart);	// grow tree trunk

	// display changes
	update_panels();
	doupdate();
}

int main(int argc, char* argv[]) {
	struct config conf = {
		.live = 0,
		.infinite = 0,
		.screensaver = 0,
		.printTree = 0,
		.verbosity = 0,
		.lifeStart = 32,
		.multiplier = 5,
		.baseType = 1,
		.seed = 0,
		.leavesSize = 0,

		.timeWait = 4,
		.timeStep = 0.03,

		.message = NULL,
		.leavesInput = "&",
		.leaves = {0},
	};

	struct option long_options[] = {
		{"live", no_argument, NULL, 'l'},
		{"time", required_argument, NULL, 't'},
		{"infinite", no_argument, NULL, 'i'},
		{"wait", required_argument, NULL, 'w'},
		{"screensaver", no_argument, NULL, 'S'},
		{"message", required_argument, NULL, 'm'},
		{"base", required_argument, NULL, 'b'},
		{"leaf", required_argument, NULL, 'c'},
		{"multiplier", required_argument, NULL, 'M'},
		{"life", required_argument, NULL, 'L'},
		{"print", required_argument, NULL, 'p'},
		{"seed", required_argument, NULL, 's'},
		{"verbose", no_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{0, 0, 0, 0}
	};

	// parse arguments
	int option_index = 0;
	int c;
	while ((c = getopt_long(argc, argv, "lt:iw:Sm:b:c:M:L:ps:vh", long_options, &option_index)) != -1) {
		switch (c) {
			case 'l':
				conf.live = 1;
				break;
			case 't':
				if (strtold(optarg, NULL) != 0) conf.timeStep = strtod(optarg, NULL);
				else {
					printf("error: invalid step time: '%s'\n", optarg);
					exit(1);
				}
				if (conf.timeStep < 0) {
					printf("error: invalid step time: '%s'\n", optarg);
					exit(1);
				}
				break;
			case 'i':
				conf.infinite = 1;
				break;
			case 'w':
				if (strtold(optarg, NULL) != 0) conf.timeWait = strtod(optarg, NULL);
				else {
					printf("error: invalid wait time: '%s'\n", optarg);
					exit(1);
				}
				if (conf.timeWait < 0) {
					printf("error: invalid wait time: '%s'\n", optarg);
					exit(1);
				}
				break;
			case 'S':
				conf.live = 1;
				conf.infinite = 1;
				conf.screensaver = 1;
				break;
			case 'm':
				conf.message = optarg;
				break;
			case 'b':
				if (strtold(optarg, NULL) != 0) conf.baseType = strtod(optarg, NULL);
				else {
					printf("error: invalid base index: '%s'\n", optarg);
					exit(1);
				}
				break;
			case 'c':
				conf.leavesInput = optarg;
				break;
			case 'M':
				if (strtold(optarg, NULL) != 0) conf.multiplier = strtod(optarg, NULL);
				else {
					printf("error: invalid multiplier: '%s'\n", optarg);
					exit(1);
				}
				if (conf.multiplier < 0) {
					printf("error: invalid multiplier: '%s'\n", optarg);
					exit(1);
				}
				break;
			case 'L':
				if (strtold(optarg, NULL) != 0) conf.lifeStart = strtod(optarg, NULL);
				else {
					printf("error: invalid initial life: '%s'\n", optarg);
					exit(1);
				}
				if (conf.lifeStart < 0) {
					printf("error: invalid initial life: '%s'\n", optarg);
					exit(1);
				}
				break;
			case 'p':
				conf.printTree = 1;
				break;
			case 's':
				if (strtold(optarg, NULL) != 0) conf.seed = strtod(optarg, NULL);
				else {
					printf("error: invalid seed: '%s'\n", optarg);
					exit(1);
				}
				if (conf.seed < 0) {
					printf("error: invalid seed: '%s'\n", optarg);
					exit(1);
				}
				break;
			case 'v':
				conf.verbosity++;
				break;

			// '?' represents unknown option. Treat it like --help.
			case '?':
			case 'h':
				printHelp(conf);
				return 0;
				break;
		}
	}

	// delimit leaves on "," and add each token to the leaves[] list
	char *token = strtok(conf.leavesInput, ",");
	while (token != NULL) {
		if (conf.leavesSize < 100) conf.leaves[conf.leavesSize] = token;
		token = strtok(NULL, ",");
		conf.leavesSize++;
	}

	shootsMax = conf.multiplier + 1;

	// seed random number generator
	if (conf.seed == 0) conf.seed = time(NULL);
	srand(conf.seed);

	do {
		init(conf);
		growTree(conf);
		if (conf.infinite) {
			timeout(conf.timeWait * 1000);
			checkKeyPress(conf.screensaver);

			// seed random number generator
			conf.seed = time(NULL);
			srand(conf.seed);
		}
	} while (conf.infinite);

	if (conf.printTree) {
		finish();

		// overlay all windows onto stdscr
		overlay(baseWin, stdscr);
		overlay(treeWin, stdscr);
		overlay(messageBorderWin, stdscr);
		overlay(messageWin, stdscr);

		// copy contents of stdscr to the terminal window
		int maxY, maxX, color, attribs;
		getmaxyx(stdscr, maxY, maxX);
		for (int y = 0; y < maxY; y++) {
			for (int x = 0; x < maxX; x++) {
				// get attributes of this character
				color = mvwinch(stdscr, y, x) & A_COLOR;
				attribs = (mvwinch(stdscr, y, x) & A_ATTRIBUTES) - color;
				color /= 256;

				// enable bold if needed
				if ((attribs) == A_BOLD) printf("%s", "\033[1m");
				else printf("%s", "\033[0m");

				// enable correct color
				if (color == 0) printf("%s", "\033[0m");
				else if (color <= 7) printf("\033[3%im", color);
				else if (color >= 8) printf("\033[9%im", color - 8);

				// print character
				printf("%c", mvwinch(stdscr, y, x));
			}
		}
		printf("%s\n", "\033[0m");
	} else {
		wgetch(treeWin);
		finish();
	}

	// free window memory
	delwin(baseWin);
	delwin(treeWin);
	delwin(messageBorderWin);
	delwin(messageWin);

	return 0;
}
