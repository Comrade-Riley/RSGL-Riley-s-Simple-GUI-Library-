#include "RSGL.hpp"
#include <pthread.h>

bool running = true;

RSGL::rect grect; RSGL::color gcolor;
//threads
pthread_t t;

RSGL::window window("name",{500,500,500,500},{255,255,255});
RSGL::pixmap dbuffer(window,{500,500});

void* rectLoop(void*){
    RSGL::drawRect(grect,gcolor,true,1,0,{},dbuffer);
    return NULL;
}

void drawRect(RSGL::rect r, RSGL::color c){
    grect=r; gcolor=c; 
    pthread_create(&t,NULL,rectLoop,NULL);
}
 
int main(){
    while (running){
        window.CheckEvents();
        switch(window.event.type){
            case RSGL::quit: running = false;
        }
        drawRect({200,200,200,200},{200,0,0});
    }
    window.close();
} 