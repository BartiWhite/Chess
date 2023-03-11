#include "TextureMaker.hpp"

SDL_Texture* TextureMaker::retTexture(const char *path)
{
    SDL_Surface* surface = IMG_Load(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* TextureMaker::retTexture(const char *path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
