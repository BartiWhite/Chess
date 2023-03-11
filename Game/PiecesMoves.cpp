#include "Game.hpp"

int const width = 8;

void Game::renderPossibleMoves(int y, int x)
{
    switch (type)
    {
        case 1 ... 8:
            renderWhitePawnMoves(y, x);
            break;
            
        case 9 ... 10:
            renderWhiteHorizontalAndVertical(y, x);
            break;
            
        case 11 ... 12:
            renderWhiteKnightMoves(y, x);
            break;
            
        case 13 ... 14:
            renderWhiteDiagonal(y, x);
            break;

        case 15:
            renderWhiteDiagonal(y, x);
            renderWhiteHorizontalAndVertical(y, x);
            break;
            
        case 16:
            renderKingMoves(y, x);
            if(castlingPossibility[0])
                mapMark[58] = 0;
            if(castlingPossibility[1])
                mapMark[62] = 0;
            break;
            
        case 17 ... 24:
            renderBlackPawnMoves(y, x);
            break;
            
        case 25 ... 26:
            renderBlackHorizontalAndVertical(y, x);
            break;
            
        case 27 ... 28:
            renderBlackKnightMoves(y, x);
            break;
            
        case 29 ... 30:
            renderBlackDiagonal(y, x);
            break;
            
        case 31:
            renderBlackDiagonal(y, x);
            renderBlackHorizontalAndVertical(y, x);
            break;
            
        case 32:
            renderKingMoves(y, x);
            if(castlingPossibility[2])
                mapMark[2] = 0;
            if(castlingPossibility[3])
                mapMark[6] = 0;
            break;
    }
}

void Game::renderWhitePawnMoves(int y, int x)
{
    if(lastPinnedPieceIndex != 0)
    {
        bool found = false;
        for(int i = 0; i < lastPinnedPieceIndex; i++)
        {
            if(pinnedPices[i*6] == type)
            {
                found = true;
                
                if(!check)
                {
                    int pinningDir = 0;
                    for(int j = 1; j <= 4; j++)
                        if(pinnedPices[i*6 + 1 + j] == 1)
                        {
                            pinningDir = j;
                            break;
                        }
                    
                    if(y == 6 && pinningDir == 1)
                    {
                        for(int i = 1; i < 3; i++)
                            if(map[(y-i)*width + x] == 0)
                                mapMark[(y-i)*width + x] = 0;
                    }
                    else if(map[(y-1)*width + x] == 0 && pinningDir == 1 && !check)
                        mapMark[(y-1)*width + x] = 0;
                    if(x > 0)
                        if(map[(y-1)*width + x-1] > 16 && pinningDir == 4)
                            mapMark[(y-1)*width + x-1] = 1;
                    if(x < 7)
                        if(map[(y-1)*width + x+1] > 16 && pinningDir == 3)
                            mapMark[(y-1)*width + x+1] = 1;
                    
                    if(y == 3 && blackEnPassantPosition == x-1 && pinningDir == 4)
                    {
                        mapMark[y*width + x-1] = 1;
                        mapMark[(y-1)*width + x-1] = 0;
                    }
                    else if(y == 3 && blackEnPassantPosition == x+1 && pinningDir == 3)
                    {
                        mapMark[y*width + x+1] = 1;
                        mapMark[(y-1)*width + x+1] = 0;
                    }
                }
                
                break;
            }
        }
        
        if(!found)
            markWhitePawnMoves(y, x);
    }
    else markWhitePawnMoves(y, x);
}

void Game::markWhitePawnMoves(int y, int x)
{
    if(check)
    {
        if(y == 6)
        {
            for(int i = 1; i < 3; i++)
                if(map[(y-i)*width + x] == 0 && allowedMoves[(y-i)*width + x] == 0)
                    mapMark[(y-i)*width + x] = 0;
        }
        else if(map[(y-1)*width + x] == 0 && allowedMoves[(y-1)*width + x] == 0)
            mapMark[(y-1)*width + x] = 0;
        if(x > 0)
            if(map[(y-1)*width + x-1] > 16 && allowedMoves[(y-1)*width + x-1] == 0)
                mapMark[(y-1)*width + x-1] = 1;
        if(x < 7)
            if(map[(y-1)*width + x+1] > 16 && allowedMoves[(y-1)*width + x+1] == 0)
                mapMark[(y-1)*width + x+1] = 1;
        
        if(y == 3 && blackEnPassantPosition == x-1 && allowedMoves[(y-1)*width + x-1] == 0)
        {
            mapMark[y*width + x-1] = 1;
            mapMark[(y-1)*width + x-1] = 0;
        }
        else if(y == 3 && blackEnPassantPosition == x+1 && allowedMoves[(y-1)*width + x+1] == 0)
        {
            mapMark[y*width + x+1] = 1;
            mapMark[(y-1)*width + x+1] = 0;
        }
    }
    else
    {
        if(y == 6)
        {
            for(int i = 1; i < 3; i++)
                if(map[(y-i)*width + x] == 0)
                    mapMark[(y-i)*width + x] = 0;
        }
        else if(map[(y-1)*width + x] == 0)
            mapMark[(y-1)*width + x] = 0;
        if(x > 0)
            if(map[(y-1)*width + x-1] > 16)
                mapMark[(y-1)*width + x-1] = 1;
        if(x < 7)
            if(map[(y-1)*width + x+1] > 16)
                mapMark[(y-1)*width + x+1] = 1;
        
        if(y == 3 && blackEnPassantPosition == x-1)
        {
            mapMark[y*width + x-1] = 1;
            mapMark[(y-1)*width + x-1] = 0;
        }
        else if(y == 3 && blackEnPassantPosition == x+1)
        {
            mapMark[y*width + x+1] = 1;
            mapMark[(y-1)*width + x+1] = 0;
        }
    }
}

void Game::renderWhiteKnightMoves(int y, int x)
{
    if(lastPinnedPieceIndex != 0)
    {
        bool found = false;
        for(int i = 0; i < lastPinnedPieceIndex; i++)
            if(pinnedPices[i*6] == type)
            {
                found = true;
                break;
            }
        
        if(!found)
            markWhiteKnightMoves(y, x);
    }
    else markWhiteKnightMoves(y, x);
}

