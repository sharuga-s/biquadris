module;
#include <X11/Xlib.h>
export module xwindow;

import <iostream>;
import <string>;

export class Xwindow {
  Display *d;
  Window w;
  int s;
  GC gc;
  unsigned long colours[10];

 public:
  Xwindow(int width=500, int height=500); // ctor
  ~Xwindow(); // dtor
  Xwindow(const Xwindow&) = delete;
  Xwindow &operator=(const Xwindow&) = delete;

  // available colours
  enum {White=0, Black, Red, Green, Blue, Cyan, Yellow, Magenta, Orange, Brown};

  // draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour=Black);

  // draws a string
  void drawString(int x, int y, std::string msg);

};
