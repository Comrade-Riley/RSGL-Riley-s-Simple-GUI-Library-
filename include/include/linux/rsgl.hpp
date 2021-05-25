#pragma once
#include <X11/Xlib.h>
#include <iostream>
#include <math.h>
#include <X11/keysym.h>
#include <cstring>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "deps/CImg.h"
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "deps/stb_image_write.h" 
#include "deps/png++/image.hpp" 

#define STB_TRUETYPE_IMPLEMENTATION 
#include "deps/stb_truetype.h"

using namespace cimg_library;
#define RSGLRGBTOHEX(r, g, b) ((r << 16) + (g << 8) + b)

namespace RSGL{
    Display* display;
    GC gc;
    char keyboard[32];
    Window window;
    int screenNumber;
    int init();

    int Quit();

    void GetError();

    bool initPass = false;

    XEvent event;
    //page 189
    int KeyPressed=2;
    int KeyReleased=3;
    int MouseButtonPressed=4;
    int MouseButtonReleased=5;
    int MousePosChanged=6;
    /*int EnterNotify=7;
    int LeaveNotify=8;
    int FocusIn=9;
    int FocusOut=10;
    int KeymapNotify=11;
    int Expose=12;
    int GraphicsExpose=13;
    int NoExpose=14;
    int VisibilityNotify=15;
    int CreateNotify=16;
    int DestroyNotify=17;
    int UnmapNotify=18;
    int MapNotify=19;
    icnt MapRequest=20;
    /*
    int ReparentNotify=21;
    */
    int WindowConfigurationUpdated=22;
    int WindowPosChanged;
    int WindowSizeChanged;
    /*
    int ConfigureRequest=23;
    int GravityNotify=24;
    int ResizeRequest=25;
    int CirculateNotify=26;
    int CirculateRequest=27;
    int PropertyNotify=28;
    int SelectionClear=29;
    int SelectionRequest=30;
    int SelectionNotify=31;
    int ColormapNotify=32;
    */
    int quit = 33;
    /*int MappingNotify=34;
    int GenericEvent=35;
    int LASTEvent=36;*/

    struct point{int x, y;};
    struct rect{int x, y; int length,width; };
    struct circle{int x, y; int radius;};
    struct color{int r,g,b;};

    struct image{
      RSGL::rect r;
      RSGL::rect srcr = r; 
      const char* file; 
      std::vector<std::vector<int>> cords;
      XImage* image;
      png::image< png::rgba_pixel> img;
      Pixmap pixmap;
      GC gc;
      bool loaded = false;
    };

    struct text{
      RSGL::rect r; 
      std::vector<std::vector<int>> cords;
      std::vector<std::vector<int>> pixels;
      XImage* image;
      CImg<unsigned char> img;
      Pixmap pixmap;
      GC gc;
      RSGL::rect srcr = r; 
      RSGL::color c;
      RSGL::color sc;
      const char* text;
      const char* stext;
      bool loaded = false;
    };
    
    int clear(RSGL::rect r = {0,0,0,0}); //clears everything and redraws things defined below it

    int CircleCollidePoint(RSGL::circle c, RSGL::point p);
    int CircleCollideRect(RSGL::circle c, RSGL::rect r);
    int CircleCollide(RSGL::circle cir1,RSGL::circle cir2);
    int RectCollidePoint(RSGL::rect r, RSGL::point p);
    int RectCollideRect(RSGL::rect r, RSGL::rect r2);
    int PointCollide(RSGL::point p, RSGL::point p2);
    int ImageCollideRect(RSGL::image img, RSGL::rect r);
    int ImageCollideCircle(RSGL::image img, RSGL::circle c);
    int ImageCollidePoint(RSGL::image img, RSGL::point p);
    int ImageCollideImage(RSGL::image img, RSGL::image img2);
    
    
    struct music{Mix_Music* loaded; bool isLoaded=false;};

    music loadMusic(const char* file){return {Mix_LoadMUS(file),true};}

    image loadImage(const char* file, RSGL::rect r);

    int drawImage(RSGL::image image);

    int playMusic(RSGL::music m,int loop);

    text loadText(const char* word, RSGL::rect r, const char* font, RSGL::color c);

    int drawText(RSGL::text);
    
    bool isPressed(unsigned long key);
    int drawPoint(RSGL::point p, color c);

    
    int drawRect(RSGL::rect r,color c, bool fill=true){
      Pixmap pixmap = XCreatePixmap(RSGL::display,RSGL::window,r.width,r.length, DefaultDepth(RSGL::display, RSGL::screenNumber));
      XSetForeground(RSGL::display,RSGL::gc,RSGLRGBTOHEX(c.r,c.g,c.b));
      if (fill){XFillRectangle(RSGL::display,RSGL::window,RSGL::gc,r.x,r.y,r.width,r.length);}
      XDrawRectangle(RSGL::display,RSGL::window,RSGL::gc,r.x,r.y,r.width,r.length);
      return 1;
    }

