#include "Game.hpp"

const int width = 8;

bool Game::whiteKingCheckmate(int y, int x, int type, int kingY, int kingX, int possibleMoves, int kingsIndex)
{
    if(26 < type && type < 29)
        allowedMoves[y*width + x] = 0;
    
    else if(16 < type && type < 25)
        allowedMoves[y*width + x] = 0;
    
    else if(24 < type && type < 27)
    {
        int xDiv = x - kingX;
        int yDiv = y - kingY;
        int distance = xDiv == 0 ? std::abs(yDiv) : std::abs(xDiv);
        
        kingHorizontalAndVerticalCheckmate(xDiv, yDiv, distance, y, x);
    }
    
    else if(28 < type && type < 31)
    {
        int xDiv = x - kingX;
        int yDiv = y - kingY;
        int distance = std::abs(xDiv);
        
        kingDiagonalCheckmate(xDiv, yDiv, distance, y, x);
    }
    
    else if(type == 31)
    {
        int xDiv = x - kingX;
        int yDiv = y - kingY;
        int distance = 0;
        if(xDiv == 0)
            distance = std::abs(yDiv);
        else
            distance = std::abs(xDiv);
        
        kingHorizontalAndVerticalCheckmate(xDiv, yDiv, distance, y, x);
        kingDiagonalCheckmate(xDiv, yDiv, distance, y, x);
    }
    
    int numberOfMoves = possibleMoves;
    
    for(int i = 0; i < kingsIndex; i++)
        numberOfMoves += allowedMovesForWhiteCounter(pieces[i].dirRec.y/90, pieces[i].dirRec.x/90, pieces[i].getType());
    
    if(numberOfMoves != 0)
        return false;
    
    return true;
}

void Game::kingHorizontalAndVerticalCheckmate(int xDiv, int yDiv, int distance, int y, int x)
{
    if(xDiv > 0 && yDiv == 0)
        for(int i = 0; i < distance; i++)
            allowedMoves[y*width + x-i] = 0;
    else if(xDiv < 0 && yDiv == 0)
        for(int i = 0; i < distance; i++)
            allowedMoves[y*width + x+i] = 0;
    else if(xDiv == 0 && yDiv > 0)
        for(int i = 0; i < distance; i++)
            allowedMoves[(y-i)*width + x] = 0;
    else if(xDiv == 0 && yDiv < 0)
        for(int i = 0; i < distance; i++)
            allowedMoves[(y+i)*width + x] = 0;
}

void Game::kingDiagonalCheckmate(int xDiv, int yDiv, int distance, int y, int x)
{
    if(xDiv > 0 && yDiv > 0)
        for(int i = 0; i < distance; i++)
            allowedMoves[(y-i)*width + x-i] = 0;
    else if(xDiv < 0 && yDiv < 0)
        for(int i = 0; i < distance; i++)
            allowedMoves[(y+i)*width + x+i] = 0;
    else if(xDiv < 0 && yDiv > 0)
        for(int i = 0; i < distance; i++)
            allowedMoves[(y-i)*width + x+i] = 0;
    else if(xDiv > 0 && yDiv < 0)
        for(int i = 0; i < distance; i++)
            allowedMoves[(y+i)*width + x-i] = 0;
}

int Game::allowedMovesForWhiteCounter(int y, int x, int type)
{
    int possibleMovesCounter = 0;
    
    switch (type) {
        case 1 ... 8:
            possibleMovesCounter += countPossibleWhitePawns(y, x, type);
            break;
            
        case 9 ... 10:
            possibleMovesCounter += countPossibleWhiteHorizontalAndVertical(y, x, type);
            break;
            
        case 11 ... 12:
            possibleMovesCounter += countPossibleWhiteKnights(y, x, type);
            break;
            
        case 13 ... 14:
            possibleMovesCounter += countPossibleWhiteDiagonal(y, x, type);
            break;
            
        case 15:
            possibleMovesCounter += countPossibleWhiteDiagonal(y, x, type) + countPossibleWhiteHorizontalAndVertical(y, x, type);
            break;
            
        default:
            break;
    }
    
    return possibleMovesCounter;
}