void Game::markWhiteKnightMoves(int y, int x)
{
    if(check)
    {
        if(y > 0)
        {
            if(y > 1)
            {
                if(x > 0)
                {
                    if(map[(y-2)*width + x-1] == 0 && allowedMoves[(y-2)*width + x-1] == 0)
                        mapMark[(y-2)*width + x-1] = 0;
                    else if(map[(y-2)*width + x-1] > 16 && allowedMoves[(y-2)*width + x-1] == 0)
                        mapMark[(y-2)*width + x-1] = 1;
                }
                if(x < 7)
                {
                    if(map[(y-2)*width + x+1] == 0 && allowedMoves[(y-2)*width + x+1] == 0)
                        mapMark[(y-2)*width + x+1] = 0;
                    else if(map[(y-2)*width + x+1] > 16 && allowedMoves[(y-2)*width + x+1] == 0)
                        mapMark[(y-2)*width + x+1] = 1;
                }
            }
            
            if(x > 1)
            {
                if(map[(y-1)*width + x-2] == 0 && allowedMoves[(y-1)*width + x-2] == 0)
                    mapMark[(y-1)*width + x-2] = 0;
                else if(map[(y-1)*width + x-2] > 16 && allowedMoves[(y-1)*width + x-2] == 0)
                    mapMark[(y-1)*width + x-2] = 1;
            }
            if(x < 6)
            {
                if(map[(y-1)*width + x+2] == 0 && allowedMoves[(y-1)*width + x+2] == 0)
                    mapMark[(y-1)*width + x+2] = 0;
                else if(map[(y-1)*width + x+2] > 16 && allowedMoves[(y-1)*width + x+2] == 0)
                    mapMark[(y-1)*width + x+2] = 1;
            }
            
        }
        if(y < 7)
        {
            if(y < 6)
            {
                if(x > 0)
                {
                    if(map[(y+2)*width + x-1] == 0 && allowedMoves[(y+2)*width + x-1] == 0)
                        mapMark[(y+2)*width + x-1] = 0;
                    else if(map[(y+2)*width + x-1] > 16 && allowedMoves[(y+2)*width + x-1] == 0)
                        mapMark[(y+2)*width + x-1] = 1;
                }
                if(x < 7)
                {
                    if(map[(y+2)*width + x+1] == 0 && allowedMoves[(y+2)*width + x+1] == 0)
                        mapMark[(y+2)*width + x+1] = 0;
                    else if(map[(y+2)*width + x+1] > 16 && allowedMoves[(y+2)*width + x+1] == 0)
                        mapMark[(y+2)*width + x+1] = 1;
                }
            }
            
            if(x > 1)
            {
                if(map[(y+1)*width + x-2] == 0 && allowedMoves[(y+1)*width + x-2] == 0)
                    mapMark[(y+1)*width + x-2] = 0;
                else if(map[(y+1)*width + x-2] > 16 && allowedMoves[(y+1)*width + x-2] == 0)
                    mapMark[(y+1)*width + x-2] = 1;
            }
            if(x < 6)
            {
                if(map[(y+1)*width + x+2] == 0 && allowedMoves[(y+1)*width + x+2] == 0)
                    mapMark[(y+1)*width + x+2] = 0;
                else if(map[(y+1)*width + x+2] > 16 && allowedMoves[(y+1)*width + x+2] == 0)
                    mapMark[(y+1)*width + x+2] = 1;
            }
        }
    }
    else
    {
        if(y > 0)
        {
            if(y > 1)
            {
                if(x > 0)
                {
                    if(map[(y-2)*width + x-1] == 0)
                        mapMark[(y-2)*width + x-1] = 0;
                    else if(map[(y-2)*width + x-1] > 16)
                        mapMark[(y-2)*width + x-1] = 1;
                }
                if(x < 7)
                {
                    if(map[(y-2)*width + x+1] == 0)
                        mapMark[(y-2)*width + x+1] = 0;
                    else if(map[(y-2)*width + x+1] > 16)
                        mapMark[(y-2)*width + x+1] = 1;
                }
            }
            
            if(x > 1)
            {
                if(map[(y-1)*width + x-2] == 0)
                    mapMark[(y-1)*width + x-2] = 0;
                else if(map[(y-1)*width + x-2] > 16)
                    mapMark[(y-1)*width + x-2] = 1;
            }
            if(x < 6)
            {
                if(map[(y-1)*width + x+2] == 0)
                    mapMark[(y-1)*width + x+2] = 0;
                else if(map[(y-1)*width + x+2] > 16)
                    mapMark[(y-1)*width + x+2] = 1;
            }
            
        }
        if(y < 7)
        {
            if(y < 6)
            {
                if(x > 0)
                {
                    if(map[(y+2)*width + x-1] == 0)
                        mapMark[(y+2)*width + x-1] = 0;
                    else if(map[(y+2)*width + x-1] > 16)
                        mapMark[(y+2)*width + x-1] = 1;
                }
                if(x < 7)
                {
                    if(map[(y+2)*width + x+1] == 0)
                        mapMark[(y+2)*width + x+1] = 0;
                    else if(map[(y+2)*width + x+1] > 16)
                        mapMark[(y+2)*width + x+1] = 1;
                }
            }
            
            if(x > 1)
            {
                if(map[(y+1)*width + x-2] == 0)
                    mapMark[(y+1)*width + x-2] = 0;
                else if(map[(y+1)*width + x-2] > 16)
                    mapMark[(y+1)*width + x-2] = 1;
            }
            if(x < 6)
            {
                if(map[(y+1)*width + x+2] == 0)
                    mapMark[(y+1)*width + x+2] = 0;
                else if(map[(y+1)*width + x+2] > 16)
                    mapMark[(y+1)*width + x+2] = 1;
            }
        }
    }
}

