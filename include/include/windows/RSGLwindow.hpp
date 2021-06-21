#pragma once
#include "rsgl.hpp"


LRESULT CALLBACK winproc(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp){
    switch(wm){
        case 0x0010: RSGL::dev::eventPass = 1; break; 
        case 0x0100: RSGL::dev::eventPass = 2; break;
        case 0x0101: RSGL::dev::eventPass = 3; break;
        case 0x0201: RSGL::dev::eventPass = 5; RSGL::dev::rawEventPass= 0x0201; break;
        case 0x0204: RSGL::dev::eventPass = 5; RSGL::dev::rawEventPass= 0x0204; break;
        case 0x0202: RSGL::dev::eventPass = 6; RSGL::dev::rawEventPass= 0x0202; break;
        case 0x0205: RSGL::dev::eventPass = 6; RSGL::dev::rawEventPass= 0x0205; break;
        case 0x0207: RSGL::dev::eventPass = 5; RSGL::dev::rawEventPass= 0x0207; break;
        case 0x0208: RSGL::dev::eventPass = 6; RSGL::dev::rawEventPass= 0x0208; break;
        case 0x0003: RSGL::dev::eventPass = 44; break;
        case WM_PAINT:  RSGL::dev::livewindow = hwnd;  break; 
        default: break;
    }
    return DefWindowProc(hwnd, wm, wp, lp);
}

RSGL::window createWindow(std::string name,int x, int y,int w, int h, RSGL::color c){
    WNDCLASSW wc = { };
 
    wc.lpszClassName = (LPCWSTR)name.data();
    wc.hInstance     = NULL;
    wc.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wc.lpfnWndProc   = winproc;
    wc.hCursor       = LoadCursor (NULL, IDC_ARROW);

    if (!RegisterClassW(&wc)) {
        MessageBox(NULL, TEXT("Could not register window class"), 
                  NULL, MB_ICONERROR);
    }
  
    RSGL::dev::window = CreateWindowW(wc.lpszClassName, (LPCWSTR)name.data(),
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  x,y, w, h, 0, 0, NULL, 0);
    if (!RSGL::dev::window) {
        MessageBox(NULL, TEXT("Could not create window"), NULL, MB_ICONERROR);
    }

    ShowWindow(RSGL::dev::window, 6);
    UpdateWindow(RSGL::dev::window);


    return {RSGL::dev::window,name,x,y,w,h,c};
}


