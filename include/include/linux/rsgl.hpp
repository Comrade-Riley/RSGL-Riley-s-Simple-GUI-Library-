#pragma once
#include <X11/Xlib.h>
#include <cstdio>
#include <math.h>
#include <X11/keysym.h>
#define RSGLRGBTOHEX(r, g, b) ((r << 16) + (g << 8) + b)

namespace RSGL{
    Display* display;
    GC gc;
    char keyboard[32];
    Window window;
    int screenNumber;
    int init();
    int Quit();
    int clear(); //clears everything and redraws things defined below it

    XEvent event;
    //page 189
    int KeyPressed=2;
    int KeyReleased=3;
    int MouseButtonPressed=4;
    int MouseButtonReleased=5;
    int MousePosChanged=6;
    /*int EnterNotify=7;
    int LeaveNotify=8;
    int FocusIn=9;
    int FocusOut=10;
    int KeymapNotify=11;
    int Expose=12;
    int GraphicsExpose=13;
    int NoExpose=14;
    int VisibilityNotify=15;
    int CreateNotify=16;
    int DestroyNotify=17;
    int UnmapNotify=18;
    int MapNotify=19;
    icnt MapRequest=20;
    /*
    int ReparentNotify=21;
    */
    int WindowConfigurationUpdated=22;
    /*
    int ConfigureRequest=23;
    int GravityNotify=24;
    int ResizeRequest=25;
    int CirculateNotify=26;
    int CirculateRequest=27;
    int PropertyNotify=28;
    int SelectionClear=29;
    int SelectionRequest=30;
    int SelectionNotify=31;
    int ColormapNotify=32;
    */
    int quit;
    /*int MappingNotify=34;
    int GenericEvent=35;
    int LASTEvent=36;*/

    struct point{int x, y;};
    struct rect{int x, y; int length,width;};
    struct circle{int x, y; int radius;};
    struct color{int r,g,b;};
    
    bool isPressed(unsigned long key);
    int drawPoint(RSGL::point p, color c);

    int drawRect(RSGL::rect r,color c, bool fill=True){
      for (int l=0; l < r.length+1; l++){
        for (int w=0; w < r.width+1; w++){
          if (fill || w==0 || w == r.width || l==0 || l == r.length){
            RSGL::drawPoint({l+r.x,w+r.y},c);
          }
        }
      }
      return 1;
    }

    int drawCircle(RSGL::circle c, color col,bool fill=True){
      if (fill){
        for (int y=-c.radius; y < (c.radius-1); y++){
          for (int x=-c.radius; x < (c.radius-1); x++){
            if ((x * x) + (y * y) <= (c.radius * c.radius)){
              RSGL::drawPoint({c.x + x,c.y + y},col);
            }
          }
        }
      }
      else {
        int x, y;
        int l;

        for (x = 0; x <= (c.radius-1); x++) {
          y = (int) sqrt ((double) (c.radius * c.radius) - (x * x));

          RSGL::drawPoint({c.x+x, c.y+y},col);
          RSGL::drawPoint({c.x+x, c.y+-y},col);
          RSGL::drawPoint({c.x+-x,c.y+ y},col);
          RSGL::drawPoint({c.x+-x,c.y+ -y},col);

          RSGL::drawPoint({c.x+y, c.y+x},col);
          RSGL::drawPoint({c.x+y, c.y+-x},col);
          RSGL::drawPoint({c.x+-y,c.y+ x},col);
          RSGL::drawPoint({c.x+-y,c.y+ -x},col);
        }
      }
      return 1;
    }
};

int RSGL::init(){
    display = XOpenDisplay(NULL);
    if (display == NULL){return -1;}
    screenNumber = DefaultScreen(display);
    return 0;
}

int RSGL::Quit(){
    if (!XCloseDisplay(display)){return -1;}
    return 0;
}


int RSGL::clear(){
  XClearWindow(display,RSGL::window);
  return 1;
}


bool RSGL::isPressed(unsigned long key) {
    KeyCode kc2 = XKeysymToKeycode(RSGL::display, key);
    bool isPressed = !!(RSGL::keyboard[kc2 >> 3] & (1 << (kc2 & 7)));
    return isPressed;
}

int RSGL::drawPoint(RSGL::point p, color c){
  XSetForeground(RSGL::display,RSGL::gc,RSGLRGBTOHEX(c.r,c.g,c.b));
  XDrawPoint(RSGL::display,RSGL::window,RSGL::gc,p.x,p.y);
  return 1;
  XSetForeground(RSGL::display,RSGL::gc,0x000000);
}


struct Event{
  int type;
  int button;
  int x,y;
  void CheckEvents();
};

void Event::CheckEvents(){
  XEvent E;
  XNextEvent(RSGL::display, &E);
  type = E.type;
  if (type == 33 && E.xclient.data.l[0] == XInternAtom(RSGL::display, "WM_DELETE_WINDOW", True)){RSGL::quit = 33;}
  if (type == 4 || type == 5){button = E.xbutton.button;}
  if (type == 4 || type == 5 || type == 6){x=E.xbutton.x;y=E.xbutton.y;}
  if (type == 2 || type == 3){XQueryKeymap(RSGL::display,RSGL::keyboard);}
  else{RSGL::quit == 0;} 
}