void Game::renderWhiteDiagonal(int y, int x)
{
    if(lastPinnedPieceIndex != 0)
    {
        bool found = false;
        for(int i = 0; i < lastPinnedPieceIndex; i++)
        {
            if(pinnedPices[i*6] == type)
            {
                found = true;
                
                if(!check)
                {
                    int pinningDir = 0;
                    for(int j = 3; j <= 4; j++)
                        if(pinnedPices[i*6 + 1 + j] == 1)
                        {
                            pinningDir = j;
                            break;
                        }
                    
                    int lenght = 0;
                    
                    // up and left
                    if(pinningDir == 4)
                    {
                        if(x < 7 && y < 7)
                        {
                            lenght = 8 - x >= 8 - y ? 8 - y : 8 - x;
                            for(int i = 1; i < lenght; i++)
                                if(map[(y+i)*width + x+i] == 0)
                                    mapMark[(y+i)*width + x+i] = 0;
                                else if(map[(y+i)*width + x+i] > 16)
                                {
                                    mapMark[(y+i)*width + x+i] = 1;
                                    break;
                                }
                                else break;
                        }
                        if(0 < x && 0 < y)
                        {
                            lenght = x >= y ? y+1 : x+1;
                            for(int i = 1; i < lenght; i++)
                                if(map[(y-i)*width + x-i] == 0)
                                    mapMark[(y-i)*width + x-i] = 0;
                                else if(map[(y-i)*width + x-i] > 16)
                                {
                                    mapMark[(y-i)*width + x-i] = 1;
                                    break;
                                }
                                else break;
                        }
                    }
                    
                    // up and right
                    else if(pinningDir == 3)
                    {
                        if(0 < x && y < 7)
                        {
                            lenght = x >= 8-y ? 8-y : x+1;
                            for(int i = 1; i < lenght; i++)
                                if(map[(y+i)*width + x-i] == 0)
                                    mapMark[(y+i)*width + x-i] = 0;
                                else if(map[(y+i)*width + x-i] > 16)
                                {
                                    mapMark[(y+i)*width + x-i] = 1;
                                    break;
                                }
                                else break;
                        }
                        if(x < 7 && 0 < y)
                        {
                            lenght = y >= 8-x ? 8-x : y+1;
                            for(int i = 1; i < lenght; i++)
                                if(map[(y-i)*width + x+i] == 0)
                                    mapMark[(y-i)*width + x+i] = 0;
                                else if(map[(y-i)*width + x+i] > 16)
                                {
                                    mapMark[(y-i)*width + x+i] = 1;
                                    break;
                                }
                                else break;
                        }
                    }
                }
                
                break;
            }
        }
        
        if(!found)
            markWhiteDiagonal(y, x);
    }
    else markWhiteDiagonal(y, x);
}

void Game::markWhiteDiagonal(int y, int x)
{
    if(check)
    {
        int lenght = 0;
        if(x < 7 && y < 7)
        {
            lenght = 8 - x >= 8 - y ? 8 - y : 8 - x;
            for(int i = 1; i < lenght; i++)
                if(map[(y+i)*width + x+i] == 0 && allowedMoves[(y+i)*width + x+i] == 0)
                    mapMark[(y+i)*width + x+i] = 0;
                else if(map[(y+i)*width + x+i] > 16 && allowedMoves[(y+i)*width + x+i] == 0)
                {
                    mapMark[(y+i)*width + x+i] = 1;
                    break;
                }
            else if(0 < map[(y+i)*width + x+i] && map[(y+i)*width + x+i] <= 16)
                break;
        }
        if(0 < x && 0 < y)
        {
            lenght = x >= y ? y+1 : x+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y-i)*width + x-i] == 0 && allowedMoves[(y-i)*width + x-i] == 0)
                    mapMark[(y-i)*width + x-i] = 0;
                else if(map[(y-i)*width + x-i] > 16 && allowedMoves[(y-i)*width + x-i] == 0)
                {
                    mapMark[(y-i)*width + x-i] = 1;
                    break;
                }
                else if(0 < map[(y-i)*width + x-i] && map[(y-i)*width + x-i] <= 16)
                    break;
        }
        if(0 < x && y < 7)
        {
            lenght = x >= 8-y ? 8-y : x+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y+i)*width + x-i] == 0 && allowedMoves[(y+i)*width + x-i] == 0)
                    mapMark[(y+i)*width + x-i] = 0;
                else if(map[(y+i)*width + x-i] > 16 && allowedMoves[(y+i)*width + x-i] == 0)
                {
                    mapMark[(y+i)*width + x-i] = 1;
                    break;
                }
                else if(0 < map[(y+i)*width + x-i] && map[(y+i)*width + x-i] <= 16)
                    break;
        }
        if(x < 7 && 0 < y)
        {
            lenght = y >= 8-x ? 8-x : y+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y-i)*width + x+i] == 0 && allowedMoves[(y-i)*width + x+i] == 0)
                    mapMark[(y-i)*width + x+i] = 0;
                else if(map[(y-i)*width + x+i] > 16 && allowedMoves[(y-i)*width + x+i] == 0)
                {
                    mapMark[(y-i)*width + x+i] = 1;
                    break;
                }
                else if(0 < map[(y-i)*width + x+i] && map[(y-i)*width + x+i] <= 16)
                    break;
        }
    }
    else
    {
        int lenght = 0;
        if(x < 7 && y < 7)
        {
            lenght = 8 - x >= 8 - y ? 8 - y : 8 - x;
            for(int i = 1; i < lenght; i++)
                if(map[(y+i)*width + x+i] == 0)
                    mapMark[(y+i)*width + x+i] = 0;
                else if(map[(y+i)*width + x+i] > 16)
                {
                    mapMark[(y+i)*width + x+i] = 1;
                    break;
                }
                else break;
        }
        if(0 < x && 0 < y)
        {
            lenght = x >= y ? y+1 : x+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y-i)*width + x-i] == 0)
                    mapMark[(y-i)*width + x-i] = 0;
                else if(map[(y-i)*width + x-i] > 16)
                {
                    mapMark[(y-i)*width + x-i] = 1;
                    break;
                }
                else break;
        }
        if(0 < x && y < 7)
        {
            lenght = x >= 8-y ? 8-y : x+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y+i)*width + x-i] == 0)
                    mapMark[(y+i)*width + x-i] = 0;
                else if(map[(y+i)*width + x-i] > 16)
                {
                    mapMark[(y+i)*width + x-i] = 1;
                    break;
                }
                else break;
        }
        if(x < 7 && 0 < y)
        {
            lenght = y >= 8-x ? 8-x : y+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y-i)*width + x+i] == 0)
                    mapMark[(y-i)*width + x+i] = 0;
                else if(map[(y-i)*width + x+i] > 16)
                {
                    mapMark[(y-i)*width + x+i] = 1;
                    break;
                }
                else break;
        }
    }
}

