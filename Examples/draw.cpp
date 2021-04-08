#include "../RSGL.hpp"
#include <vector>
#include <iostream>
bool running = true;

std::vector<RSGL::circle> dots = {};

int main(){
    RSGL::init();
    RSGLWindow window = createWindow("name",500,500,500,500, {255,255,255});
    Event e;
    RSGL::circle cir = {50,100,100};
    bool pressed = false;
    while (running){
        e.CheckEvents();
        if (e.type == RSGL::quit){
            running = false;
        }
        if (e.type == RSGL::MouseButtonPressed){pressed = true;}
        if (e.type == RSGL::MouseButtonReleased){pressed = false;}
        if (pressed && e.button == 1){dots.insert(dots.end(),{25,e.x,e.y});}
        for (int i = 0; i < dots.size(); i++){
            RSGL::drawCircle({25,dots[i].x,dots[i].y},{0,0,0});
        }
    }
    RSGL::Quit();
} 