int Game::countPossibleWhitePawns(int y, int x, int type)
{
    int possibleMovesCounter = 0;
    bool found = false;
    
    for(int i = 0; i < lastPinnedPieceIndex; i++)
        if(pinnedPices[i*6] == type)
        {
            found = true;
            break;
        }
    
    if(!found)
    {
        if(y == 6)
        {
            for(int i = 1; i < 3; i++)
                if(map[(y-i)*width + x] == 0 && allowedMoves[(y-i)*width + x] == 0)
                    possibleMovesCounter++;
        }
        else if(map[(y-1)*width + x] == 0 && allowedMoves[(y-1)*width + x] == 0)
            possibleMovesCounter++;
        if(x > 0)
            if(map[(y-1)*width + x-1] > 16 && allowedMoves[(y-1)*width + x-1] == 0)
                possibleMovesCounter++;
        if(x < 7)
            if(map[(y-1)*width + x+1] > 16 && allowedMoves[(y-1)*width + x+1] == 0)
                possibleMovesCounter++;
        
        if(y == 3 && blackEnPassantPosition == x-1)
            possibleMovesCounter++;
        else if(y == 3 && blackEnPassantPosition == x+1)
            possibleMovesCounter++;
    }
    
    return possibleMovesCounter;
}

int Game::countPossibleWhiteKnights(int y, int x, int type)
{
    int possibleMovesCounter = 0;
    bool found = false;
    
    for(int i = 0; i < lastPinnedPieceIndex; i++)
        if(pinnedPices[i*6] == type)
        {
            found = true;
            break;
        }
    
    if(!found)
    {
        if(y > 0)
        {
            if(y > 1)
            {
                if(x > 0)
                {
                    if(map[(y-2)*width + x-1] == 0 && allowedMoves[(y-2)*width + x-1] == 0)
                        possibleMovesCounter++;
                    else if(map[(y-2)*width + x-1] > 16 && allowedMoves[(y-2)*width + x-1] == 0)
                        possibleMovesCounter++;
                }
                if(x < 7)
                {
                    if(map[(y-2)*width + x+1] == 0 && allowedMoves[(y-2)*width + x+1] == 0)
                        possibleMovesCounter++;
                    else if(map[(y-2)*width + x+1] > 16 && allowedMoves[(y-2)*width + x+1] == 0)
                        possibleMovesCounter++;
                }
            }
            
            if(x > 1)
            {
                if(map[(y-1)*width + x-2] == 0 && allowedMoves[(y-1)*width + x-2] == 0)
                    possibleMovesCounter++;
                else if(map[(y-1)*width + x-2] > 16 && allowedMoves[(y-1)*width + x-2] == 0)
                    possibleMovesCounter++;
            }
            if(x < 6)
            {
                if(map[(y-1)*width + x+2] == 0 && allowedMoves[(y-1)*width + x+2] == 0)
                    possibleMovesCounter++;
                else if(map[(y-1)*width + x+2] > 16 && allowedMoves[(y-1)*width + x+2] == 0)
                    possibleMovesCounter++;
            }
        }
        if(y < 7)
        {
            if(y < 6)
            {
                if(x > 0)
                {
                    if(map[(y+2)*width + x-1] == 0 && allowedMoves[(y+2)*width + x-1] == 0)
                        possibleMovesCounter++;
                    else if(map[(y+2)*width + x-1] > 16 && allowedMoves[(y+2)*width + x-1] == 0)
                        possibleMovesCounter++;
                }
                if(x < 7)
                {
                    if(map[(y+2)*width + x+1] == 0 && allowedMoves[(y+2)*width + x+1] == 0)
                        possibleMovesCounter++;
                    else if(map[(y+2)*width + x+1] > 16 && allowedMoves[(y+2)*width + x+1] == 0)
                        possibleMovesCounter++;
                }
            }
            
            if(x > 1)
            {
                if(map[(y+1)*width + x-2] == 0 && allowedMoves[(y+1)*width + x-2] == 0)
                    possibleMovesCounter++;
                else if(map[(y+1)*width + x-2] > 16 && allowedMoves[(y+1)*width + x-2] == 0)
                    possibleMovesCounter++;
            }
            if(x < 6)
            {
                if(map[(y+1)*width + x+2] == 0 && allowedMoves[(y+1)*width + x+2] == 0)
                    possibleMovesCounter++;
                else if(map[(y+1)*width + x+2] > 16 && allowedMoves[(y+1)*width + x+2] == 0)
                    possibleMovesCounter++;
            }
        }
    }
    
    return possibleMovesCounter;
}

