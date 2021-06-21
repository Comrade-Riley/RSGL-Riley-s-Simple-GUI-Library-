#pragma once
#include <windows.h>
#include <iostream>

namespace RSGL{
    struct rect{int x,y; int length, width;}; 
    struct point{int x,y;};
    struct circle{int x,y; int radius;};
    struct color{int r,g,b;};

    int drawRect(RSGL::rect r, RSGL::color c, bool fill=true); 
    int drawCircle(RSGL::circle c, color col, bool fill=true);
    int drawPoint(RSGL::point p, RSGL::color c);

    int setBackground(RSGL::color c);

    const int quit = 1;
    const int KeyPressed=2;
    const int KeyReleased=3;
    const int MousePosChanged=4;
    int MouseButtonPressed=5;
    int MouseButtonReleased=6;

    bool isPressed(int key);

    int init();
    bool isPressed(int key);

    struct window{HWND window; std::string name;
        int x; int y; int w;int h;
        RSGL::color color;};
};

namespace RSGL::dev{
    HWND window;
    int eventPass;
    int rawEventPass;
    MSG  msg; 
    HWND livewindow;
}

bool RSGL::isPressed(int key) {
    return GetKeyState(key);
}


int RSGL::drawRect(RSGL::rect r, RSGL::color c, bool fill){
   PAINTSTRUCT ps;
   HDC hDC = BeginPaint(RSGL::dev::livewindow, &ps);
   
   HPEN hpenOld = static_cast<HPEN>(SelectObject(hDC, GetStockObject(DC_PEN)));
   HBRUSH hbrushOld = static_cast<HBRUSH>(SelectObject(hDC, GetStockObject(NULL_BRUSH)));

   RECT rc1;
   rc1.left = r.x; rc1.top =  r.y;
   rc1.right  = r.width; rc1.bottom = r.length;

   SetDCPenColor(hDC, RGB(c.r, c.g, c.b)); 
   Rectangle(hDC, rc1.left, rc1.top, rc1.right, rc1.bottom);
   RSGL::dev::window = RSGL::dev::window;
} 


struct Event{
  int type;
  int button;
  int x,y;
  void CheckEvents();
};

void Event::CheckEvents(){
    if(GetMessage(&RSGL::dev::msg, NULL, 0, 0)){
        TranslateMessage(&RSGL::dev::msg);
        DispatchMessage(&RSGL::dev::msg);
    }
    type = RSGL::dev::eventPass;
    button = 0;
    if (type == 5 || type == 6){
        switch(RSGL::dev::rawEventPass){
            case 0x0201: button = 1; break;
            case 0x0202: button = 1; break;
            case 0x0207: button = 2; break;
            case 0x0208: button = 2; break;
            case 0x0204: button = 3; break;
            case 0x0205: button = 3; break;
            default: break;
        }
    }
    POINT Point; RECT Rect;
    GetCursorPos(&Point); GetWindowRect(RSGL::dev::window, &Rect);
    Point.x -= Rect.left; Point.y -= Rect.top;
    if (type == 44 && x != Point.x && y != Point.y){
        type = 4;
        x = Point.x;
        y = Point.y;
    }
}
