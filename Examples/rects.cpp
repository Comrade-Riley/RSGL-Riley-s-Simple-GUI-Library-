#include "../RSGL.hpp"
#include <iostream>
bool running = true;

int main(){
    RSGL::init();
    RSGLWindow window = createWindow("name",500,500,500,500, {255,255,255});
    Event e;
    RSGL::color c = {255,0,0};
    RSGL::rect r = {100,100,50,50};
    while (running){
        e.CheckEvents();
        if (e.type == RSGL::quit){
            running = false;
        }
        RSGL::drawRect({200,200,25,25},{0,0,0},false);
        RSGL::drawRect(r,c);
    }
    RSGL::Quit();
} 