#include "../RSGL.hpp"
#include <iostream>
bool running = true;

int main(){
    RSGL::init();
    RSGLWindow window = createWindow("name",500,500,500,500, {255,255,255});
    Event e;
    RSGL::color c = {255,0,0};
    RSGL::circle cir = {50,100,100};
    while (running){
        e.CheckEvents();
        if (e.type == RSGL::quit){
            running = false;
        }
        RSGL::drawCircle({25,200,200},{0,0,0},false);
        RSGL::drawCircle(cir,c);
    }
    RSGL::Quit();
} 