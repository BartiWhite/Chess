#ifndef Piece_hpp
#define Piece_hpp

#include "/Library/Frameworks/SDL2.framework/Headers/SDL.h"

class Piece{
    
public:
    Piece(int type, int x, int y);
    ~Piece();
    
    void Update(int xPos, int yPos);
    void updateTexture(int type);
    const char* retPath(int type);
    int getType();
    void Render();
    
    SDL_Rect dirRec {0, 0, 80, 80};
    
private:
    SDL_Texture* texture;
    int type;
};

#endif /* Piece_hpp */