int Game::countPossibleWhiteHorizontalAndVertical(int y, int x, int type)
{
    int possibleMovesCounter = 0;
    bool found = false;
    
    for(int i = 0; i < lastPinnedPieceIndex; i++)
        if(pinnedPices[i*6] == type)
        {
            found = true;
            break;
        }
    
    if(!found)
    {
        for(int i = 1; i < 8-x; i++)
            if(map[y*width + x+i] == 0 && allowedMoves[y*width + x+i] == 0)
                possibleMovesCounter++;
            else if(map[y*width + x+i] > 16 && allowedMoves[y*width + x+i] == 0)
            {
                possibleMovesCounter++;
                break;
            }
            else if(0 < map[y*width + x+i] && map[y*width + x+i] <= 16)
                break;
            
        if(x > 0)
        {
            for(int i = 1; i <= x; i++)
                if(map[y*width + x-i] == 0 && allowedMoves[y*width + x-i] == 0)
                    possibleMovesCounter++;
                else if(map[y*width + x-i] > 16 && allowedMoves[y*width + x-i] == 0)
                {
                    possibleMovesCounter++;
                    break;
                }
                else if(0 < map[y*width + x-i] && map[y*width + x-i] <= 16)
                    break;
        }
        for(int i = 1; i < 8-y; i++)
            if(map[(y+i)*width + x] == 0 && allowedMoves[(y+i)*width + x] == 0)
                possibleMovesCounter++;
            else if(map[(y+i)*width + x] > 16 && allowedMoves[(y+i)*width + x] == 0)
            {
                possibleMovesCounter++;
                break;
            }
            else if(0 < map[(y+i)*width + x] && map[(y+i)*width + x] <= 16)
                break;
        if(y > 0)
        {
            for(int i = 1; i <= y; i++)
                if(map[(y-i)*width + x] == 0 && allowedMoves[(y-i)*width + x] == 0)
                    possibleMovesCounter++;
                else if(map[(y-i)*width + x] > 16 && allowedMoves[(y-i)*width + x] == 0)
                {
                    possibleMovesCounter++;
                    break;
                }
                else if(0 < map[(y-i)*width + x] && map[(y-i)*width + x] <= 16)
                    break;
        }
    }
    
    return possibleMovesCounter;
}

int Game::countPossibleWhiteDiagonal(int y, int x, int type)
{
    int possibleMovesCounter = 0;
    bool found = false;
    
    for(int i = 0; i < lastPinnedPieceIndex; i++)
        if(pinnedPices[i*6] == type)
        {
            found = true;
            break;
        }
    
    if(!found)
    {
        int lenght = 0;
        if(x < 7 && y < 7)
        {
            lenght = 8 - x >= 8 - y ? 8 - y : 8 - x;
            for(int i = 1; i < lenght; i++)
                if(map[(y+i)*width + x+i] == 0 && allowedMoves[(y+i)*width + x+i] == 0)
                    possibleMovesCounter++;
                else if(map[(y+i)*width + x+i] > 16 && allowedMoves[(y+i)*width + x+i] == 0)
                {
                    possibleMovesCounter++;
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
                    possibleMovesCounter++;
                else if(map[(y-i)*width + x-i] > 16 && allowedMoves[(y-i)*width + x-i] == 0)
                {
                    possibleMovesCounter++;
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
                    possibleMovesCounter++;
                else if(map[(y+i)*width + x-i] > 16 && allowedMoves[(y+i)*width + x-i] == 0)
                {
                    possibleMovesCounter++;
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
                    possibleMovesCounter++;
                else if(map[(y-i)*width + x+i] > 16 && allowedMoves[(y-i)*width + x+i] == 0)
                {
                    possibleMovesCounter++;
                    break;
                }
                else if(0 < map[(y-i)*width + x+i] && map[(y-i)*width + x+i] <= 16)
                    break;
        }
    }
    
    return possibleMovesCounter;
}
