Compile the files in this directory as follows:

g++20h iostream cstdlib string
g++20 -c window.cc
g++20 -c window-impl.cc
g++20 -c graphicsdemo.cc
g++20 *.o -lX11 -o demo

For Mac users, if the last line fails try compiling with:
g++20 *.o -o demo -lX11 -L/usr/X11/lib -I/usr/X11/include

Then run ./demo

You should see a window with a blue bar and some text.

