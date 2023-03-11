#include "Game.hpp"

const int width = 8;

bool Game::blackKingCheckmate(int y, int x, int type, int kingY, int kingX, int possibleMoves, int kingsIndex)
{
    if(10 < type && type < 13)
        allowedMoves[y*width + x] = 0;
    
    else if(0 < type && type < 9)
        allowedMoves[y*width + x] = 0;
    
    else if(8 < type && type < 11)
    {
        int xDiv = x - kingX;
        int yDiv = y - kingY;
        int distance = xDiv == 0 ? std::abs(yDiv) : std::abs(xDiv);
        
        kingHorizontalAndVerticalCheckmate(xDiv, yDiv, distance, y, x);
    }
    
    else if(12 < type && type < 15)
    {
        int xDiv = x - kingX;
        int yDiv = y - kingY;
        int distance = std::abs(xDiv);
        
        kingDiagonalCheckmate(xDiv, yDiv, distance, y, x);
    }
    
    else if(type == 15)
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
        numberOfMoves += allowedMovesForBlackCounter(pieces[i].dirRec.y/90, pieces[i].dirRec.x/90, pieces[i].getType());
    
    return true;
}

int Game::allowedMovesForBlackCounter(int y, int x, int type)
{
    int possibleMovesCounter = 0;
    
    switch (type) {
        case 17 ... 24:
            possibleMovesCounter += countPossibleBlackPawns(y, x, type);
            break;
            
        case 25 ... 26:
            possibleMovesCounter += countPossibleBlackHorizontalAndVertical(y, x, type);
            break;

        case 27 ... 28:
            possibleMovesCounter += countPossibleBlackKnights(y, x, type);
            break;

        case 29 ... 30:
            possibleMovesCounter += countPossibleBlackDiagonal(y, x, type);
            break;

        case 31:
            possibleMovesCounter += countPossibleBlackDiagonal(y, x, type) + countPossibleBlackHorizontalAndVertical(y, x, type);
            break;
            
        default:
            break;
    }
    
    return possibleMovesCounter;
}

int Game::countPossibleBlackPawns(int y, int x, int type)
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
        if(y == 1)
        {
            for(int i = 1; i < 3; i++)
                if(map[(y+i)*width + x] == 0 && allowedMoves[(y+i)*width + x] == 0)
                    possibleMovesCounter++;
        }
        else if(map[(y+1)*width + x] == 0 && allowedMoves[(y+1)*width + x] == 0)
            possibleMovesCounter++;
        if(x > 0)
            if(map[(y+1)*width + x-1] <= 16 && map[(y+1)*width + x-1] > 0 && allowedMoves[(y+1)*width + x-1] == 0)
                possibleMovesCounter++;
        if(x < 7)
            if(map[(y+1)*width + x+1] <= 16 && map[(y+1)*width + x+1] > 0 && allowedMoves[(y+1)*width + x+1] == 0)
                possibleMovesCounter++;
        
        if(y == 4 && whiteEnPassantPosition == x-1 && allowedMoves[(y-1)*width + x-1] == 0)
            possibleMovesCounter++;
        else if(y == 4 && whiteEnPassantPosition == x+1 && allowedMoves[(y-1)*width + x+1] == 0)
            possibleMovesCounter++;
    }
    
    return possibleMovesCounter;
}

int Game::countPossibleBlackKnights(int y, int x, int type)
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
                    else if(map[(y-2)*width + x-1] <= 16 && allowedMoves[(y-2)*width + x-1] == 0)
                        possibleMovesCounter++;
                }
                if(x < 7)
                {
                    if(map[(y-2)*width + x+1] == 0 && allowedMoves[(y-2)*width + x+1] == 0)
                        possibleMovesCounter++;
                    else if(map[(y-2)*width + x+1] <= 16 && allowedMoves[(y-2)*width + x+1] == 0)
                        possibleMovesCounter++;
                }
            }
            
            if(x > 1)
            {
                if(map[(y-1)*width + x-2] == 0 && allowedMoves[(y-1)*width + x-2] == 0)
                    possibleMovesCounter++;
                else if(map[(y-1)*width + x-2] <= 16 && allowedMoves[(y-1)*width + x-2] == 0)
                    possibleMovesCounter++;
            }
            if(x < 6)
            {
                if(map[(y-1)*width + x+2] == 0 && allowedMoves[(y-1)*width + x+2] == 0)
                    possibleMovesCounter++;
                else if(map[(y-1)*width + x+2] <= 16 && allowedMoves[(y-1)*width + x+2] == 0)
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
                    else if(map[(y+2)*width + x-1] <= 16 && allowedMoves[(y+2)*width + x-1] == 0)
                        possibleMovesCounter++;
                }
                if(x < 7)
                {
                    if(map[(y+2)*width + x+1] == 0 && allowedMoves[(y+2)*width + x+1] == 0)
                        possibleMovesCounter++;
                    else if(map[(y+2)*width + x+1] <= 16 && allowedMoves[(y+2)*width + x+1] == 0)
                        possibleMovesCounter++;
                }
            }
            
            if(x > 1)
            {
                if(map[(y+1)*width + x-2] == 0 && allowedMoves[(y+1)*width + x-2] == 0)
                    possibleMovesCounter++;
                else if(map[(y+1)*width + x-2] <= 16 && allowedMoves[(y+1)*width + x-2] == 0)
                    possibleMovesCounter++;
            }
            if(x < 6)
            {
                if(map[(y+1)*width + x+2] == 0 && allowedMoves[(y+1)*width + x+2] == 0)
                    possibleMovesCounter++;
                else if(map[(y+1)*width + x+2] <= 16 && allowedMoves[(y+1)*width + x+2] == 0)
                    possibleMovesCounter++;
            }
        }
    }
    
    return possibleMovesCounter;
}