void Game::renderWhiteHorizontalAndVertical(int y, int x)
{
    if(lastPinnedPieceIndex != 0)
    {
        bool found = false;
        for(int i = 0; i < lastPinnedPieceIndex; i++)
        {
            if(pinnedPices[i*6] == type)
            {
                found = true;
                
                if(!check)
                {
                    int pinningDir = 0;
                    for(int j = 1; j <= 2; j++)
                        if(pinnedPices[i*6 + 1 + j] == 1)
                        {
                            pinningDir = j;
                            break;
                        }
                    
                    // horizontal
                    if(pinningDir == 2)
                    {
                        for(int i = 1; i < 8-x; i++)
                            if(map[y*width + x+i] == 0)
                                mapMark[y*width + x+i] = 0;
                            else if(map[y*width + x+i] > 16)
                            {
                                mapMark[y*width + x+i] = 1;
                                break;
                            }
                            else if(map[y*width + x+i] <= 16)
                                break;

                        for(int i = 1; i <= x; i++)
                            if(map[y*width + x-i] == 0)
                                mapMark[y*width + x-i] = 0;
                            else if(map[y*width + x-i] > 16)
                            {
                                mapMark[y*width + x-i] = 1;
                                break;
                            }
                            else if(map[y*width + x-i] <= 16)
                                break;
                    }

                    // vertical
                    else if(pinningDir == 1)
                    {
                        for(int i = 1; i < 8-y; i++)
                            if(map[(y+i)*width + x] == 0)
                                mapMark[(y+i)*width + x] = 0;
                            else if(map[(y+i)*width + x] > 16)
                            {
                                mapMark[(y+i)*width + x] = 1;
                                break;
                            }
                            else if(map[(y+i)*width + x] <= 16)
                                break;

                        for(int i = 1; i <= y; i++)
                            if(map[(y-i)*width + x] == 0)
                                mapMark[(y-i)*width + x] = 0;
                            else if(map[(y-i)*width + x] > 16)
                            {
                                mapMark[(y-i)*width + x] = 1;
                                break;
                            }
                            else if(map[(y-i)*width + x] <= 16)
                                break;
                    }
                }
            }
        }
        if(!found)
            markWhiteHorizontalAndVertical(y, x);
    }
    else markWhiteHorizontalAndVertical(y, x);
}

void Game::markWhiteHorizontalAndVertical(int y, int x)
{
    if(check)
    {
        for(int i = 1; i < 8-x; i++)
            if(map[y*width + x+i] == 0 && allowedMoves[y*width + x+i] == 0)
                mapMark[y*width + x+i] = 0;
            else if(map[y*width + x+i] > 16 && allowedMoves[y*width + x+i] == 0)
            {
                mapMark[y*width + x+i] = 1;
                break;
            }
            else if(0 < map[y*width + x+i] && map[y*width + x+i] <= 16)
                break;
            
        if(x > 0)
        {
            for(int i = 1; i <= x; i++)
                if(map[y*width + x-i] == 0 && allowedMoves[y*width + x-i] == 0)
                    mapMark[y*width + x-i] = 0;
                else if(map[y*width + x-i] > 16 && allowedMoves[y*width + x-i] == 0)
                {
                    mapMark[y*width + x-i] = 1;
                    break;
                }
                else if(0 < map[y*width + x-i] && map[y*width + x-i] <= 16)
                    break;
        }
        for(int i = 1; i < 8-y; i++)
            if(map[(y+i)*width + x] == 0 && allowedMoves[(y+i)*width + x] == 0)
                mapMark[(y+i)*width + x] = 0;
            else if(map[(y+i)*width + x] > 16 && allowedMoves[(y+i)*width + x] == 0)
            {
                mapMark[(y+i)*width + x] = 1;
                break;
            }
            else if(0 < map[(y+i)*width + x] && map[(y+i)*width + x] <= 16)
                break;
        if(y > 0)
        {
            for(int i = 1; i <= y; i++)
                if(map[(y-i)*width + x] == 0 && allowedMoves[(y-i)*width + x] == 0)
                    mapMark[(y-i)*width + x] = 0;
                else if(map[(y-i)*width + x] > 16 && allowedMoves[(y-i)*width + x] == 0)
                {
                    mapMark[(y-i)*width + x] = 1;
                    break;
                }
                else if(0 < map[(y-i)*width + x] && map[(y-i)*width + x] <= 16)
                    break;
        }
    }
    else
    {
        for(int i = 1; i < 8-x; i++)
            if(map[y*width + x+i] == 0)
                mapMark[y*width + x+i] = 0;
            else if(map[y*width + x+i] > 16)
            {
                mapMark[y*width + x+i] = 1;
                break;
            }
            else if(map[y*width + x+i] <= 16)
                break;
        if(x > 0)
        {
            for(int i = 1; i <= x; i++)
                if(map[y*width + x-i] == 0)
                    mapMark[y*width + x-i] = 0;
                else if(map[y*width + x-i] > 16)
                {
                    mapMark[y*width + x-i] = 1;
                    break;
                }
                else if(map[y*width + x-i] <= 16)
                    break;
        }
        for(int i = 1; i < 8-y; i++)
            if(map[(y+i)*width + x] == 0)
                mapMark[(y+i)*width + x] = 0;
            else if(map[(y+i)*width + x] > 16)
            {
                mapMark[(y+i)*width + x] = 1;
                break;
            }
            else if(map[(y+i)*width + x] <= 16)
                break;
        if(y > 0)
        {
            for(int i = 1; i <= y; i++)
                if(map[(y-i)*width + x] == 0)
                    mapMark[(y-i)*width + x] = 0;
                else if(map[(y-i)*width + x] > 16)
                {
                    mapMark[(y-i)*width + x] = 1;
                    break;
                }
                else if(map[(y-i)*width + x] <= 16)
                    break;
        }
    }
}

