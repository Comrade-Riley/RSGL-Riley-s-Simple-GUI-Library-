#include <SFML/Audio.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../include/include/linux/deps/stb_image_write.h" 
#define STB_TRUETYPE_IMPLEMENTATION 
#include "../../include/include/linux/deps/stb_truetype.h"
#include "../../include/include/linux/rsgl.hpp"
#include "../../include/include/linux/deps/X11/Xutil.h"
#include <pthread.h>

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


RSGL::text RSGL::loadText(std::string word, RSGL::rect r, std::string font, RSGL::color c,RSGL::drawable win){
    Display* display;
    
    /* load font file */
    int size;
    unsigned char* fontBuffer;
    
    FILE* fontFile = fopen(font.data(), "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset */
    
    fontBuffer = (unsigned char*)malloc(size);
    
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);

    /* prepare font */
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontBuffer, 0)) std::cout << "failed to load font\n";
    r.width = (int)word.size()*25.6;
    //r.width = word.size()*r.length;
    int b_w = r.width; /* bitmap width */;
    int b_h = r.length; /* bitmap height */
    int l_h = r.length; /* line height */

    /* create a bitmap for the phrase */
    unsigned char* bitmap = (unsigned char*)calloc(b_w * b_h, sizeof(unsigned char));
    memset(bitmap,0xFF,b_w * b_h*sizeof(unsigned char));
    
    /* calculate font scaling */
    float scale = stbtt_ScaleForPixelHeight(&info, l_h);
    
    int x = 0;
       
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    
    ascent = roundf(ascent * scale); descent = roundf(descent * scale);
    
    for (int i = 0; i < word.size(); ++i){
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
    int len;    
    std::ifstream stream;

    png::image< png::rgba_pixel > image(stream);
    
    
    
    unsigned long valuemask = GCForeground | GCBackground | GCGraphicsExposures;
	  XGCValues		gcv;
    int screenNumber = XDefaultScreen(win.display);
    gcv.foreground = XWhitePixel(display,screenNumber);
	  gcv.background = XBlackPixel(display,screenNumber);
	  gcv.graphics_exposures = 0;
    Pixmap pixmap = XCreatePixmap(display,win.d,sizeof(word)*5, b_h, DefaultDepth(win.display, screenNumber));
    GC gc = XCreateGC(display, pixmap,valuemask,&gcv);
    XImage* img = XGetImage(display, win.d, 0, 0 , b_w, b_h, AllPlanes, ZPixmap);    
    memcpy(img->data, bitmap,b_h*b_w); free(bitmap);
    for (int y=0; y < b_h; y++){
      for (int x=0; x < b_w; x++) std::cout << img->data[x*y] << std::endl;
    }
    XPutImage(display, pixmap, gc, img, 0, 0, 0, 0, sizeof(word)*5, b_h);
    return {r,img,pixmap,gc,r,c,c,word,word,true};
}

RSGL::image RSGL::loadImage(const char* file, RSGL::rect r,RSGL::drawable win){
  std::string command = "convert ";
  command += file; command += " -resize " + std::to_string(r.width) + "x" +std::to_string(r.length) + " out.png"; 
  system(command.data());
  
  png::image< png::rgba_pixel > image("out.png");
  system("rm out.png");
  std::vector<std::vector<int>> cords;
  XImage* img = XGetImage(win.display, root.d, 0, 0 , r.length, r.width, AllPlanes, ZPixmap);
  char* data;
  
  for (png::uint_16 y=0; y < image.get_height(); y++){
    for (png::uint_16 x=0; x < image.get_width(); x++){
        if(image[y][x].alpha >= 100){
          cords.insert(cords.end(),{(int)x+r.x,(int)y+r.y});
          XPutPixel(img,x,y,(65536 * image[y][x].red) + (256 * image[y][x].green) + image[y][x].blue);
        }
    }
  }
  int screenNumber = XDefaultScreen(win.display);
  Pixmap pixmap = XCreatePixmap(win.display,root.d,r.width,r.length, DefaultDepth(win.display, screenNumber));
  //Pixmap pixmap2 = XCreateBitmapFromData(win.display,RSGL::)
  unsigned long valuemask = GCForeground | GCBackground | GCGraphicsExposures;
	XGCValues		gcv;
  gcv.foreground = XWhitePixel(win.display,screenNumber);
	gcv.background = XBlackPixel(win.display,screenNumber);
	gcv.graphics_exposures = 0;
  GC gc = XCreateGC(win.display, pixmap, valuemask,&gcv);
  XPutImage(win.display, pixmap, gc, img, 0, 0, 0,0, r.width,r.length);
  return {r,r,file, cords, img, image,pixmap,gc,true};
}

