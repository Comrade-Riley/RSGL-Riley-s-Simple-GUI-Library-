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
    #ifdef OPENGL
      cairo_surface_t *sfc;
      cairo_t *ctx;
    #endif
    
    int init();

    int Quit();

    void GetError();

    bool initPass = false;

    XEvent event;
    const int KeyPressed=2;
    const int KeyReleased=3;
    const int MouseButtonPressed=4;
    const int MouseButtonReleased=5;
    const int MousePosChanged=6;
    const int quit = 33;
    const int dnd = 34;

    struct point{int x, y;};
    struct rect{int x, y; int length,width; };
    struct circle{int x, y; int radius;};
    struct color{int r,g,b; float a;};

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
    
    
    struct music{Mix_Music* loaded; bool isLoaded=false;std::string title;};

    music loadMusic(std::string file){return {Mix_LoadMUS(file.data()),true,file};}

    image loadImage(const char* file, RSGL::rect r);

    int drawImage(RSGL::image image);

    int playMusic(RSGL::music m,int loop);
    void pauseMusic();
    void resumeMusic();
    void stopMusic();

    bool isPlaying(RSGL::music m);


    text loadText(const char* word, RSGL::rect r, const char* font, RSGL::color c);

    int drawText(RSGL::text);
    
    bool isPressed(unsigned long key);
    int drawPoint(RSGL::point p, color c);

    
    int drawRect(RSGL::rect r,color c, bool fill=True,int stroke=1, int lineColor = 0, RSGL::color lineCol = {});

    int drawCircle(RSGL::circle c, color col,bool fill=true,int stroke=1, int lineColor = 0, RSGL::color lineCol = {});


    struct rectButton{
      RSGL::rect r;
      RSGL::color c;
      RSGL::text t;
      void draw(){RSGL::drawRect(r,c); if (t.pixmap) RSGL::drawText(t);}
      bool isClicked(){return RSGL::event.type == RSGL::MouseButtonReleased && RSGL::RectCollidePoint(r,{RSGL::event.xbutton.x,RSGL::event.xbutton.y});}
      bool isHovered(){return RSGL::RectCollidePoint(r,{RSGL::event.xbutton.x,RSGL::event.xbutton.y});}
      bool isPressed(){return RSGL::event.type == RSGL::MouseButtonPressed && RSGL::RectCollidePoint(r,{RSGL::event.xbutton.x,RSGL::event.xbutton.y});}
    };

    struct circleButton{
      RSGL::circle c;
      RSGL::color col;
      RSGL::text t;
      void draw(){RSGL::drawCircle(c,col); if (t.pixmap) RSGL::drawText(t);}
      bool isClicked(){return RSGL::event.type == RSGL::MouseButtonReleased && RSGL::CircleCollidePoint(c,{RSGL::event.xbutton.x,RSGL::event.xbutton.y});}
      bool isHovered(){return RSGL::CircleCollidePoint(c,{RSGL::event.xbutton.x,RSGL::event.xbutton.y});}
      bool isPressed(){return RSGL::event.type == RSGL::MouseButtonPressed && RSGL::CircleCollidePoint(c,{RSGL::event.xbutton.x,RSGL::event.xbutton.y});}
    };

    struct imgButton{
      RSGL::image img;
      RSGL::text t;
      void draw(){RSGL::drawImage(img); if (t.pixmap) RSGL::drawText(t); }
      bool isClicked(){return RSGL::event.type == RSGL::MouseButtonReleased && ImageCollidePoint(img,{RSGL::event.xbutton.x,RSGL::event.xbutton.y});}
      bool isHovered(){return RSGL::ImageCollidePoint(img,{RSGL::event.xbutton.x,RSGL::event.xbutton.y});}
      bool isPressed(){return RSGL::event.type == RSGL::MouseButtonPressed && ImageCollidePoint(img,{RSGL::event.xbutton.x,RSGL::event.xbutton.y});}
    };
    
    struct circleSliderThingy{
      RSGL::circle c;
      RSGL::color dotColorPos1;
      RSGL::color sliderColorPos1;
      RSGL::color dotColorPos2;
      RSGL::color sliderColorPos2;
      RSGL::circleButton cb = {c};
      int pos = 1; 
      void draw(); 
    };


    std::vector<std::string> fileDialog(std::string title,bool multiple=false,bool save=false, bool directory=false);
    void notifiy(std::string title, std::string content,std::string image="");
    void messageBox(std::string message, bool question=false,bool error=false);
};



void RSGL::notifiy(std::string title, std::string content ,std::string image){
  std::string com = "notify-send \"" + title +"\" \"" + content + "\" ";
  if (image != "")  com += "-i \"" + image + "\"";
  popen(com.data(),"r");
}

void RSGL::messageBox(std::string message,bool question,bool error){
  std::string com = "zenity ";
  if (question) com+="--question "; else if (error) com+= "error"; else com+="--warning ";
  com += "--text \"" + message + "\"";
  std::cout << com.data();
  popen(com.data(),"r");
}

