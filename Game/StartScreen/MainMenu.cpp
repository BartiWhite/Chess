#include "MainMenu.hpp"
#include "Piece.hpp"

SDL_Event menuEvent;
SDL_Point mousePos;
int buttonsHover = 0;

MainMenu::MainMenu(SDL_Window* window, SDL_Renderer* renderer)
{
    this->window = window;
    this->renderer = renderer;
    background = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/graphics/background.jpg", renderer);
    header = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/graphics/logo.png" ,renderer);
    play1 = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/graphics/play1.png", renderer);
    play2 = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/graphics/play2.png", renderer);
    settings1 = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/graphics/settings1.png", renderer);
    settings2 = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/graphics/settings2.png", renderer);
    exit1 = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/graphics/exit1.png", renderer);
    exit2 = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/graphics/exit2.png", renderer);
}

const char* MainMenu::concString(char str[15]){
    char path[70] = "/Users/bartek/Desktop/Game/graphics/";
    std::strcat(path, str);
    const char *car  = (const char*) path;
    std::cout << car;
    return car;
}

MainMenu::~MainMenu()
{
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(header);
    SDL_DestroyTexture(play1);
    SDL_DestroyTexture(play2);
    SDL_DestroyTexture(settings1);
    SDL_DestroyTexture(settings2);
    SDL_DestroyTexture(exit1);
    SDL_DestroyTexture(exit2);
}

void MainMenu::display()
{
    SDL_Init(IMG_INIT_PNG);
    
    SDL_SetRenderDrawColor(renderer, 219, 176, 129, 0);
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
    
    square.x = 265;
    square.y = 350;
    square.w = 190;
    square.h = 50;
    
    if(buttonsHover == 1)
        SDL_RenderCopy(renderer, play2, NULL, &square);
    else
        SDL_RenderCopy(renderer, play1, NULL, &square);
    
    square.y = 450;
    
    if(buttonsHover == 2)
        SDL_RenderCopy(renderer, settings2, NULL, &square);
    else
        SDL_RenderCopy(renderer, settings1, NULL, &square);
    
    square.y = 550;
    
    if(buttonsHover == 3)
        SDL_RenderCopy(renderer, exit2, NULL, &square);
    else
        SDL_RenderCopy(renderer, exit1, NULL, &square);
    
    SDL_RenderPresent(renderer);
}

void MainMenu::execute(bool *pointer)
{
    SDL_PollEvent(&menuEvent);
    switch (menuEvent.type) {
        case SDL_QUIT:
            *pointer = false;
            isRunning = false;
            break;
            
        case SDL_KEYDOWN:
            if(menuEvent.key.keysym.sym == SDLK_ESCAPE)
                isRunning = false;
            break;
            
        case SDL_MOUSEMOTION:
            
            mousePos = { menuEvent.motion.x, menuEvent.motion.y };
            
            if(265 < mousePos.x && mousePos.x < 455)
            {
                switch (mousePos.y) {
                    case 350 ... 400:
                        buttonsHover = 1;
                        break;
                        
                    case 450 ... 500:
                        buttonsHover = 2;
                        break;
                        
                    case 550 ... 600:
                        buttonsHover = 3;
                        break;
                        
                    default:
                        buttonsHover = 0;
                        break;
                }
            }
            else buttonsHover = 0;
            break;
            
        case SDL_MOUSEBUTTONUP:
            switch (buttonsHover) {
                case 1:
                    *pointer = false;
                    *(pointer + 1) = true;
                    break;
                    
                case 2:
                    *pointer = false;
                    *(pointer + 2) = true;
                    break;
                    
                case 3:
                    *pointer = false;
                    *(pointer + 3) = true;
                    isRunning = false;
                    break;
                    
                default:
                    break;
            }
            break;

        default:
            break;
    }
}

bool MainMenu::getRun() {return isRunning;}