    int drawCircle(RSGL::circle c, color col,bool fill=true){
      XSetForeground(RSGL::display,RSGL::gc,RSGLRGBTOHEX(col.r,col.g,col.b));
      if (fill){XFillArc(RSGL::display,RSGL::window,RSGL::gc, c.x-(30/2), c.y-(30/2), c.radius, c.radius, 0, 360*64);}
      XDrawArc(RSGL::display, RSGL::window, gc, c.x-(30/2), c.y-(30/2), c.radius, c.radius, 0, 360*64);
      return 1;
    }
};

int RSGL::init(){
    RSGL::initPass = true;
    display = XOpenDisplay(NULL);
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,1,2048);
    if (display == NULL){return 0;}
    screenNumber = DefaultScreen(display);
    return 1;
}

int RSGL::playMusic(RSGL::music m,int loop){
  if (!initPass){return 0;}
  if(Mix_PlayMusic(m.loaded, loop) == -1){return 0;}
  return 1;
}


RSGL::text RSGL::loadText(const char* word, RSGL::rect r, const char* font, RSGL::color c){
    /* load font file */
    long size;
    unsigned char* fontBuffer;
    
    FILE* fontFile = fopen(font, "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset */
    
    fontBuffer = (unsigned char*)malloc(size);
    
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);

    /* prepare font */
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontBuffer, 0))
    {
        printf("failed\n");
    }
    
    int b_w = r.width*4; /* bitmap width */
    int b_h = r.length*4; /* bitmap height */
    int l_h = r.length; /* line height */

    /* create a bitmap for the phrase */
    unsigned char* bitmap = (unsigned char*)calloc(b_w * b_h, sizeof(unsigned char));
    /* calculate font scaling */
    float scale = stbtt_ScaleForPixelHeight(&info, l_h);
    
    int x = 0;
       
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);
    
    int i;
    for (i = 0; i < strlen(word); ++i)
    {
        /* how wide is this character */
        int ax;
		int lsb;
        stbtt_GetCodepointHMetrics(&info, word[i], &ax, &lsb);

        /* get bounding box for character (may be offset to account for chars that dip above or below the line */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        
        /* compute y (different characters have different heights */
        int y = ascent + c_y1;
        //imagebitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);

        /* advance x */
        x += roundf(ax * scale);
        
        /* add kerning */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
        x += roundf(kern * scale);
    }
    stbi_write_png("out.text", b_w, b_h, 1, bitmap, b_w);
    std::vector<std::vector<int>> cords;
    std::vector<std::vector<int>> pixels;
    CImg<unsigned char> image("out.text"), visu(500,400,1,3,0);
    remove("out.text");
    image = image.resize(r.length,r.width,1,3);
    for (int h=0; h < image.height(); h++){
      for (int w=0; w < image.width(); w++){
        if ((int)image(w,h,0,0)+(int)image(w,h,0,1)+(int)image(w,h,0,2) > 0){
          cords.insert(cords.end(),{w,h});
          pixels.insert(pixels.end(),{(int)image(w,h,0,0), (int)image(w,h,0,1), (int)image(w,h,0,2)});
        }
      }
    }
    unsigned long valuemask = GCForeground | GCBackground | GCGraphicsExposures;
	  XGCValues		gcv;
    gcv.foreground = XWhitePixel(RSGL::display,RSGL::screenNumber);
	  gcv.background = XBlackPixel(RSGL::display,RSGL::screenNumber);
	  gcv.graphics_exposures = 0;
    Pixmap pixmap = XCreatePixmap(RSGL::display,RSGL::window,r.width,r.length, DefaultDepth(RSGL::display, RSGL::screenNumber));
    GC gc = XCreateGC(RSGL::display, pixmap,valuemask,&gcv);
    XImage* img = XGetImage(RSGL::display, RSGL::window, 0, 0 , r.length, r.width, AllPlanes, ZPixmap);
    for (int i=0;i < cords.size(); i++){
      XPutPixel(img,cords[i][0],cords[i][1],(65536 * c.r) + (256 * c.g) + c.b);           
    }
    XPutImage(RSGL::display, pixmap, gc, img, 0, 0, 0, 0, r.width, r.length);
    
    return {r,cords,pixels, img, image,pixmap,gc,r,c,c,word,word,true};
}



