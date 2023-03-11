#ifndef TextureMaker_hpp
#define TextureMaker_hpp

#include "Game.hpp"

class TextureMaker{
  
public:
    static SDL_Texture* retTexture(const char* path);
    static SDL_Texture* retTexture(const char *path, SDL_Renderer* renderer);
    static SDL_Texture* createText();
};

#endif /* TextureMaker_hpp */
