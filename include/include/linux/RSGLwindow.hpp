#pragma once
#include <iostream>
#include "rsgl.hpp"

struct RSGLWindow{
    Window window;
    std::string name;
    
    int x;
    int y;
    int w;
    int h;
    RSGL::color color;
    int setColor(RSGL::color c){
        color = c;
        XSetWindowBackground(RSGL::display,window,RSGLRGBTOHEX(c.r,c.g,c.b));
        return 1;
    }
};

RSGLWindow createWindow(std::string name,int x1, int y1,int w1, int h1, RSGL::color c){
    //extern XVisualInfo* vi = glXChooseVisual(RSGL::display, 0, 0);
    RSGL::window = XCreateSimpleWindow(RSGL::display, RootWindow(RSGL::display, RSGL::screenNumber), x1, y1, w1, h1, 0,0,  RSGLRGBTOHEX(c.r,c.g,c.b));
    long event_mask = ExposureMask
                    | KeyPressMask
                    | KeyReleaseMask
                    | ButtonPressMask
                    | ButtonReleaseMask
                    | FocusChangeMask
                    | PointerMotionMask
                    | SubstructureNotifyMask
                    | StructureNotifyMask
                    | ExposureMask;
    XSelectInput(RSGL::display, RSGL::window, event_mask);
    XMapWindow(RSGL::display, RSGL::window);
    if (!RSGL::window){
        return {};
    }
    XStoreName(RSGL::display, RSGL::window, name.data());
    XNextEvent(RSGL::display, &RSGL::event);
    Atom wm_delete = XInternAtom(RSGL::display, "WM_DELETE_WINDOW", true);
    XSetWMProtocols(RSGL::display, RSGL::window, &wm_delete, 1);
    XGCValues values;
    RSGL::gc = DefaultGC(RSGL::display, 0);

    #ifdef OPENGL
    RSGL::sfc = cairo_xlib_surface_create(RSGL::display, RSGL::window,
        DefaultVisual(RSGL::display,XDefaultScreen(RSGL::display)), w1, h1);
    cairo_xlib_surface_set_size(RSGL::sfc, w1, h1);
    RSGL::ctx = cairo_create(RSGL::sfc);
    #endif
    
    return {RSGL::window,name,x1,x1,w1,w1,.color=c};
}
