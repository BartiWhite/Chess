#include "Game.hpp"
#include "MainMenu.hpp"
#include "Settings.hpp"

int main() {
    SDL_Window* window = SDL_CreateWindow("Chess",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 720, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool appRun = true;
    bool modulesRun[4];
    int fpsChosen = 60;
    
    MainMenu mainMenu(window, renderer);
    
    while (appRun) {
        
        int delay = 1000/fpsChosen;
        Uint32 frameStart;
        int frameTime;
        
        modulesRun[0] = true;
        for(int i = 1; i < 3; i++)
            modulesRun[i] = false;

        do
        {
            frameStart = SDL_GetTicks();
            
            mainMenu.display(); // requires much memory, probably because of the size of (displayed) graphics
            mainMenu.execute(modulesRun);
            
            frameTime = SDL_GetTicks() - frameStart;
            if(delay > frameTime)
                SDL_Delay(delay - frameTime);
            
            if(!mainMenu.getRun())
                appRun = false;
        } while (modulesRun[0]);

        if(modulesRun[1])
        {
            Game game;
            game.onInit(window, renderer);

            while(game.getRun())
            {
                frameStart = SDL_GetTicks();

                game.execute();
                game.drawScreen();

                frameTime = SDL_GetTicks() - frameStart;
                if(delay > frameTime)
                    SDL_Delay(delay - frameTime);

            }
            game.clean();
            
            modulesRun[0] = true;
            modulesRun[1] = false;
        }
        
        if(modulesRun[2])
        {
            int *setFps = &fpsChosen;
            Settings settings(window, renderer, setFps);
            
            while (settings.getRun()) {
                frameStart = SDL_GetTicks();
                
                settings.display();
                settings.execute(modulesRun);
                
                frameTime = SDL_GetTicks() - frameStart;
                if(delay > frameTime)
                    SDL_Delay(delay - frameTime);
            }
            
            modulesRun[0] = true;
            modulesRun[2] = false;
        }
        
        if(modulesRun[3])
            appRun = false;
        
        if(!appRun)
        {
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            IMG_Quit();
        }
        
    }
    
    return 0;
}
