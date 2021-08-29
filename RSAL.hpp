#include <iostream> //std::string, std::cout
#include <fstream> //reading audio files
#include <pthread.h> //threading
#include <mpg123.h>

namespace RSAL{
    struct audio{
        void play();
        void pause();
    };
    audio loadMP3(std::string mp3,bool file=true);
    audio loadWAV(std::string wav,bool file=true);
};

RSAL::audio RSAL::loadMP3(std::string mp3,bool file=true){
    audio au; std::string l,data,e;
    if (file){
        std::fstream fs(mp3);
        if (fs.is_open()) while (std::getline(fs,l)){ data+=(e+l); e='\n';}
        else std::cout << "failied to load file" << std::endl;
    } else data=file;    

}

RSAL::audio RSAL::loadWAV(std::string wav,bool file=true){
    audio au; std::string l,data,e;
    if (file){
        std::fstream fs(wav);
        if (fs.is_open()) while (std::getline(fs,l)){ data+=(e+l); e='\n';}
        else std::cout << "failied to load file" << std::endl;
    } else data=file;   
    
}