int Game::countPossibleBlackHorizontalAndVertical(int y, int x, int type)
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
            else if(map[y*width + x+i] <= 16 && allowedMoves[y*width + x+i] == 0)
            {
                possibleMovesCounter++;
                break;
            }
            else if(map[y*width + x+i] > 16 && allowedMoves[y*width + x+i] == 0)
                break;
        if(x > 0)
        {
            for(int i = 1; i <= x; i++)
                if(map[y*width + x-i] == 0 && allowedMoves[y*width + x-i] == 0)
                    possibleMovesCounter++;
                else if(map[y*width + x-i] <= 16 && allowedMoves[y*width + x-i] == 0)
                {
                    possibleMovesCounter++;
                    break;
                }
                else if(map[y*width + x-i] > 16 && allowedMoves[y*width + x-i] == 0)
                    break;
        }
        for(int i = 1; i < 8-y; i++)
            if(map[(y+i)*width + x] == 0 && allowedMoves[(y+i)*width + x] == 0)
                possibleMovesCounter++;
            else if(map[(y+i)*width + x] <= 16 && allowedMoves[(y+i)*width + x] == 0)
            {
                possibleMovesCounter++;
                break;
            }
            else if(map[(y+i)*width + x] > 16 && allowedMoves[(y+i)*width + x] == 0)
                break;
        if(y > 0)
        {
            for(int i = 1; i <= y; i++)
                if(map[(y-i)*width + x] == 0 && allowedMoves[(y-i)*width + x] == 0)
                    possibleMovesCounter++;
                else if(map[(y-i)*width + x] <= 16 && allowedMoves[(y-i)*width + x] == 0)
                {
                    possibleMovesCounter++;
                    break;
                }
                else if(map[(y-i)*width + x] > 16 && allowedMoves[(y-i)*width + x] == 0)
                    break;
        }
    }
    
    return possibleMovesCounter;
}

int Game::countPossibleBlackDiagonal(int y, int x, int type)
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
                else if(map[(y+i)*width + x+i] <= 16 && allowedMoves[(y+i)*width + x+i] == 0)
                {
                    possibleMovesCounter++;
                    break;
                }
                else break;
        }
        if(0 < x && 0 < y)
        {
            lenght = x >= y ? y+1 : x+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y-i)*width + x-i] == 0 && allowedMoves[(y-i)*width + x-i] == 0)
                    possibleMovesCounter++;
                else if(map[(y-i)*width + x-i] <= 16 && allowedMoves[(y-i)*width + x-i] == 0)
                {
                    possibleMovesCounter++;
                    break;
                }
                else break;
        }
        if(0 < x && y < 7)
        {
            lenght = x >= 8-y ? 8-y : x+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y+i)*width + x-i] == 0 && allowedMoves[(y+i)*width + x-i] == 0)
                    possibleMovesCounter++;
                else if(map[(y+i)*width + x-i] <= 16 && allowedMoves[(y+i)*width + x-i] == 0)
                {
                    possibleMovesCounter++;
                    break;
                }
                else break;
        }
        if(x < 7 && 0 < y)
        {
            lenght = y >= 8-x ? 8-x : y+1;
            for(int i = 1; i < lenght; i++)
                if(map[(y-i)*width + x+i] == 0 && allowedMoves[(y-i)*width + x+i] == 0)
                    possibleMovesCounter++;
                else if(map[(y-i)*width + x+i] <= 16 && allowedMoves[(y-i)*width + x+i] == 0)
                {
                    possibleMovesCounter++;
                    break;
                }
                else break;
        }
    }
    
    return possibleMovesCounter;
}
