#ifndef Settings_hpp
#define Settings_hpp

#include <stdio.h>
#include "../../SDL2.framework/Versions/A/Headers/SDL.h"

class Settings
{
public:
    Settings(SDL_Window* window, SDL_Renderer* renderer, int *setFps);
    ~Settings();
    void display();
    void execute(bool *pointer);
    bool getRun();
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect square;
    SDL_Texture* header;
    SDL_Texture* background;
    SDL_Texture* fps601;
    SDL_Texture* fps602;
    SDL_Texture* fps901;
    SDL_Texture* fps902;
    SDL_Texture* fps1201;
    SDL_Texture* fps1202;
    bool isRunning = true;
};

#endif /* Settings_hpp */