RSGL::image RSGL::loadImage(const char* file, RSGL::rect r){
  std::string command = "convert ";
  command += file; command += " -resize " + std::to_string(r.width) + "x" +std::to_string(r.length) + " out.png"; 
  system(command.data());
  png::image< png::rgba_pixel > image("out.png");
  system("rm out.png");
  std::vector<std::vector<int>> cords;
  XImage* img = XGetImage(RSGL::display, RSGL::window, 0, 0 , r.length, r.width, AllPlanes, ZPixmap);
  for (png::uint_16 y=0; y < image.get_height(); y++){
    for (png::uint_16 x=0; x < image.get_width(); x++){
        if(image[y][x].alpha >= 100){
          cords.insert(cords.end(),{(int)x+r.x,(int)y+r.y});
          XPutPixel(img,x,y,(65536 * image[y][x].red) + (256 * image[y][x].green) + image[y][x].blue);
        }
    }
  }
  Pixmap pixmap = XCreatePixmap(RSGL::display,RSGL::window,r.width,r.length, DefaultDepth(RSGL::display, RSGL::screenNumber));
  unsigned long valuemask = GCForeground | GCBackground | GCGraphicsExposures;
	XGCValues		gcv;
  gcv.foreground = XWhitePixel(RSGL::display,RSGL::screenNumber);
	gcv.background = XBlackPixel(RSGL::display,RSGL::screenNumber);
	gcv.graphics_exposures = 0;
  GC gc = XCreateGC(RSGL::display, pixmap, valuemask,&gcv);
  XPutImage(RSGL::display, pixmap, gc, img, 0, 0, 0,0, r.width,r.length);
  return {r,r,file, cords, img, image,pixmap,gc,true};
}

int RSGL::drawImage(RSGL::image image){
  if (image.srcr.width != image.r.width || image.srcr.length != image.r.length) image = RSGL::loadImage(image.file,image.r);

  XCopyArea(RSGL::display,image.pixmap,RSGL::window,image.gc,0,0,image.r.width,image.r.length,image.r.x,image.r.y);
  return 1;
}


int RSGL::Quit(){
    if (!RSGL::initPass){return 0;}
    if (!XCloseDisplay(display)){return 0;}
    return 1;
}


int RSGL::clear(RSGL::rect r){
  if (!RSGL::initPass){return 0;}
  if (r.x+r.y+r.length+r.width == 0){XClearWindow(RSGL::display,RSGL::window); return 1;}
  XClearArea(RSGL::display, RSGL::window, r.x, r.y, r.width, r.length, false);
  return 1;
}

bool RSGL::isPressed(unsigned long key) {
    KeyCode kc2 = XKeysymToKeycode(RSGL::display, key);
    bool isPressed = !!(RSGL::keyboard[kc2 >> 3] & (1 << (kc2 & 7)));
    return isPressed;
}



int RSGL::drawPoint(RSGL::point p, color c){
  XSetForeground(RSGL::display,RSGL::gc,RSGLRGBTOHEX(c.r,c.g,c.b));
  XDrawPoint(RSGL::display,RSGL::window,RSGL::gc,p.x,p.y);
  return 1;
}


struct Event{
  int type;
  int button;
  int x,y;
  void CheckEvents();
};

void Event::CheckEvents(){
  XEvent E;
  XNextEvent(RSGL::display, &E);
  type = E.type;
  if (type == 33 && E.xclient.data.l[0] == XInternAtom(RSGL::display, "WM_DELETE_WINDOW", true)){} else{type == 0;} 
  if (type == 4 || type == 5){button = E.xbutton.button;}
  if (type == 4 || type == 5 || type == 6){x=E.xbutton.x;y=E.xbutton.y;}
  if (type == 2 || type == 3){XQueryKeymap(RSGL::display,RSGL::keyboard);}
}


void RSGL::GetError(){
  if (!RSGL::initPass){std::cout << "RSGL::init() failed or was never used";}
}


int RSGL::drawText(RSGL::text t){
    RSGL::text image = t;
    if (image.srcr.width != image.r.width || image.srcr.length != image.r.length || (image.c.r+image.c.g+image.c.b) != (image.sc.r+image.sc.g+image.sc.b) || image.text  != image.stext){
      image.srcr.width = image.r.width; 
      image.srcr.length = image.r.length;
      image.stext = image.text;
      image.sc = image.c;
      image.pixmap = XCreatePixmap(RSGL::display,RSGL::window,image.r.width,image.r.length, DefaultDepth(RSGL::display, RSGL::screenNumber));
      XImage* img = XGetImage(RSGL::display, image.pixmap, 0, 0 , image.r.length, image.r.width, AllPlanes, ZPixmap);
      image.img = image.img.resize(image.r.length,image.r.width,1,3); //just incase the rect is changed
      for (int i=0;i < image.pixels.size(); i++){
        XPutPixel(image.image,image.cords[i][0],image.cords[i][1],(65536 * t.c.r) + (256 * t.c.g) + t.c.b);           
      }
      XPutImage(RSGL::display, image.pixmap, t.gc, image.image, 0, 0, 0, 0, image.r.width, image.r.length);
    }
    XCopyArea(RSGL::display,image.pixmap,RSGL::window,t.gc,0,0,image.r.width,image.r.length,image.r.x,image.r.y);
    return 1;
}