void Game::renderKingMoves(int y, int x)
{
    int start = x > 0 ? 0 : 1;
    int stop = x < 7 ? 0 : 1;
    if(y > 0)
    {
        for(int i = start; i < 3 - stop; i++)
            if(kingMoves[i] == 0)
                mapMark[(y-1)*width + x-1+i] = 0;
            else if(kingMoves[i] == 1)
                mapMark[(y-1)*width + x-1+i] = 1;
    }
    if(y < 7)
    {
        for(int i = start; i < 3 - stop; i++)
            if(kingMoves[6-i] == 0)
                mapMark[(y+1)*width + x-1+i] = 0;
            else if(kingMoves[6-i] == 1)
                mapMark[(y+1)*width + x-1+i] = 1;
    }
    if(start != 1)
    {
        if(kingMoves[7] == 0)
            mapMark[y*width + x-1] = 0;
        else if(kingMoves[7] == 1)
            mapMark[y*width + x-1] = 1;
    }
    if(stop != 1)
    {
        if(kingMoves[3] == 0)
            mapMark[y*width + x+1] = 0;
        else if(kingMoves[3] == 1)
            mapMark[y*width + x+1] = 1;
    }
}

void Game::renderBlackPawnMoves(int y, int x)
{
    if(lastPinnedPieceIndex != 0)
    {
        int found = false;
        for(int i = 0; i < lastPinnedPieceIndex; i++)
        {
            if(pinnedPices[i*6] == type)
            {
                found = true;
                
                if(!check)
                {
                    int pinningDir = 0;
                    for(int j = 1; j <= 4; j++)
                        if(pinnedPices[i*6 + 1 + j] == 1)
                        {
                            pinningDir = j;
                            break;
                        }
                    
                    if(y == 1 && pinningDir == 1)
                    {
                        for(int i = 1; i < 3; i++)
                            if(map[(y+i)*width + x] == 0)
                                mapMark[(y+i)*width + x] = 0;
                    }
                    else if(map[(y+1)*width + x] == 0 && pinningDir == 1 && !check)
                        mapMark[(y+1)*width + x] = 0;
                    if(x > 0)
                        if(map[(y+1)*width + x-1] <= 16 && map[(y+1)*width + x-1] > 0 && pinningDir == 3)
                            mapMark[(y+1)*width + x-1] = 1;
                    if(x < 7)
                        if(map[(y+1)*width + x+1] <= 16 && map[(y+1)*width + x+1] > 0 && pinningDir == 4)
                            mapMark[(y+1)*width + x+1] = 1;
                    
                    if(y == 4 && whiteEnPassantPosition == x-1 && pinningDir == 3)
                    {
                        mapMark[y*width + x-1] = 1;
                        mapMark[(y+1)*width + x-1] = 0;
                    }
                    else if(y == 4 && whiteEnPassantPosition == x+1 && pinningDir == 4)
                    {
                        mapMark[y*width + x+1] = 1;
                        mapMark[(y+1)*width + x+1] = 0;
                    }
                }
                
                break;
            }
        }
        
        if(!found)
            markBlackPawnMoves(y, x);
    }
    else markBlackPawnMoves(y, x);
}

void Game::markBlackPawnMoves(int y, int x)
{
    if(check)
    {
        if(y == 1)
        {
            for(int i = 1; i < 3; i++)
                if(map[(y+i)*width + x] == 0 && allowedMoves[(y+i)*width + x] == 0)
                    mapMark[(y+i)*width + x] = 0;
        }
        else if(map[(y+1)*width + x] == 0 && allowedMoves[(y+1)*width + x] == 0)
            mapMark[(y+1)*width + x] = 0;
        if(x > 0)
            if(map[(y+1)*width + x-1] <= 16 && map[(y+1)*width + x-1] > 0 && allowedMoves[(y+1)*width + x-1] == 0)
                mapMark[(y+1)*width + x-1] = 1;
        if(x < 7)
            if(map[(y+1)*width + x+1] <= 16 && map[(y+1)*width + x+1] > 0 && allowedMoves[(y+1)*width + x+1] == 0)
                mapMark[(y+1)*width + x+1] = 1;
        
        if(y == 4 && whiteEnPassantPosition == x-1 && allowedMoves[(y-1)*width + x-1] == 0)
        {
            mapMark[y*width + x-1] = 1;
            mapMark[(y+1)*width + x-1] = 0;
        }
        else if(y == 4 && whiteEnPassantPosition == x+1 && allowedMoves[(y-1)*width + x+1] == 0)
        {
            mapMark[y*width + x+1] = 1;
            mapMark[(y+1)*width + x+1] = 0;
        }
    }
    else
    {
        if(y == 1)
        {
            for(int i = 1; i < 3; i++)
                if(map[(y+i)*width + x] == 0)
                    mapMark[(y+i)*width + x] = 0;
        }
        else if(map[(y+1)*width + x] == 0)
            mapMark[(y+1)*width + x] = 0;
        if(x > 0)
            if(map[(y+1)*width + x-1] <= 16 && map[(y+1)*width + x-1] > 0)
                mapMark[(y+1)*width + x-1] = 1;
        if(x < 7)
            if(map[(y+1)*width + x+1] <= 16 && map[(y+1)*width + x+1] > 0)
                mapMark[(y+1)*width + x+1] = 1;
        
        if(y == 4 && whiteEnPassantPosition == x-1)
        {
            mapMark[y*width + x-1] = 1;
            mapMark[(y+1)*width + x-1] = 0;
        }
        else if(y == 4 && whiteEnPassantPosition == x+1)
        {
            mapMark[y*width + x+1] = 1;
            mapMark[(y+1)*width + x+1] = 0;
        }
    }
}

void Game::renderBlackKnightMoves(int y, int x)
{
    if(lastPinnedPieceIndex != 0)
    {
        bool found = false;
        for(int i = 0; i < lastPinnedPieceIndex; i++)
            if(pinnedPices[i*6] == type)
            {
                found = true;
                break;
            }
        
        if(!found)
            markBlackKnightMoves(y, x);
    }
    else markBlackKnightMoves(y, x);
}

