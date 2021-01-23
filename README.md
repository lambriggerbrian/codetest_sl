A modified sl command made for the OpenDrives codetest

Build Dependencies:
- Ncurses

Usage:

sl [OPTION] ...

Options:

-a: There are people on the train!
-F: The train flies away
-c: Use a C51 train instead
-l: Use a simpler train
-n [N]: Choose the Nth car (0-N, will wrap if N is bigger than options);
-m [M]: Add M cars to the end of the train (will not count coal cars for some trains)

Makefile usage:

all: build the command (with -w to avoid C++ complaining about using the original header ASCII art)
sl: the build target
debug: build with debugging symbols
package: creates a .deb package in ./debpkgs folder, along with the staging folder. Version is copied from Makefile vars
clean: removes binaries and the debpkgs folder
distclean: removes binaries and the debpkgs folder


Original README Below

SL(1): Cure your bad habit of mistyping
=======================================

SL (Steam Locomotive) runs across your terminal when you type "sl" as
you meant to type "ls". It's just a joke command, and not useful at
all.

Copyright 1993,1998,2014 Toyoda Masashi (mtoyoda@acm.org)

![](demo.gif)