std::vector<std::string> RSGL::fileDialog(std::string title,bool multiple,bool save, bool directory){
  char filename[1024];
  std::string com="zenity --file-selection --title=\""+title+"\"";
  if (multiple) com += " --multiple --separator=\" \"";
  if (directory) com += " --directory";
  if (save) com += " --save";
  FILE *f = popen(com.data(), "r");
  fgets(filename, 1024, f);
  std::string fn(filename);
  if (multiple){
    std::vector<std::string> output;
    std::string file;
    for (int i=0; i < fn.size(); i++){
      if (fn[i] == ' '){ output.insert(output.end(),file); file="";}
      else file+=fn[i];
    }
  }
  return {fn};
}



void RSGL::circleSliderThingy::draw(){
    if (cb.isPressed()){ if (pos == 1) pos=2; else pos=1; }  
    cb.draw();
    if (pos == 1){ 
      cb.c = c; cb.col = dotColorPos1;
      RSGL::drawCircle(c,sliderColorPos1);
      RSGL::drawCircle({c.x+(c.radius/2),c.y,c.radius},sliderColorPos1);
      RSGL::drawCircle({c.x+(c.radius/2)*2,c.y,c.radius},sliderColorPos1);
    }
    else{ 
      cb.c = {c.x*3,c.y,c.radius}; cb.col = dotColorPos2;
      RSGL::drawCircle(c,sliderColorPos2);
      RSGL::drawCircle({c.x+(c.radius/2),c.y,c.radius},sliderColorPos2);
      RSGL::drawCircle({c.x+(c.radius/2)*2,c.y,c.radius},sliderColorPos2);
    }
}

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
  if(Mix_PlayMusic(m.loaded, loop) == -1) return 0;
  return 1;
}

void RSGL::pauseMusic(){Mix_PauseMusic();}
void RSGL::resumeMusic(){Mix_ResumeMusic();}
void RSGL::stopMusic(){Mix_HaltMusic();}

bool RSGL::isPlaying(RSGL::music m){return Mix_PlayingMusic(); }

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
    if (!stbtt_InitFont(&info, fontBuffer, 0)) std::cout << "failed to load font\n";
    r.width = sizeof(word)*15;
    int b_w = r.width; /* bitmap width */;
    int b_h = r.length; /* bitmap height */
    int l_h = r.length; /* line height */

    /* create a bitmap for the phrase */
    unsigned char* bitmap = (unsigned char*)calloc(b_w * b_h, sizeof(unsigned char));
    memset(bitmap,0,b_w * b_h);
    
    /* calculate font scaling */
    float scale = stbtt_ScaleForPixelHeight(&info, l_h);
    
    int x = 0;
       
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    
    ascent = roundf(ascent * scale); descent = roundf(descent * scale);
    
    for (int i = 0; i < strlen(word); ++i){
        /* how wide is this character */
        int ax; int lsb;
        stbtt_GetCodepointHMetrics(&info, word[i], &ax, &lsb);

        /* get bounding box for character (may be offset to account for chars that dip above or below the line */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        
        /* compute y (different characters have different heights */
        int y = ascent + c_y1;
        
        /* render character (stride and offset is important here) */
        int byteOffset = x + roundf(lsb * scale) + (y * b_w);
        stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);

        /* advance x */
        x += roundf(ax * scale);
        
        /* add kerning */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
        x += roundf(kern * scale);
    }
    unsigned long valuemask = GCForeground | GCBackground | GCGraphicsExposures;
	  XGCValues		gcv;
    gcv.foreground = XWhitePixel(RSGL::display,RSGL::screenNumber);
	  gcv.background = XBlackPixel(RSGL::display,RSGL::screenNumber);
	  gcv.graphics_exposures = 0;
    Pixmap pixmap = XCreatePixmap(RSGL::display,RSGL::window,sizeof(word)*5, b_h, DefaultDepth(RSGL::display, RSGL::screenNumber));
    GC gc = XCreateGC(RSGL::display, pixmap,valuemask,&gcv);
    XImage* img = XGetImage(RSGL::display, RSGL::window, 0, 0 , b_w, b_h, AllPlanes, ZPixmap);    
    memcpy(img->data, bitmap,b_h*b_w); free(bitmap);
    
    XPutImage(RSGL::display, pixmap, gc, img, 0, 0, 0, 0, sizeof(word)*5, b_h);
    return {r,img,pixmap,gc,r,c,c,word,word,true};
}