void Game::markBlackKnightMoves(int y, int x)
{
    if(check)
    {
        if(y > 0)
        {
            if(y > 1)
            {
                if(x > 0)
                {
                    if(map[(y-2)*width + x-1] == 0 && allowedMoves[(y-2)*width + x-1] == 0)
                        mapMark[(y-2)*width + x-1] = 0;
                    else if(map[(y-2)*width + x-1] <= 16 && allowedMoves[(y-2)*width + x-1] == 0)
                        mapMark[(y-2)*width + x-1] = 1;
                }
                if(x < 7)
                {
                    if(map[(y-2)*width + x+1] == 0 && allowedMoves[(y-2)*width + x+1] == 0)
                        mapMark[(y-2)*width + x+1] = 0;
                    else if(map[(y-2)*width + x+1] <= 16 && allowedMoves[(y-2)*width + x+1] == 0)
                        mapMark[(y-2)*width + x+1] = 1;
                }
            }
            
            if(x > 1)
            {
                if(map[(y-1)*width + x-2] == 0 && allowedMoves[(y-1)*width + x-2] == 0)
                    mapMark[(y-1)*width + x-2] = 0;
                else if(map[(y-1)*width + x-2] <= 16 && allowedMoves[(y-1)*width + x-2] == 0)
                    mapMark[(y-1)*width + x-2] = 1;
            }
            if(x < 6)
            {
                if(map[(y-1)*width + x+2] == 0 && allowedMoves[(y-1)*width + x+2] == 0)
                    mapMark[(y-1)*width + x+2] = 0;
                else if(map[(y-1)*width + x+2] <= 16 && allowedMoves[(y-1)*width + x+2] == 0)
                    mapMark[(y-1)*width + x+2] = 1;
            }
            
        }
        if(y < 7)
        {
            if(y < 6)
            {
                if(x > 0)
                {
                    if(map[(y+2)*width + x-1] == 0 && allowedMoves[(y+2)*width + x-1] == 0)
                        mapMark[(y+2)*width + x-1] = 0;
                    else if(map[(y+2)*width + x-1] <= 16 && allowedMoves[(y+2)*width + x-1] == 0)
                        mapMark[(y+2)*width + x-1] = 1;
                }
                if(x < 7)
                {
                    if(map[(y+2)*width + x+1] == 0 && allowedMoves[(y+2)*width + x+1] == 0)
                        mapMark[(y+2)*width + x+1] = 0;
                    else if(map[(y+2)*width + x+1] <= 16 && allowedMoves[(y+2)*width + x+1] == 0)
                        mapMark[(y+2)*width + x+1] = 1;
                }
            }
            
            if(x > 1)
            {
                if(map[(y+1)*width + x-2] == 0 && allowedMoves[(y+1)*width + x-2] == 0)
                    mapMark[(y+1)*width + x-2] = 0;
                else if(map[(y+1)*width + x-2] <= 16 && allowedMoves[(y+1)*width + x-2] == 0)
                    mapMark[(y+1)*width + x-2] = 1;
            }
            if(x < 6)
            {
                if(map[(y+1)*width + x+2] == 0 && allowedMoves[(y+1)*width + x+2] == 0)
                    mapMark[(y+1)*width + x+2] = 0;
                else if(map[(y+1)*width + x+2] <= 16 && allowedMoves[(y+1)*width + x+2] == 0)
                    mapMark[(y+1)*width + x+2] = 1;
            }
        }
    }
    else
    {
        if(y > 0)
        {
            if(y > 1)
            {
                if(x > 0)
                {
                    if(map[(y-2)*width + x-1] == 0)
                        mapMark[(y-2)*width + x-1] = 0;
                    else if(map[(y-2)*width + x-1] <= 16)
                        mapMark[(y-2)*width + x-1] = 1;
                }
                if(x < 7)
                {
                    if(map[(y-2)*width + x+1] == 0)
                        mapMark[(y-2)*width + x+1] = 0;
                    else if(map[(y-2)*width + x+1] <= 16)
                        mapMark[(y-2)*width + x+1] = 1;
                }
            }
            
            if(x > 1)
            {
                if(map[(y-1)*width + x-2] == 0)
                    mapMark[(y-1)*width + x-2] = 0;
                else if(map[(y-1)*width + x-2] <= 16)
                    mapMark[(y-1)*width + x-2] = 1;
            }
            if(x < 6)
            {
                if(map[(y-1)*width + x+2] == 0)
                    mapMark[(y-1)*width + x+2] = 0;
                else if(map[(y-1)*width + x+2] <= 16)
                    mapMark[(y-1)*width + x+2] = 1;
            }
            
        }
        if(y < 7)
        {
            if(y < 6)
            {
                if(x > 0)
                {
                    if(map[(y+2)*width + x-1] == 0)
                        mapMark[(y+2)*width + x-1] = 0;
                    else if(map[(y+2)*width + x-1] <= 16)
                        mapMark[(y+2)*width + x-1] = 1;
                }
                if(x < 7)
                {
                    if(map[(y+2)*width + x+1] == 0)
                        mapMark[(y+2)*width + x+1] = 0;
                    else if(map[(y+2)*width + x+1] <= 16)
                        mapMark[(y+2)*width + x+1] = 1;
                }
            }
            
            if(x > 1)
            {
                if(map[(y+1)*width + x-2] == 0)
                    mapMark[(y+1)*width + x-2] = 0;
                else if(map[(y+1)*width + x-2] <= 16)
                    mapMark[(y+1)*width + x-2] = 1;
            }
            if(x < 6)
            {
                if(map[(y+1)*width + x+2] == 0)
                    mapMark[(y+1)*width + x+2] = 0;
                else if(map[(y+1)*width + x+2] <= 16)
                    mapMark[(y+1)*width + x+2] = 1;
            }
        }
    }
}

