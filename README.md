# retch - REnderer from scraTCH
A toy renderer that runs on the CPU - built from scratch

# Build instructions
For your build environment, you will need:
- GNU make
- A working installation of GCC (you'll need to modify the makefile to use another compiler)
- A Linux OS, you probably _may_ be able to build it under windows using MinGW, but I haven't tried. Best of luck to you.
- OpenGL framework 3 (Package name: libglfw3-dev)
- OpenGL extension wrangler (Package name: libglew2.0)

This project also depends on a couple of my own libraries:
- gltools: https://github.com/ac101m/gltools
- optparse: https://github.com/ac101m/optparse

You will need to download and build those first in accordance with their build instructions and add their respective bin/install directories to your search paths before this project will link.
Something like this should do the trick:

```bash
export OPTPARSE_PATH=<path to optparse/bin/install on your system>
export GLTOOLS_PATH=<path to gltools/bin/install on your system>
export CPATH="$CPATH:${OPTPARSE_PATH}:${GLTOOLS_PATH}"
export LIBRARY_PATH="$LIBRARY_PATH:${OPTPARSE_PATH}:${GLTOOLS_PATH}"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:${OPTPARSE_PATH}:${GLTOOLS_PATH}"
```

Next, download this projects source off of github: https://github.com/ac101m/retch
Then you just need to navigate to the root of the source tree and run `make release`

# Running the program
Binaries are located in the bin directory. To run the program, invoke it on the command line. Run retch -h for a list of available command line options.
All it does is draw a spinning utah teapot using your CPU. Enjoy.
