#include "Settings.hpp"
#include "TextureMaker.hpp"

SDL_Event settingsEvent;
SDL_Point settingsMousePos;
int *fpsChosen = nullptr;

Settings::Settings(SDL_Window* window, SDL_Renderer* renderer, int *setFps)
{
    this->window = window;
    this->renderer = renderer;
    background = TextureMaker::retTexture("/Users/bartek/Desktop/Game/graphics/background.jpg", renderer);
    header = TextureMaker::retTexture("/Users/bartek/Desktop/Game/graphics/logo.png" ,renderer);
    fps601 = TextureMaker::retTexture("/Users/bartek/Desktop/Game/graphics/60fps1.png", renderer);
    fps602 = TextureMaker::retTexture("/Users/bartek/Desktop/Game/graphics/60fps2.png" ,renderer);
    fps901 = TextureMaker::retTexture("/Users/bartek/Desktop/Game/graphics/90fps1.png", renderer);
    fps902 = TextureMaker::retTexture("/Users/bartek/Desktop/Game/graphics/90fps2.png" ,renderer);
    fps1201 = TextureMaker::retTexture("/Users/bartek/Desktop/Game/graphics/120fps1.png", renderer);
    fps1202 = TextureMaker::retTexture("/Users/bartek/Desktop/Game/graphics/120fps2.png" ,renderer);
    fpsChosen = setFps;
}

Settings::~Settings()
{
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(header);
    SDL_DestroyTexture(fps601);
    SDL_DestroyTexture(fps602);
    SDL_DestroyTexture(fps901);
    SDL_DestroyTexture(fps902);
    SDL_DestroyTexture(fps1201);
    SDL_DestroyTexture(fps1202);
}

void Settings::display()
{
    SDL_RenderClear(renderer);
    
    square.x = 0;
    square.y = 0;
    square.w = 720;
    square.h = 720;
    
    SDL_RenderCopy(renderer, background, NULL, &square);
    
    square.x = 160;
    square.y = 35;
    square.w = 400;
    square.h = 210;
    
    SDL_RenderCopy(renderer, header, NULL, &square);
    
    square.w = 134;
    square.h = 40;
    square.x = 159;
    square.y = 300;
    
    if(*fpsChosen == 60)
        SDL_RenderCopy(renderer, fps602, NULL, &square);
    else
        SDL_RenderCopy(renderer, fps601, NULL, &square);
    
    square.x = 293;
    
    if(*fpsChosen == 90)
        SDL_RenderCopy(renderer, fps902, NULL, &square);
    else
        SDL_RenderCopy(renderer, fps901, NULL, &square);
    
    square.x = 427;
    
    if(*fpsChosen == 120)
        SDL_RenderCopy(renderer, fps1202, NULL, &square);
    else
        SDL_RenderCopy(renderer, fps1201, NULL, &square);
    
    SDL_RenderPresent(renderer);
}

void Settings::execute(bool *pointer)
{
    SDL_PollEvent(&settingsEvent);
    switch (settingsEvent.type) {
        case SDL_QUIT:
            *(pointer + 3) = true;
            isRunning = false;
            break;
            
        case SDL_KEYDOWN:
            if(settingsEvent.key.keysym.sym == SDLK_ESCAPE)
                isRunning = false;
            break;
            
//        case SDL_MOUSEMOTION:
//
//
//            break;
            
        case SDL_MOUSEBUTTONUP:
            
            settingsMousePos = { settingsEvent.motion.x, settingsEvent.motion.y };
            
            if(300 < settingsMousePos.y && settingsMousePos.y < 340)
            {
                switch (settingsMousePos.x) {
                    case 159 ... 293:
                        *fpsChosen = 60;
                        break;
                        
                    case 294 ... 427:
                        *fpsChosen = 90;
                        break;
                        
                    case 428 ... 561:
                        *fpsChosen = 120;
                        break;
                        
                    default:
                        break;
                }
            }
            
            break;

        default:
            break;
    }
}

bool Settings::getRun() { return isRunning; }