void Game::renderBlackHorizontalAndVertical(int y, int x)
{
    if(lastPinnedPieceIndex != 0)
    {
        bool found = false;
        for(int i = 0; i < lastPinnedPieceIndex; i++)
        {
            if(pinnedPices[i*6] == type)
            {
                found = true;
                
                if(!check)
                {
                    int pinningDir = 0;
                    for(int j = 1; j <= 2; j++)
                        if(pinnedPices[i*6 + 1 + j] == 1)
                        {
                            pinningDir = j;
                            break;
                        }
                    
                    // horizontal
                    if(pinningDir == 2)
                    {
                        for(int i = 1; i < 8-x; i++)
                            if(map[y*width + x+i] == 0)
                                mapMark[y*width + x+i] = 0;
                            else if(map[y*width + x+i] <= 16)
                            {
                                mapMark[y*width + x+i] = 1;
                                break;
                            }
                            else if(map[y*width + x+i] > 16)
                                break;

                        for(int i = 1; i <= x; i++)
                            if(map[y*width + x-i] == 0)
                                mapMark[y*width + x-i] = 0;
                            else if(map[y*width + x-i] <= 16)
                            {
                                mapMark[y*width + x-i] = 1;
                                break;
                            }
                            else if(map[y*width + x-i] > 16)
                                break;
                    }

                    // vertical
                    else if(pinningDir == 1)
                    {
                        for(int i = 1; i < 8-y; i++)
                            if(map[(y+i)*width + x] == 0)
                                mapMark[(y+i)*width + x] = 0;
                            else if(map[(y+i)*width + x] <= 16)
                            {
                                mapMark[(y+i)*width + x] = 1;
                                break;
                            }
                            else if(map[(y+i)*width + x] > 16)
                                break;
                        
                        for(int i = 1; i <= y; i++)
                            if(map[(y-i)*width + x] == 0)
                                mapMark[(y-i)*width +x] = 0;
                            else if(map[(y-i)*width + x] <= 16)
                            {
                                mapMark[(y-i)*width + x] = 1;
                                break;
                            }
                            else if(map[(y-i)*width + x] > 16)
                                break;
                    }
                }
            }
            
            break;
        }
        
        if(!found)
            markBlackHorizontalAndVertical(y, x);
    }
    else markBlackHorizontalAndVertical(y, x);
}

void Game::markBlackHorizontalAndVertical(int y, int x)
{
    if(check)
    {
        for(int i = 1; i < 8-x; i++)
            if(map[y*width + x+i] == 0 && allowedMoves[y*width + x+i] == 0)
                mapMark[y*width + x+i] = 0;
            else if(map[y*width + x+i] <= 16 && allowedMoves[y*width + x+i] == 0)
            {
                mapMark[y*width + x+i] = 1;
                break;
            }
            else if(map[y*width + x+i] > 16 && allowedMoves[y*width + x+i] == 0)
                break;
        if(x > 0)
        {
            for(int i = 1; i <= x; i++)
                if(map[y*width + x-i] == 0 && allowedMoves[y*width + x-i] == 0)
                    mapMark[y*width + x-i] = 0;
                else if(map[y*width + x-i] <= 16 && allowedMoves[y*width + x-i] == 0)
                {
                    mapMark[y*width + x-i] = 1;
                    break;
                }
                else if(map[y*width + x-i] > 16 && allowedMoves[y*width + x-i] == 0)
                    break;
        }
        for(int i = 1; i < 8-y; i++)
            if(map[(y+i)*width + x] == 0 && allowedMoves[(y+i)*width + x] == 0)
                mapMark[(y+i)*width + x] = 0;
            else if(map[(y+i)*width + x] <= 16 && allowedMoves[(y+i)*width + x] == 0)
            {
                mapMark[(y+i)*width + x] = 1;
                break;
            }
            else if(map[(y+i)*width + x] > 16 && allowedMoves[(y+i)*width + x] == 0)
                break;
        if(y > 0)
        {
            for(int i = 1; i <= y; i++)
                if(map[(y-i)*width + x] == 0 && allowedMoves[(y-i)*width + x] == 0)
                    mapMark[(y-i)*width +x] = 0;
                else if(map[(y-i)*width + x] <= 16 && allowedMoves[(y-i)*width + x] == 0)
                {
                    mapMark[(y-i)*width + x] = 1;
                    break;
                }
                else if(map[(y-i)*width + x] > 16 && allowedMoves[(y-i)*width + x] == 0)
                    break;
        }
    }
    else
    {
        for(int i = 1; i < 8-x; i++)
            if(map[y*width + x+i] == 0)
                mapMark[y*width + x+i] = 0;
            else if(map[y*width + x+i] <= 16)
            {
                mapMark[y*width + x+i] = 1;
                break;
            }
            else if(map[y*width + x+i] > 16)
                break;
        if(x > 0)
        {
            for(int i = 1; i <= x; i++)
                if(map[y*width + x-i] == 0)
                    mapMark[y*width + x-i] = 0;
                else if(map[y*width + x-i] <= 16)
                {
                    mapMark[y*width + x-i] = 1;
                    break;
                }
                else if(map[y*width + x-i] > 16)
                    break;
        }
        for(int i = 1; i < 8-y; i++)
            if(map[(y+i)*width + x] == 0)
                mapMark[(y+i)*width + x] = 0;
            else if(map[(y+i)*width + x] <= 16)
            {
                mapMark[(y+i)*width + x] = 1;
                break;
            }
            else if(map[(y+i)*width + x] > 16)
                break;
        if(y > 0)
        {
            for(int i = 1; i <= y; i++)
                if(map[(y-i)*width + x] == 0)
                    mapMark[(y-i)*width +x] = 0;
                else if(map[(y-i)*width + x] <= 16)
                {
                    mapMark[(y-i)*width + x] = 1;
                    break;
                }
                else if(map[(y-i)*width + x] > 16)
                    break;
        }
    }
}

