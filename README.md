# cbonsai

<img src="https://i.imgur.com/rnqJx3P.gif" align="right" width="400px">

`cbonsai` is a bonsai tree generator, written in `C` using `ncurses`. It intelligently creates, colors, and positions a bonsai tree, and is entirely configurable via CLI options-- see [usage](#usage). There are 2 modes of operation: `static` (the default), and `live`. See [modes](#modes) for details.

`cbonsai` is always looking for ideas for improvement- feel free to open an issue if you've got an idea or a bug!

<br>
<br>
<br>
<br>
<br>
<br>

## Installation

### Arch Linux (AUR)

AUR user `nichobi` has kindly packaged `cbonsai` in the AUR, called [cbonsai-git](https://aur.archlinux.org/packages/cbonsai-git). It can be installed via an AUR helper, e.g. `yay`:

```bash
yay -S cbonsai-git
```

### Homebrew

`brew install mhzawadi/cbonsai/cbonsai`

Or `brew tap mhzawadi/cbonsai` and then `brew install cbonsai`.

`brew help`, `man brew` or check [Homebrew's documentation](https://docs.brew.sh).

### Manual

You'll need to have a working `ncurses` library. If you're on a `Debian`-based system, you can install `ncurses` like so:

```bash
sudo apt install ncurses
```

Ensure dependencies are met, then to install:

```bash
git clone https://gitlab.com/jallbrit/cbonsai
cd cbonsai
make install
```

## Usage

```
Usage: cbonsai [OPTION]...

cbonsai is a beautifully random bonsai tree generator.

Options:
  -l, --live             live mode: show each step of growth
  -t, --time=TIME        in live mode, wait TIME secs between
                           steps of growth (must be larger than 0) [default: 0.03]
  -i, --infinite         infinite mode: keep growing trees
  -w, --wait=TIME        in infinite mode, wait TIME between each tree
                           generation [default: 4.00]
  -S, --screensaver      screensaver mode; equivalent to -liWC and
                           quit on any keypress
  -m, --message=STR      attach message next to the tree
  -b, --base=INT         ascii-art plant base to use, 0 is none
  -c, --leaf=LIST        list of comma-delimited strings randomly chosen
                           for leaves
  -M, --multiplier=INT   branch multiplier; higher -> more
                           branching (0-20) [default: 5]
  -L, --life=INT         life; higher -> more growth (0-200) [default: 32]
  -p, --print            print tree to terminal when finished
  -s, --seed=INT         seed random number generator
  -W, --save=FILE        save progress to file [default: ~/.cache/cbonsai]
  -C, --load=FILE        load progress from file [default: ~/.cache/cbonsai]
  -v, --verbose          increase output verbosity
  -h, --help             show help

```

## Tips

### Screensaver Mode

Try out `-S/--screensaver` mode! As the help message states, it activates the `--live` and `--infinite` modes, quits upon any keypress, also saves/loads using the default cache file (`~/.cache/cbonsai`). This means:

* When you start `cbonsai` with `--screensaver`, a tree (including its seed and progress) is loaded from the default cache file.
* When you quit `cbonsai` and `--screensaver` was on, the current tree being generated (including its seed and progress) is written to the default cache file.

This is helpful for a situations like the following: let's say you're growing a really big tree, really slowly:

```bash
$ cbonsai --screensaver --life 40 --multiplier 5 --time 20 --screensaver
```

Normally, when you quite `cbonsai` (e.g. by you hitting `q` or `ctrl-c`), you would lose all progress on that tree. However, by specifying `--screensaver`, the tree is automatically saved to a cache file upon quitting. The next time you run that exact same screensaver command:

```bash
$ cbonsai --screensaver --life 40 --multiplier 5 --time 20 --screensaver
```

The tree is automatically loaded from the cache file! And, since infinite mode is automatically turned on, it will finish the cached tree and just keep generating more. When you quit `cbonsai` again, the tree is once again written to the cache file for next time.

Keep in mind that only the seed and number of branches are written to the cache file, so if you want to continue a previously generated tree, make sure you re-specify any other options you may have changed.

### Add to `.bashrc`

For a new bonsai tree every time you open a terminal, just add the following to the end of your `~/.bashrc`:

```bash
cbonsai -p
```

Notice it uses the print mode, so that you can immediately start typing commands below the bonsai tree.

## How it Works

`cbonsai` starts by drawing the base onto the screen, which is basically just a static string of characters. To generate the actual tree, `cbonsai` uses a ~~bunch of if statements~~ homemade algorithm to decide how the tree should grow every step. Shoots to the left and right are generated as the main trunk grows. As any branch dies, it branches out into a bunch of leaves.

`cbonsai` has rules for which character and color it should use for each tiny branch piece, depending on things like what type of branch it is and what direction it's facing.

The algorithm is tweaked to look best at the default size, so larger sized trees may not be as bonsai-like.

## Inspiration

This project wouldn't be here if it weren't for its *roots*! `cbonsai` is a newer version of  [bonsai.sh](https://gitlab.com/jallbrit/bonsai.sh), which was written in `bash` and was itself a port of [this bonsai tree generator](https://avelican.github.io/bonsai/) written in `javascript`.