int RSGL::drawImage(RSGL::image image,RSGL::drawable win){
  if (image.srcr.width != image.r.width || image.srcr.length != image.r.length) image = RSGL::loadImage(image.file,image.r);
  XCopyArea(win.display,image.pixmap,win.d,image.gc,0,0,image.r.width,image.r.length,image.r.x,image.r.y);
  return 1;
}


int RSGL::clear(RSGL::rect r,RSGL::drawable win){
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

bool RSGL::window::isPressed(unsigned long key) {
    KeyCode kc2 = XKeysymToKeycode(display, key);
    bool isPressed = !!(keyboard[kc2 >> 3] & (1 << (kc2 & 7)));
    return isPressed;
}


int RSGL::drawRect(RSGL::rect r,color c, bool fill,int stroke, int lineColor, RSGL::color lineCol,RSGL::drawable win){
      if (!lineColor) lineCol = c;
      #ifdef OPENGL
        cairo_set_source_rgba(RSGL::ctx, (double)lineCol.r,(double)lineCol.g,(double)lineCol.b,(double)lineCol.a);
        cairo_rectangle(RSGL::ctx, (float)r.x,(float)r.y, (float)r.width,(float)r.length);
        cairo_set_line_width(RSGL::ctx, stroke);
        cairo_stroke(RSGL::ctx);
        if (fill){
          cairo_rectangle(RSGL::ctx, (float)r.x,(float)r.y, (float)r.width,(float)r.length);
          cairo_set_source_rgba(RSGL::ctx, (double)c.r,(double)c.g,(double)c.b,(double)c.a);
          cairo_fill(RSGL::ctx); 
        }
      #endif
      return 1;
}

int RSGL::drawCircle(RSGL::circle c, color col,bool fill,int stroke, int lineColor, RSGL::color lineCol,RSGL::drawable win){
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


int RSGL::drawPoint(RSGL::point p, color c, RSGL::drawable win){
  #ifdef OPENGL
      cairo_set_source_rgba(RSGL::ctx, (double)c.r,(double)c.b,(double)c.g,(double)c.a);
      cairo_rectangle(RSGL::ctx, (float)p.x,(float)p.y, 1,1);
      cairo_fill_preserve(RSGL::ctx);
  #endif
  return 1;
}


void RSGL::window::CheckEvents(){
  XEvent E;
  XNextEvent(display, &E);
  event.type = E.type;
  if (event.type == 33 && E.xclient.data.l[0] == XInternAtom(display, "WM_DELETE_WINDOW", true)){} 
  else if (event.type == 33 && E.xclient.message_type == XInternAtom(display, "XdndDrop", false)){event.type=34;}  else if (event.type==33){event.type = 0;} 
  if (event.type == 4 || event.type == 5){event.button = E.xbutton.button;}
  if (event.type == 4 || event.type == 5 || event.type == 6){event.x=E.xbutton.x;event.y=E.xbutton.y;}
  if (event.type == 2 || event.type == 3){XQueryKeymap(display,keyboard);}
}

void RSGL::window::close(){
  XCloseDisplay(display);
}



int RSGL::drawText(RSGL::text t,RSGL::drawable win){
    RSGL::text image = t;
    if (image.srcr.width != image.r.width || image.srcr.length != image.r.length || (image.c.r+image.c.g+image.c.b) != (image.sc.r+image.sc.g+image.sc.b) || image.text  != image.stext){

    }
    XCopyArea(win.display,image.pixmap,win.d,t.gc,0,0,image.r.width,image.r.length,image.r.x,image.r.y);
    return 1;
}


RSGL::pixmap::pixmap(RSGL::drawable dr, RSGL::area a){
  display = dr.display;
  d = XCreatePixmap(display,dr.d,a.width,a.length,XDefaultDepth(display,XDefaultScreen(display)));
}

int RSGL::window::setColor(RSGL::color c){
    color = c;
    XSetWindowBackground(display,d,RSGLRGBTOHEX(c.r,c.g,c.b));
    return 1;
}

RSGL::window::window(std::string wname,RSGL::rect winrect, RSGL::color c, bool resize){
    display = XOpenDisplay(0);
    if (display == nullptr) exit(0);
    Window parent = RootWindow(display, XDefaultScreen(display));
    d = XCreateSimpleWindow(display, parent, winrect.x,winrect.y,winrect.width,winrect.length, 0,0,  RSGLRGBTOHEX(c.r,c.g,c.b));
    long event_mask = ExposureMask
                | KeyPressMask
                | KeyReleaseMask
                | ButtonPressMask
                | ButtonReleaseMask
                | FocusChangeMask
                | PointerMotionMask
                | SubstructureNotifyMask
                | StructureNotifyMask
                | ExposureMask;
    XSelectInput(display, d, event_mask);
    XMapWindow(display, d);
    if (!d) d={};
    XStoreName(display, d, name.data());
    XEvent E;
    XNextEvent(display, &E);
    Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", true);
    XSetWMProtocols(display, d, &wm_delete, 1);
    XGCValues values;
    
    Atom xdndAtom = XInternAtom( display, "XdndAware", False );
    char xdndVersion = 5;
    XChangeProperty( display, d, xdndAtom, XA_ATOM, 32,
        PropModeReplace, (unsigned char *)&xdndVersion, 1 );

    #ifdef OPENGL
    RSGL::sfc = cairo_xlib_surface_create(display, d,
    DefaultVisual(display,XDefaultScreen(display)), winrect.width, winrect.length);
    cairo_xlib_surface_set_size(RSGL::sfc, winrect.width,winrect.length);
    RSGL::ctx = cairo_create(RSGL::sfc);
    #endif
    //"hack" to stop the window from resizing
    /*if (!resize){
        XSizeHints* sizeHints = XAllocSizeHints();
        sizeHints->flags = PMinSize | PMaxSize | USPosition;
        sizeHints->min_width =  r.width; sizeHints->max_width = r.width;
        sizeHints->min_height =  r.width; sizeHints->max_height = r.length;
        sizeHints->x = r.x;
        sizeHints->y = r.y;
        XSetWMNormalHints(display,d, sizeHints);
        XFree(sizeHints);
    }*/
    name = wname; r = winrect; color = c;
    if (&RSGL::root == nullptr) root.name = wname; root.r = winrect; root.color = c; root.d = d;

    dbuffer.d = XCreatePixmap(display,d,winrect.width,winrect.length,XDefaultDepth(display,XDefaultScreen(display)));
}

int RSGL::CircleCollide(RSGL::circle cir,RSGL::circle cir2){
	float distanceBetweenCircles = (float)sqrt(
	(cir2.x - cir.x) * (cir2.x - cir.x) + 
    (cir2.y - cir.y) * (cir2.y - cir.y)
  	);
	if (distanceBetweenCircles > cir.radius + cir2.radius){return 0;}else{return 1;}
}

int RSGL::CircleCollideRect(RSGL::circle c, RSGL::rect r){
  float testX = c.x; float testY = c.y;

  if (c.x < r.x) {testX = r.x;}  else if (c.x > r.x+r.width) {testX = r.x+r.width;}
  if (c.y < r.y) {testY = r.y;}  else if (c.y > r.y+r.length) {testY = r.y+r.length;} 
  
  return (sqrt( ( (c.x-testX) * (c.x-testX) ) + ( (c.y-testY) *(c.y-testY) ) )  <= c.radius);
}

int RSGL::CircleCollidePoint(RSGL::circle c, RSGL::point p){
	float testX = c.x; float testY = c.y;

  	if (c.x < p.x) {testX = p.x;}  else if (c.x > p.x+1) {testX = p.x+1;}
  	if (c.y < p.y) {testY = p.y;}  else if (c.y > p.y+1) {testY = p.y+1;} 
  
  	return (sqrt( ( (c.x-testX) * (c.x-testX) ) + ( (c.y-testY) *(c.y-testY) ) )  <= c.radius);
}

int RSGL::RectCollidePoint(RSGL::rect r, RSGL::point p){
    if (p.x >= r.x &&  p.x <= r.x + r.width && p.y >= r.y && p.y <= r.y + r.length) return 1;
    return 0;
}

int RSGL::PointCollide(RSGL::point p, RSGL::point p2){
    if (p.x == p2.x && p.y == p2.y) return 1;
    return 0;
}

int RSGL::RectCollideRect(RSGL::rect r, RSGL::rect r2){
    if(r.x + r.width >= r2.x && r.x <= r2.x + r2.width && r.y + r.length >= r2.y && r.y <= r2.y + r2.length) return 1; 
    return 0;
}

int RSGL::ImageCollideRect(RSGL::image img, RSGL::rect r){
    for (int i=0; i < img.cords.size(); i++){
        if(RSGL::RectCollidePoint(r, {img.cords[i][0],img.cords[i][1]})){
            return 1;
        }
    }return 0;
}

int RSGL::ImageCollideCircle(RSGL::image img, RSGL::circle c){
    for (int i=0; i < img.cords.size(); i++){
        if(RSGL::CircleCollidePoint(c, {img.cords[i][0],img.cords[i][1]})){
            return 1;
        }
    }return 0;    
}

int RSGL::ImageCollidePoint(RSGL::image img, RSGL::point p){
    for (int i=0; i < img.cords.size(); i++){
        if(RSGL::PointCollide(p, {img.cords[i][0],img.cords[i][1]})){
            return 1;
        }
    }return 0;      
}

int RSGL::ImageCollideImage(RSGL::image img, RSGL::image img2){
    for (int i=0; i < img.cords.size(); i++){
        for (int j=0; j < img2.cords.size(); j++)
            if(RSGL::PointCollide({img2.cords[i][0],img2.cords[i][1]}, {img.cords[i][0],img.cords[i][1]})){
                return 1;
            }
    }return 0;      
}