void Game::renderBlackDiagonal(int y, int x)
{
    if(lastPinnedPieceIndex != 0)
    {
        bool found = false;
        for(int i = 0; i < lastPinnedPieceIndex; i++)
        {
            if(pinnedPices[i*6] == type)
            {
                found = true;
                
                if(!check)
                {
                    int pinningDir = 0;
                    for(int j = 3; j <= 4; j++)
                        if(pinnedPices[i*6 + 1 + j] == 1)
                        {
                            pinningDir = j;
                            break;
                        }
                    
                    int lenght = 0;
                    
                    // up and left
                    if(pinningDir == 4)
                    {
                        if(x < 7 && y < 7)
                        {
                            lenght = 8 - x >= 8 - y ? 8 - y : 8 - x;
                            for(int i = 1; i < lenght; i++)
                                if(map[(y+i)*width + x+i] == 0)
                                    mapMark[(y+i)*width + x+i] = 0;
                                else if(map[(y+i)*width + x+i] <= 16)
                                {
                                    mapMark[(y+i)*width + x+i] = 1;
                                    break;
                                }
                                else break;
                        }
                        if(0 < x && 0 < y)
                        {
                            lenght = x >= y ? y+1 : x+1;
                            for(int i = 1; i < lenght; i++)
                                if(map[(y-i)*width + x-i] == 0)
                                    mapMark[(y-i)*width + x-i] = 0;
                                else if(map[(y-i)*width + x-i] <= 16)
                                {
                                    mapMark[(y-i)*width + x-i] = 1;
                                    break;
                                }
                                else break;
                        }
                    }
                    
                    // up and right
                    else if(pinningDir == 3)
                    {
                        if(0 < x && y < 7)
                        {
                            lenght = x >= 8-y ? 8-y : x+1;
                            for(int i = 1; i < lenght; i++)
                                if(map[(y+i)*width + x-i] == 0)
                                    mapMark[(y+i)*width + x-i] = 0;
                                else if(map[(y+i)*width + x-i] <= 16)
                                {
                                    mapMark[(y+i)*width + x-i] = 1;
                                    break;
                                }
                                else break;
                        }
                        if(x < 7 && 0 < y)
                        {
                            lenght = y >= 8-x ? 8-x : y+1;
                            for(int i = 1; i < lenght; i++)
                                if(map[(y-i)*width + x+i] == 0)
                                    mapMark[(y-i)*width + x+i] = 0;
                                else if(map[(y-i)*width + x+i] <= 16)
                                {
                                    mapMark[(y-i)*width + x+i] = 1;
                                    break;
                                }
                                else break;
                        }
                    }
                }
                
                break;
            }
        }
        
        if(!found)
            markBlackDiagonal(y, x);
    }
    else markBlackDiagonal(y, x);
}

void Game::markBlackDiagonal(int y, int x)
{
    if(check)
    {
        int lenght = 0;
        if(x < 7 && y < 7)
        {
            lenght = 8 - x >= 8 - y ? 8 - y : 8 - x;
            for(int i = 1; i < lenght; i++)
                if(map[(y+i)*width + x+i] == 0 && allowedMoves[(y+i)*width + x+i] == 0)
                    mapMark[(y+i)*width + x+i] = 0;
                else if(map[(y+i)*width + x+i] <= 16 && allowedMoves[(y+i)*width + x+i] == 0)
                {
                    mapMark[(y+i)*width + x+i] = 1;
                    break;
                }
                else break;
        }
        if(0 < x && 0 < y)
        {
            lenght = x >= y ? y+1 : x+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y-i)*width + x-i] == 0 && allowedMoves[(y-i)*width + x-i] == 0)
                    mapMark[(y-i)*width + x-i] = 0;
                else if(map[(y-i)*width + x-i] <= 16 && allowedMoves[(y-i)*width + x-i] == 0)
                {
                    mapMark[(y-i)*width + x-i] = 1;
                    break;
                }
                else break;
        }
        if(0 < x && y < 7)
        {
            lenght = x >= 8-y ? 8-y : x+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y+i)*width + x-i] == 0 && allowedMoves[(y+i)*width + x-i] == 0)
                    mapMark[(y+i)*width + x-i] = 0;
                else if(map[(y+i)*width + x-i] <= 16 && allowedMoves[(y+i)*width + x-i] == 0)
                {
                    mapMark[(y+i)*width + x-i] = 1;
                    break;
                }
                else break;
        }
        if(x < 7 && 0 < y)
        {
            lenght = y >= 8-x ? 8-x : y+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y-i)*width + x+i] == 0 && allowedMoves[(y-i)*width + x+i] == 0)
                    mapMark[(y-i)*width + x+i] = 0;
                else if(map[(y-i)*width + x+i] <= 16 && allowedMoves[(y-i)*width + x+i] == 0)
                {
                    mapMark[(y-i)*width + x+i] = 1;
                    break;
                }
                else break;
        }
    }
    else
    {
        int lenght = 0;
        if(x < 7 && y < 7)
        {
            lenght = 8 - x >= 8 - y ? 8 - y : 8 - x;
            for(int i = 1; i < lenght; i++)
                if(map[(y+i)*width + x+i] == 0)
                    mapMark[(y+i)*width + x+i] = 0;
                else if(map[(y+i)*width + x+i] <= 16)
                {
                    mapMark[(y+i)*width + x+i] = 1;
                    break;
                }
                else break;
        }
        if(0 < x && 0 < y)
        {
            lenght = x >= y ? y+1 : x+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y-i)*width + x-i] == 0)
                    mapMark[(y-i)*width + x-i] = 0;
                else if(map[(y-i)*width + x-i] <= 16)
                {
                    mapMark[(y-i)*width + x-i] = 1;
                    break;
                }
                else break;
        }
        if(0 < x && y < 7)
        {
            lenght = x >= 8-y ? 8-y : x+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y+i)*width + x-i] == 0)
                    mapMark[(y+i)*width + x-i] = 0;
                else if(map[(y+i)*width + x-i] <= 16)
                {
                    mapMark[(y+i)*width + x-i] = 1;
                    break;
                }
                else break;
        }
        if(x < 7 && 0 < y)
        {
            lenght = y >= 8-x ? 8-x : y+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y-i)*width + x+i] == 0)
                    mapMark[(y-i)*width + x+i] = 0;
                else if(map[(y-i)*width + x+i] <= 16)
                {
                    mapMark[(y-i)*width + x+i] = 1;
                    break;
                }
                else break;
        }
    }
}