RSGL::image RSGL::loadImage(const char* file, RSGL::rect r){
  std::string command = "convert ";
  command += file; command += " -resize " + std::to_string(r.width) + "x" +std::to_string(r.length) + " out.png"; 
  system(command.data());
  
  png::image< png::rgba_pixel > image("out.png");
  system("rm out.png");
  std::vector<std::vector<int>> cords;
  XImage* img = XGetImage(RSGL::display, RSGL::window, 0, 0 , r.length, r.width, AllPlanes, ZPixmap);
  char* data;
  
  for (png::uint_16 y=0; y < image.get_height(); y++){
    for (png::uint_16 x=0; x < image.get_width(); x++){
        if(image[y][x].alpha >= 100){
          cords.insert(cords.end(),{(int)x+r.x,(int)y+r.y});
          XPutPixel(img,x,y,(65536 * image[y][x].red) + (256 * image[y][x].green) + image[y][x].blue);
        }
    }
  }
  
  Pixmap pixmap = XCreatePixmap(RSGL::display,RSGL::window,r.width,r.length, DefaultDepth(RSGL::display, RSGL::screenNumber));
  //Pixmap pixmap2 = XCreateBitmapFromData(RSGL::display,RSGL::)
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
    cairo_surface_destroy(RSGL::sfc);
    cairo_destroy(RSGL::ctx);
    if (!XCloseDisplay(display)){return 0;}
    return 1;
}


int RSGL::clear(RSGL::rect r){
  #ifdef OPENGL
    //cairo_push_group(RSGL::ctx);
    cairo_set_source_rgb(RSGL::ctx,1,1,1);
    cairo_paint(RSGL::ctx);
    cairo_set_source_rgb(RSGL::ctx,255,255,255);

    //cairo_pop_group_to_source(RSGL::ctx);
    cairo_paint(RSGL::ctx);
    cairo_surface_flush(RSGL::sfc);
  #endif
  return 1;
}

bool RSGL::isPressed(unsigned long key) {
    KeyCode kc2 = XKeysymToKeycode(RSGL::display, key);
    bool isPressed = !!(RSGL::keyboard[kc2 >> 3] & (1 << (kc2 & 7)));
    return isPressed;
}


int RSGL::drawRect(RSGL::rect r,color c, bool fill,int stroke, int lineColor, RSGL::color lineCol){
      if (!lineColor) lineCol = c;
      #ifdef OPENGL
        cairo_set_source_rgba(RSGL::ctx, (double)lineCol.r,(double)lineCol.b,(double)lineCol.g,(double)lineCol.a);
        cairo_rectangle(RSGL::ctx, (float)r.x,(float)r.y, (float)r.width,(float)r.length);
        cairo_set_line_width(RSGL::ctx, stroke);
        cairo_stroke(RSGL::ctx);
        if (fill){
          cairo_rectangle(RSGL::ctx, (float)r.x,(float)r.y, (float)r.width,(float)r.length);
          cairo_set_source_rgba(RSGL::ctx, (double)c.r,(double)c.b,(double)c.g,(double)c.a);
          cairo_fill(RSGL::ctx);
        }
      #endif
      return 1;
}

int RSGL::drawCircle(RSGL::circle c, color col,bool fill,int stroke, int lineColor, RSGL::color lineCol){
  if (!lineColor) lineCol = col;
    #ifdef OPENGL    
    cairo_set_source_rgba(RSGL::ctx, (double)lineCol.r,(double)lineCol.g,(double)lineCol.b,(double)lineCol.a);
    cairo_arc(RSGL::ctx,c.x,c.y,c.radius,0,2 * M_PI);
    cairo_set_line_width(RSGL::ctx, stroke);
    cairo_stroke(RSGL::ctx);
    if (fill){
      cairo_set_source_rgba(RSGL::ctx, (double)col.r,(double)col.g,(double)col.b,(double)col.a);
      cairo_arc(RSGL::ctx, c.x,c.y, c.radius,0, 2 * M_PI);
      cairo_stroke_preserve(RSGL::ctx);
      cairo_set_source_rgba(RSGL::ctx, (double)col.r,(double)col.g,(double)col.b,(double)col.a);
      cairo_fill(RSGL::ctx);
      cairo_set_source_rgba(RSGL::ctx, (double)col.r,(double)col.g,(double)col.b,(double)col.a);
    }
  #endif
  return 1;
}


int RSGL::drawPoint(RSGL::point p, color c){
  #ifdef OPENGL
      cairo_set_source_rgba(RSGL::ctx, (double)c.r,(double)c.b,(double)c.g,(double)c.a);
      cairo_rectangle(RSGL::ctx, (float)p.x,(float)p.y, 1,1);
      cairo_fill_preserve(RSGL::ctx);
  #endif
  return 1;
}


struct Event{
  int type;
  int button;
  int x,y;
  void CheckEvents();
};

void Event::CheckEvents(){
  XNextEvent(RSGL::display, &RSGL::event);
  XEvent E = RSGL::event;
  type = RSGL::event.type;
  if (type == 33 && E.xclient.data.l[0] == XInternAtom(RSGL::display, "WM_DELETE_WINDOW", true)){} 
  else if (type == 33 && E.xclient.message_type == XInternAtom(RSGL::display, "XdndDrop", false)){type=34;}  else{type = 0;} 
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

    }
    XCopyArea(RSGL::display,image.pixmap,RSGL::window,t.gc,0,0,image.r.width,image.r.length,image.r.x,image.r.y);
    return 1;
}
