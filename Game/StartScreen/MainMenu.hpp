#ifndef MainMenu_hpp
#define MainMenu_hpp

#include <stdio.h>
#include "TextureMaker.hpp"
#include "/Library/Frameworks/SDL2.framework/Headers/SDL.h"
#include "/Library/Frameworks/SDL2_image.framework/Headers/SDL_image.h"

class MainMenu
{
public:
    MainMenu(SDL_Window* window, SDL_Renderer* renderer);
    ~MainMenu();
    void display();
    void execute(bool *pointer);
    bool getRun();
    const char* concString(char* str);
    
    SDL_Rect square {0, 0, 80, 80};
    
private:
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Texture* header;
    SDL_Texture* background;
    SDL_Texture* play1;
    SDL_Texture* play2;
    SDL_Texture* settings1;
    SDL_Texture* settings2;
    SDL_Texture* exit1;
    SDL_Texture* exit2;
    bool isRunning = true;
};

#endif /* MainMenu_hpp */
