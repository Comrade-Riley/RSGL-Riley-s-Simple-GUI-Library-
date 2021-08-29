#include "RSGL.hpp"

bool running = true;

int main(){
    RSGL::window window("name",{500,500,500,500},{255,255,255});
    while (running){
        window.CheckEvents();
        switch(window.event.type){
            case RSGL::quit: running = false;
        }
    }
    window.close();
} 