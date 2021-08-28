#include "../RSGL.hpp"
#include <vector>
#include <iostream>
#include <pthread.h>
bool running = true;
bool pressed = true;

RSGL::color win[500*500];
 
std::vector<RSGL::circle> dots;
char* data = (char*)malloc(500*500*3);
Pixmap pixmap;
XImage*img;
void* draw(void*){
    for (int y=0; y < 370; y++){
        for (int x=0; x < 500; x++){
            XPutPixel(img,x,y,(65536 * win[x*y].r) + (256 * win[x*y].g) + win[x*y].b);
            //RSGL::drawPoint({x,y},win[x*y]);
        }
    }
    XPutImage(RSGL::display,pixmap,RSGL::gc,img,0,0,0,0,500,500);
    XCopyArea(RSGL::display,pixmap,RSGL::window,RSGL::gc,0,0,500,500,0,0);
    return NULL;
}

void drawRect(RSGL::rect r, RSGL::color c){
    for (int y=r.y; y < r.length+r.y; y++){
        for (int x=r.x; x < r.width+r.x; x++){
           win[x*y] = c;
        }
    }
}

int main(){
    pthread_t p;
    RSGL::init();
    RSGLWindow window = createWindow("name",500,500,500,500, {255,255,255}); 
    pixmap = XCreatePixmap(RSGL::display,RSGL::window,500,500, DefaultDepth(RSGL::display, RSGL::screenNumber));
    img = XCreateImage(RSGL::display,XDefaultVisual(RSGL::display,XDefaultScreen(RSGL::display)),XDefaultDepth(RSGL::display,XDefaultScreen(RSGL::display)),
        ZPixmap,32,data,500,500,32,0);
    Event e; 
    while (running){
        //cairo_push_group(RSGL::ctx);
        //RSGL::clear();
        e.CheckEvents();
        switch(e.type){
            case RSGL::quit: running = false; break;
            case RSGL::MouseButtonPressed: pressed = true;  break;
            case RSGL::MouseButtonReleased: pressed = false; break;
            default: break;
        }
        if (pressed && e.button == 1)  dots.insert(dots.end(),{e.x,e.y,25});
        //pthread_create(&p,NULL,draw,NULL);
        drawRect({200,200,100,100},{255,0,0,1.0});
        draw(NULL);
        //cairo_surface_flush(RSGL::sfc);
        //
    }
    RSGL::Quit();
} 