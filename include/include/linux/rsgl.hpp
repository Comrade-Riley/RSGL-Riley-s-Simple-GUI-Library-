#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "deps/X11/Xlib.h"
#include "deps/X11/Xatom.h"
#include "deps/X11/cursorfont.h"
#include "deps/SDL2_mixer/SDL_mixer.h"
#include "deps/png++/image.hpp"                     


#ifndef VULKAN
    #ifndef OPENGL
      #ifndef RSX11
        #define OPENGL 1
      #endif
    #endif
#endif

#ifdef VULKAN
    #ifdef OPENGL
      #undef OPENGL
    #endif
#endif

#ifdef OPENGL
  #define CAIRO_HAS_PNG_FUNCTIONS 1
  #include "deps/cairo/cairo.h"
  #include "deps/cairo/cairo-xlib.h"
#endif

#define RSGLRGBTOHEX(r, g, b) ((r << 16) + (g << 8) + b)

namespace RSGL{
    #ifdef OPENGL
      cairo_surface_t *sfc;
      cairo_t *ctx;
    #endif
    const int KeyPressed=2;
    const int KeyReleased=3;
    const int MouseButtonPressed=4;
    const int MouseButtonReleased=5;
    const int MousePosChanged=6;
    const int quit = 33;
    const int dnd = 34;

    struct point{int x, y;};
    struct rect {int x, y; int length,width; };
    struct circle{int x, y; int radius;};
    struct area{int width,length;};
    struct color{int r,g,b; float a=1.0;};

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
      XImage* image;
      Pixmap pixmap;
      GC gc;
      RSGL::rect srcr = r; 
      RSGL::color c;
      RSGL::color sc;
      std::string text;
      std::string stext;
      bool loaded = false;
    };

    struct drawable{
      private:
        struct Event{
          int type;
          int button;
          int x,y;
        };
        char keyboard[32];
      public:
        RSGL::rect r;
        Display* display;
        Event event;
        Drawable d;
    };

    struct pixmap : drawable{
        pixmap(){}
        pixmap(RSGL::drawable dr,RSGL::area a);
    };

    struct window : drawable{
      private:
        struct Event{
          int type;
          int button;
          int x,y;
        };
        char keyboard[32];
      public:
        std::string name;
        RSGL::color color;
        RSGL::pixmap dbuffer; //double buffer
        Event event;

        int setColor(RSGL::color c);
        window(){}
        window(std::string,RSGL::rect, RSGL::color, bool resize = false);
        void CheckEvents();
        bool isPressed(unsigned long key);
        void close();
    };
    
    window root;
    
    int clear(RSGL::rect r = {0,0,0,0},RSGL::drawable win=root); //clears everything and redraws things defined below it

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
  
    image loadImage(const char* file, RSGL::rect r, RSGL::drawable win=root);

    int drawImage(RSGL::image image,RSGL::drawable win=root);

    text loadText(std::string word, RSGL::rect r, std::string font, RSGL::color c,RSGL::drawable win=root);

    int drawText(RSGL::text,RSGL::drawable win=root);
    
    bool isPressed(unsigned long key);
    int drawPoint(RSGL::point p, color c,RSGL::drawable win=root);

    
    int drawRect(RSGL::rect r,color c, bool fill=True,int stroke=1, int lineColor = 0, RSGL::color lineCol = {}, RSGL::drawable win=root);

    int drawCircle(RSGL::circle c, color col,bool fill=true,int stroke=1, int lineColor = 0, RSGL::color lineCol = {},RSGL::drawable win=root);


    struct rectButton{
      rect r;
      color c;
      text t;
      RSGL::drawable d;
      void draw(RSGL::drawable win=root){drawRect(r,c); if (t.pixmap) drawText(t); d=win;}
      bool isClicked(){return d.event.type == MouseButtonReleased && RectCollidePoint(r,{d.event.x,d.event.y});}
      bool isHovered(){return RectCollidePoint(r,{d.event.x,d.event.y});}
      bool isPressed(){return d.event.type == MouseButtonPressed && RectCollidePoint(r,{d.event.x,d.event.y});}
    };

    struct circleButton{
      circle c;
      color col;
      text t;
      RSGL::drawable d;
      void draw(RSGL::drawable win=root){drawCircle(c,col); if (t.pixmap) drawText(t); d=win;}
      bool isClicked(){return d.event.type == MouseButtonReleased && CircleCollidePoint(c,{d.event.x,d.event.y});}
      bool isHovered(){return CircleCollidePoint(c,{d.event.x,d.event.y});}
      bool isPressed(){return d.event.type == MouseButtonPressed && CircleCollidePoint(c,{d.event.x,d.event.y});}
    };

    struct imgButton{
      image img;
      text t;
      RSGL::drawable d;
      void draw(RSGL::drawable win=root){drawImage(img); if (t.pixmap) drawText(t); d=win;}
      bool isClicked(){return d.event.type == MouseButtonReleased && ImageCollidePoint(img,{d.event.x,d.event.y});}
      bool isHovered(){return ImageCollidePoint(img,{d.event.x,d.event.y});}
      bool isPressed(){return d.event.type == MouseButtonPressed && ImageCollidePoint(img,{d.event.x,d.event.y});}
    };

    std::vector<std::string> fileDialog(std::string title,bool multiple=false,bool save=false, bool directory=false);
    void notifiy(std::string title, std::string content,std::string image="");
    void messageBox(std::string message, bool question=false,bool error=false);
#ifndef RSGLNAMESPACEEXTENTION
};
#endif