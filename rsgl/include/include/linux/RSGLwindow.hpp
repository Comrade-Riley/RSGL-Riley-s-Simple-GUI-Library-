#pragma once
#include <X11/Xlib.h>
//#include <iostream>
#include "rsgl.hpp"

struct RSGLWindow{
    Window window;
    const char* name;
    
    int x;
    int y;
    int w;
    int h;

    int setColor(RSGL::color c){
        XSetWindowBackground(RSGL::display,window,RSGLRGBTOHEX(c.r,c.g,c.b));
        return 1;
    }
};


RSGLWindow createWindow(const char* name,int x1, int y1,int w1, int h1, RSGL::color c){
    //Opens new connection to X server

    RSGL::window = XCreateSimpleWindow(RSGL::display, RootWindow(RSGL::display, RSGL::screenNumber), x1, y1, w1, h1, 1,
                                 BlackPixel(RSGL::display, RSGL::screenNumber),  RSGLRGBTOHEX(c.r,c.g,c.b));
    long event_mask = ExposureMask
                    | KeyPressMask
                    | KeyReleaseMask
                    | ButtonPressMask
                    | ButtonReleaseMask
                    | FocusChangeMask
                    | PointerMotionMask
                    | SubstructureNotifyMask
                    | StructureNotifyMask;
    XSelectInput(RSGL::display, RSGL::window, event_mask);
    XMapWindow(RSGL::display, RSGL::window);
    if (!RSGL::window){
        return {};
    }
    XStoreName(RSGL::display, RSGL::window, name);
    XNextEvent(RSGL::display, &RSGL::event);
    Atom wm_delete = XInternAtom(RSGL::display, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(RSGL::display, RSGL::window, &wm_delete, 1);
    XGCValues values;
    RSGL::gc = DefaultGC(RSGL::display, 0);
    
    return {RSGL::window,name,x1,x1,w1,w1};
}
