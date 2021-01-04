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
  -S, --screensaver      screensaver mode; equivalent to -li and
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
  -v, --verbose          increase output verbosity
  -h, --help             show help
```

## Modes

### Static

`static` mode is the default: the user only sees the final, completed tree as a picture.

### Live

`live` mode displays each "step" of growth and waits a little bit, so that the user can watch the tree being grown step by step.

## How it Works

`cbonsai` starts by drawing the base onto the screen, which is basically just a static string of characters. To generate the actual tree, `cbonsai` uses a ~~bunch of if statements~~ homemade algorithm to decide how the tree should grow every step. Shoots to the left and right are generated as the main trunk grows. As any branch dies, it branches out into a bunch of leaves.

`cbonsai` has rules for which character and color it should use for each tiny branch piece, depending on things like what type of branch it is and what direction it's facing.

The algorithm is tweaked to look best at the default size, so larger sized trees may not be as bonsai-like.

## Inspiration

This project wouldn't be here if it weren't for its *roots*! `cbonsai` is a newer version of  [bonsai.sh](https://gitlab.com/jallbrit/bonsai.sh), which was written in `bash` and was itself a port of [this bonsai tree generator](http://andai.tv/bonsai/) written in `javascript`.
