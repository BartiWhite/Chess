#include "Game.hpp"

const int width = 8;

void Game::whiteKing(int y, int x, int kingsIndex)
{
    for(int i = 0; i < 8; i++)
    {
        kingMoves[i] = -1;
        kingMoves[width + i] = -1;
        kingMoves[2*width + i] = -1;
    }
    for(int i = 0; i < 30; i++)
        pinnedPices[i] = -1;
    for(int i = 0; i < 4; i++)
        piecesChecking[i] = 0;
    for(int i = 0; i < 4; i++)
        castlingPossibility[i] = true;
    
    check = false;
    lastPinnedPieceIndex = 0;
    lastCheckingPieceIndex = 0;
    pawnCheck = false;
    checkCounter = 0;
    
    // set white kings starting available moves
    whiteKingStartingMoves(y, x);
    
    // setting distance between kings
    distanceBetweenWhiteAndBlack(y, x, pieces[pieces.size()-2].dirRec.y/90, pieces[pieces.size()-2].dirRec.x/90);
    
    int iterationStart = kingsIndex + 1;
    
    for(int i = kingsIndex + 1; i < pieces.size()-2; i++)
    {
        switch (pieces[i].getType()) {
            case 17 ... 24:
                attackFromBlackPawns(i, y, x);
                break;
            case 25 ... 26:
                attackFromBlackHorizontalAndVertical(x, y, i);
                break;
            case 27 ... 28:
                attackFromKnight(i, y, x);
                break;
            case 29 ... 30:
                attackFromBlackDiagonal(x, y, i);
                break;
            case 31:
                attackFromBlackDiagonal(x, y, i);
                attackFromBlackHorizontalAndVertical(x, y, i);
                break;
                
            default:
                break;
        }
    }
    
    // checking if castling is possible
    if(!check)
        whiteKingCastling();
    else
    {
        castlingPossibility[0] = false;
        castlingPossibility[1] = false;
    }
    
    // chacking chackmate
    int possibleMoves = 0;
    for(int i = 0; i < 8; i++)
        if(kingMoves[i] == 0 || kingMoves[i] == 1)
            possibleMoves++;
    
    if(lastCheckingPieceIndex == 2 && possibleMoves == 0)
        checkmate = true;
    else if(lastCheckingPieceIndex != 2)
    {
        if(check)
            if(whiteKingCheckmate(piecesChecking[1]/10, piecesChecking[1]%10, piecesChecking[0], y, x, possibleMoves, kingsIndex))
                checkmate = true;
    }
    
    // checking stalemate
    if(possibleMoves == 0)
    {
        int numberOfMoves = 0;
        
        for(int i = 0; i < iterationStart-1; i++)
            numberOfMoves += allowedMovesForWhiteCounter(pieces[i].dirRec.y/90, pieces[i].dirRec.x/90, pieces[i].getType());
        
        if(numberOfMoves == 0)
            stalemate = true;
    }
    
    // checking draw
    checkDraw();
    
    // checking dead position
    if(numberOfEachPieceType[2] + numberOfEachPieceType[7] == 0)
        if(numberOfEachPieceType[0] == numberOfEachPieceType[5] && numberOfEachPieceType[0] > 2)
        {
            int numberOfBlocks = 0;
            
            for(int i = iterationStart; i < iterationStart + numberOfEachPieceType[5]; i++)
            {
                int y = pieces[i].dirRec.y/90;
                int x = pieces[i].dirRec.x/90;
                
                int piece = 0;
                int leftSide = 16;
                int rightSide = 16;
                if(y < 6)
                {
                    piece = map[(y+1)*width + x];
                    if(x > 0)
                        leftSide = map[(y+1)*width + x-1];
                    if(x < 7)
                        leftSide = map[(y+1)*width + x+1];
                }
                if(0 < leftSide && leftSide < 16)
                    break;
                if(0 < rightSide && rightSide < 16)
                    break;
                if(0 < piece && piece < 9)
                    numberOfBlocks++;
                
            }
            
            if(numberOfBlocks == numberOfEachPieceType[0])
            {
                if(deadGame(iterationStart-1))
                    draw = true;
            }
        }
    
    // game over
    if(checkmate || stalemate || draw)
        gameOver = true;
    if(checkmate)
        std::cout << "Black won";
    
}

void Game::distanceBetweenWhiteAndBlack(int y, int x, int blackKingY, int blackKingX)
{
    if(y == blackKingY)
    {
        if(x == blackKingX - 2)
            for(int i = 2; i < 5; i++)
                kingMoves[i] = -1;
        else if(x == blackKingX + 2)
        {
            kingMoves[6] = -1;
            kingMoves[7] = -1;
            kingMoves[0] = -1;
        }
    }
    else if(x == blackKingX)
    {
        if(y == blackKingY - 2)
            for(int i = 4; i < 7; i++)
                kingMoves[i] = -1;
        else if(y == blackKingY + 2)
            for(int i = 0; i < 3; i++)
                kingMoves[i] = -1;
    }
    else if(std::abs(x - blackKingX) + std::abs(y - blackKingY) == 3)
    {
        if(y == blackKingY + 2)
        {
            if(x == blackKingX - 1)
            {
                kingMoves[1] = -1;
                kingMoves[2] = -1;
            }
            else
            {
                kingMoves[0] = -1;
                kingMoves[1] = -1;
            }
        }
        else if(y == blackKingY - 2)
        {
            if(x == blackKingX - 1)
            {
                kingMoves[4] = -1;
                kingMoves[5] = -1;
            }
            else
            {
                kingMoves[5] = -1;
                kingMoves[6] = -1;
            }
        }
        else if(x == blackKingX - 2)
        {
            if(y == blackKingY - 1)
            {
                kingMoves[3] = -1;
                kingMoves[4] = -1;
            }
            else
            {
                kingMoves[2] = -1;
                kingMoves[3] = -1;
            }
        }
        else if(x == blackKingX + 2)
        {
            if(y == blackKingY - 1)
            {
                kingMoves[6] = -1;
                kingMoves[7] = -1;
            }
            else
            {
                kingMoves[7] = -1;
                kingMoves[0] = -1;
            }
        }
    }
    else if(std::abs(x - blackKingX) + std::abs(y - blackKingY) == 4)
    {
        int xDiv = x - blackKingX;
        int yDiv = y - blackKingY;
        
        if(xDiv == 2)
        {
            if(yDiv == 2)
                kingMoves[0] = -1;
            else kingMoves[6] = -1;
        }
        else if(xDiv == -2)
        {
            if(yDiv == 2)
                kingMoves[2] = -1;
            else kingMoves[4] = -1;
        }
    }
}

void Game::whiteKingStartingMoves(int y, int x)
{
    int piece = 0;
    int start = x > 0 ? 0 : 1;
    int stop = x < 7 ? 0 : 1;
    
    if(y > 0)
    {
        for(int i = start; i < 3 - stop; i++)
        {
            piece = map[(y-1)*width + x-1+i];
            
            if(piece == 0)
            {
                if(i == 0)
                    kingMoves[0] = 0;
                else if(i == 1)
                    kingMoves[1] = 0;
                else kingMoves[2] = 0;
            }
            else if(piece > 16)
            {
                if(i == 0)
                    kingMoves[0] = 1;
                else if(i == 1)
                    kingMoves[1] = 1;
                else kingMoves[2] = 1;
            }
        }
    }
    if(y < 7)
    {
        for(int i = start; i < 3 - stop; i++)
        {
            piece = map[(y+1)*width + x-1+i];
            
            if(piece == 0)
            {
                if(i == 0)
                    kingMoves[6] = 0;
                else if(i == 1)
                    kingMoves[5] = 0;
                else
                    kingMoves[4] = 0;
            }
            else if(piece > 16)
            {
                if(i == 0)
                    kingMoves[6] = 1;
                else if(i == 1)
                    kingMoves[5] = 1;
                else kingMoves[4] = 1;
            }
        }
    }
    if(start != 1)
    {
        piece = map[y*width + x-1];
        
        if(piece == 0)
            this->kingMoves[7] = 0;
        else if(piece > 16)
            this->kingMoves[7] = 1;
    }
    if(stop != 1)
    {
        piece = map[y*width + x+1];
        
        if(piece== 0)
            this->kingMoves[3] = 0;
        else if(piece > 16)
            this->kingMoves[3] = 1;
    }
    
    // setting moves coordinates
    for(int i = 0; i < 3; i++)
    {
        kingMoves[width + i] = y-1;
        kingMoves[2*width + i] = x-1+i;
    }
    for(int i = 0; i < 3; i++)
    {
        kingMoves[width + i+4] = y+1;
        kingMoves[2*width + i+4] = x+1-i;
    }
    kingMoves[width + 3] = y;
    kingMoves[2*width + 3] = x+1;
    kingMoves[width + 7] = y;
    kingMoves[2*width + 7] = x-1;
}

void Game::attackFromBlackPawns(int i, int y, int x)
{
    int yDiv = pieces[i].dirRec.y/90 - y;
    int xDiv = pieces[i].dirRec.x/90 - x;
    if(yDiv <= 0 && yDiv > -3 && xDiv < 3 && xDiv > -3)
    {
        if(yDiv == -2)
        {
            switch (xDiv) {
                case -2:
                    kingMoves[0] = -1;
                    break;
                case -1:
                    kingMoves[1] = -1;
                    break;
                case 0:
                    kingMoves[0] = -1;
                    kingMoves[2] = -1;
                    break;
                case 1:
                    kingMoves[1] = -1;
                    break;
                case 2:
                    kingMoves[2] = -1;
                    break;
                default:
                    break;
            }
        }
        else if(yDiv == -1)
        {
            switch (xDiv) {
                case -2:
                    kingMoves[7] = -1;
                    break;
                case -1:
                    check = true;
                    pawnCheck = true;
                    piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                    piecesChecking[lastCheckingPieceIndex*2 + 1] = (y+yDiv)*10 + x-1;
                    lastCheckingPieceIndex++;
                    break;
                case 0:
                    kingMoves[7] = -1;
                    kingMoves[3] = -1;
                    break;
                case 1:
                    check = true;
                    pawnCheck = true;
                    piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                    piecesChecking[lastCheckingPieceIndex*2 + 1] = (y+yDiv)*10 + x+1;
                    lastCheckingPieceIndex++;
                    break;
                case 2:
                    kingMoves[3] = -1;
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch (xDiv) {
                case -2:
                    kingMoves[6] = -1;
                    break;
                case -1:
                    kingMoves[5] = -1;
                    break;
                case 1:
                    kingMoves[5] = -1;
                    break;
                case 2:
                    kingMoves[4] = -1;
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::attackFromKnight(int i, int y, int x)
{
    if(std::abs(pieces[i].dirRec.x/90 - x) < 4 && std::abs(pieces[i].dirRec.y/90 - y) < 4)
    {
        int xDiv = pieces[i].dirRec.x/90 - x;
        int yDiv = pieces[i].dirRec.y/90 - y;
        
        switch(xDiv)
        {
            case 3:
                switch(yDiv){
                    case -2:
                        kingMoves[2] = -1;
                        break;
                    case -1:
                        kingMoves[3] = -1;
                        break;
                    case 0:
                        kingMoves[2] = -1;
                        kingMoves[4] = -1;
                        break;
                    case 1:
                        kingMoves[3] = -1;
                        break;
                    case 2:
                        kingMoves[4] = -1;
                        break;
                }
            break;
            case 2:
                switch (yDiv) {
                    case -3:
                        kingMoves[2] = -1;
                        break;
                    case -2:
                        kingMoves[1] = -1;
                        kingMoves[3] = -1;
                        break;
                    case -1:
                        check = true;
                        checkCounter++;
                        piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                        piecesChecking[lastCheckingPieceIndex*2 + 1] = (y+yDiv)*10 + x+2;
                        lastCheckingPieceIndex++;
                        kingMoves[4] = -1;
                        break;
                    case 0:
                        kingMoves[1] = -1;
                        kingMoves[5] = -1;
                        break;
                    case 1:
                        check = true;
                        checkCounter++;
                        piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                        piecesChecking[lastCheckingPieceIndex*2 + 1] = (y+yDiv)*10 + x+2;
                        lastCheckingPieceIndex++;
                        kingMoves[2] = -1;
                        break;
                    case 2:
                        kingMoves[3] = -1;
                        kingMoves[5] = -1;
                        break;
                    case 3:
                        kingMoves[4] = -1;
                        break;
                }
                break;
            case 1:
                switch (yDiv) {
                    case -3:
                        kingMoves[1] = -1;
                        break;
                    case -2:
                        kingMoves[0] = -1;
                        check = true;
                        checkCounter++;
                        piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                        piecesChecking[lastCheckingPieceIndex*2 + 1] = (y+yDiv)*10 + x+1;
                        lastCheckingPieceIndex++;
                        break;
                    case -1:
                        kingMoves[7] = -1;
                        kingMoves[5] = -1;
                        break;
                    case 0:
                        kingMoves[0] = -1;
                        kingMoves[6] = -1;
                        break;
                    case 1:
                        kingMoves[1] = -1;
                        kingMoves[7] = -1;
                        break;
                    case 2:
                        kingMoves[6] = -1;
                        check = true;
                        checkCounter++;
                        piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                        piecesChecking[lastCheckingPieceIndex*2 + 1] = (y+yDiv)*10 + x+1;
                        lastCheckingPieceIndex++;
                        break;
                    case 3:
                        kingMoves[5] = -1;
                        break;
                }
                break;
            case 0:
                switch (yDiv) {
                    case -3:
                        kingMoves[0] = -1;
                        kingMoves[2] = -1;
                        break;
                    case -2:
                        kingMoves[7] = -1;
                        kingMoves[3] = -1;
                        break;
                    case -1:
                        kingMoves[6] = -1;
                        kingMoves[4] = -1;
                        break;
                    case 1:
                        kingMoves[0] = -1;
                        kingMoves[2] = -1;
                        break;
                    case 2:
                        kingMoves[7] = -1;
                        kingMoves[3] = -1;
                        break;
                    case 3:
                        kingMoves[6] = -1;
                        kingMoves[4] = -1;
                        break;
                }
                break;
            case -1:
                switch (yDiv) {
                    case -3:
                        kingMoves[1] = -1;
                        break;
                    case -2:
                        kingMoves[2] = -1;
                        check = true;
                        checkCounter++;
                        piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                        piecesChecking[lastCheckingPieceIndex*2 + 1] = (y+yDiv)*10 + x-1;
                        lastCheckingPieceIndex++;
                        break;
                    case -1:
                        kingMoves[3] = -1;
                        kingMoves[5] = -1;
                        break;
                    case 0:
                        kingMoves[2] = -1;
                        kingMoves[4] = -1;
                        break;
                    case 1:
                        kingMoves[1] = -1;
                        kingMoves[3] = -1;
                        break;
                    case 2:
                        kingMoves[4] = -1;
                        check = true;
                        checkCounter++;
                        piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                        piecesChecking[lastCheckingPieceIndex*2 + 1] = (y+yDiv)*10 + x-1;
                        lastCheckingPieceIndex++;
                        break;
                    case 3:
                        kingMoves[5] = -1;
                        break;
                }
                break;
            case -2:
                switch (yDiv) {
                    case -3:
                        kingMoves[0] = -1;
                        break;
                    case -2:
                        kingMoves[1] = -1;
                        kingMoves[7] = -1;
                        break;
                    case -1:
                        check = true;
                        checkCounter++;
                        piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                        piecesChecking[lastCheckingPieceIndex*2 + 1] = (y+yDiv)*10 + x-2;
                        lastCheckingPieceIndex++;
                        kingMoves[6] = -1;
                        break;
                    case 0:
                        kingMoves[1] = -1;
                        kingMoves[5] = -1;
                        break;
                    case 1:
                        check = true;
                        checkCounter++;
                        piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                        piecesChecking[lastCheckingPieceIndex*2 + 1] = (y+yDiv)*10 + x-2;
                        lastCheckingPieceIndex++;
                        kingMoves[0] = -1;
                        break;
                    case 2:
                        kingMoves[7] = -1;
                        kingMoves[5] = -1;
                        break;
                    case 3:
                        kingMoves[6] = -1;
                        break;
                }
                break;
            case -3:
                switch (yDiv) {
                    case -2:
                        kingMoves[0] = -1;
                        break;
                    case -1:
                        kingMoves[7] = -1;
                        break;
                    case 0:
                        kingMoves[0] = -1;
                        kingMoves[6] = -1;
                        break;
                    case 1:
                        kingMoves[7] = -1;
                        break;
                    case 2:
                        kingMoves[6] = -1;
                        break;
                }
                break;
        }
    }
}

void Game::whiteKingCastling()
{
    if(!kingsAndRooksMoved[0] && !kingsAndRooksMoved[1] && map[57] == 0 && map[58] == 0 && map[59] == 0)
    {
        if(map[41] != 27 && map[41] != 28 && map[42] != 27 && map[42] != 28
           && map[44] != 27 && map[44] != 28 && map[48] != 27 && map[48] != 28
           && map[49] != 27 && map[49] != 28 && map[52] != 27 && map[52] != 28
           && map[53] != 27 && map[53] != 28)
        {
            for(int i = 58; i < 60; i++)
                whiteCastlingPrevention(i, 0);
        }
        else castlingPossibility[0] = false;
    }
    else castlingPossibility[0] = false;
    
    if(!kingsAndRooksMoved[0] && !kingsAndRooksMoved[2] && map[61] == 0 && map[62] == 0)
    {
        if(map[44] != 27 && map[44] != 28 && map[46] != 27 && map[46] != 28
           && map[47] != 27 && map[47] != 28 && map[51] != 27 && map[51] != 28
           && map[52] != 27 && map[52] != 28 && map[55] != 27 && map[55] != 28)
        {
            for(int i = 61; i < 63; i++)
                whiteCastlingPrevention(i, 1);
        }
        else castlingPossibility[1] = false;
    }
    else castlingPossibility[1] = false;
}

void Game::whiteCastlingPrevention(int i, int side)
{
    const int width = 8;
    int y = i/8;
    int x = i%8;
    int lenght = x < y ? x : y;
    
    // up and left
    for(int j = 1; j <= lenght; j++)
    {
        int piece = map[(y-j)*width + x-j];
        
        if((0 < piece && piece < 16) || (24 < piece && piece < 29) || piece == 32)
            break;
        else if((16 < piece && piece < 25) || (28 < piece && piece < 32))
        {
            if(16 < piece && piece < 25)
            {
                if(j == 1)
                    castlingPossibility[side] = false;
                break;
            }
            castlingPossibility[side] = false;
            break;
        }
    }
    // up
    for(int j = 6; j >= 0; j--)
    {
        int piece = map[j*width + x];
        
        if((0 < piece && piece < 16) || (16 < piece && piece < 25) || (26 < piece && piece < 31) || piece == 32)
            break;
        else if((24 < piece && piece < 27) || piece == 31)
        {
            castlingPossibility[side] = false;
            break;
        }
    }
    lenght = 7 - x < y ? 7 - x : y;
    // up and right
    for(int j = 1; j <= lenght; j++)
    {
        int piece = map[(y-j)*width + x+j];
        
        if((0 < piece && piece < 16) || (24 < piece && piece < 29) || piece == 32)
            break;
        else if((16 < piece && piece < 25) || (28 < piece && piece < 32))
        {
            if(16 < piece && piece < 25)
            {
                if(j == 1)
                    castlingPossibility[side] = false;
                break;
            }
            castlingPossibility[side] = false;
            break;
        }
    }
}

void Game::checkDraw()
{
    if(pieces.size() < 6)
    {
        if(numberOfEachPieceType[0] == 0 && numberOfEachPieceType[1] == 0 && numberOfEachPieceType[4] == 0
           && numberOfEachPieceType[5] == 0 && numberOfEachPieceType[6] == 0 && numberOfEachPieceType[9] == 0)
        {
            if(numberOfEachPieceType[7] == 0 && numberOfEachPieceType[8] == 0)
            {
                if(numberOfEachPieceType[2] == 1 && numberOfEachPieceType[3] == 0)
                    draw = true;
                else if(numberOfEachPieceType[2] == 0 && numberOfEachPieceType[3] == 1)
                    draw = true;
            }
            else if(numberOfEachPieceType[2] == 0 && numberOfEachPieceType[3] == 0)
            {
                if(numberOfEachPieceType[7] == 1 && numberOfEachPieceType[8] == 0)
                    draw = true;
                else if(numberOfEachPieceType[7] == 0 && numberOfEachPieceType[8] == 1)
                    draw = true;
            }
            else if(numberOfEachPieceType[3] == 1 && numberOfEachPieceType[7] == 1)
                draw = true;
            else if(numberOfEachPieceType[1] == 1 && numberOfEachPieceType[8] == 1)
                draw = true;
            
            else if(numberOfEachPieceType[3] == 1 && numberOfEachPieceType[8] == 1)
            {
                int whiteBishop = -1;
                int blackBishop = -1;
                
                // 0 - light-square , 1 - dark-square
                whiteBishop = ((pieces[0].dirRec.y/90)%2 == (pieces[0].dirRec.x/90)%2) ? 0 : 1;
                blackBishop = ((pieces[2].dirRec.y/90)%2 == (pieces[2].dirRec.x/90)%2) ? 0 : 1;
                
                if(whiteBishop == blackBishop)
                    draw = true;
            }
        }
        
        if(pieces.size() < 4)
            draw = true;
    }
}

bool Game::deadGame(int kingsIndex)
{
    const int width = 8;
    
    if(numberOfEachPieceType[4] == 1 || numberOfEachPieceType[9] == 1) // set it at the very begining
        return false;
    
    if(numberOfEachPieceType[0] != 8 && (numberOfEachPieceType[1] != 0 || numberOfEachPieceType[6] != 0))
        return false;
    
    if(numberOfEachPieceType[0] < 6 && (numberOfEachPieceType[3] == 2 || numberOfEachPieceType[8] == 2))
        return false;
    
    int unbreakableLine[48];
    for(int i = 0; i < 48; i++)
        unbreakableLine[i] = 0;
    
    // boundry for white
    for(int i = 0; i < numberOfEachPieceType[0]; i++)
    {
        int y = pieces[i].dirRec.y/90-1;
        int x = pieces[i].dirRec.x/90;
        unbreakableLine[y*width + x] = 1;
        if(0 < x && x <= 7)
            unbreakableLine[y*width + x-1] = 2;
        if(0 <= x && x < 7)
            unbreakableLine[y*width + x+1] = 2;
    }
    
    int searchStart = -1;
    for(int i = 0; i < 6; i++)
        if(unbreakableLine[i*width] == 1 || unbreakableLine[i*width] == 2)
        {
            searchStart = i*width;
            break;
        }
    
    if(searchStart == -1)
        return false;
    
    bool up = true;
    bool down = true;
    int positionChange = searchStart;
    
    do
    {
        positionChange = searchStart;
        
        if(unbreakableLine[searchStart+1] == 1 || unbreakableLine[searchStart+1] == 2)
        {
            searchStart++;
            up = true;
            down = true;
        }
        else
        {
            if(searchStart > 7 && down)
            {
                if((unbreakableLine[searchStart-width] == 1 || unbreakableLine[searchStart-width] == 2) && up)
                {
                    searchStart = searchStart-width;
                    up = false;
                }
            }
            if(searchStart < 40 && up)
            {
                if((unbreakableLine[searchStart+width] == 1 || unbreakableLine[searchStart+width] == 2) && down)
                {
                    searchStart = searchStart+width;
                    down = false;
                }
            }
        }
        
        if(positionChange == searchStart)
            return false;
        
    }while(searchStart%8 != 7);
    
    if(numberOfEachPieceType[3] == 0 && numberOfEachPieceType[8] == 0)
        return true;
    
    if(numberOfEachPieceType[0] != 8)
        return false;
        
    if(numberOfEachPieceType[3] == 2 || numberOfEachPieceType[8] == 2)
        return false;
    else if(numberOfEachPieceType[3] == 1) // white bishop
    {
        int searchStart = (pieces[0].dirRec.y/90 - 1)*width;
        int parity = 0;
        int y = 0;
        int x = 0;
        for(int i = 8; i < kingsIndex; i++)
            if(12 < pieces[i].getType() && pieces[i].getType() < 15)
            {
                y = (pieces[i].dirRec.y/90)%2;
                x = (pieces[i].dirRec.x/90)%2;
                parity = (y == x) ? 0 : 1; // 0 - light-square , 1 - dark-square
            }
        
        if(((searchStart+width)/width)%2 != parity)
            return false;
        
        int positionChange = searchStart;
        
        do
        {
            positionChange = searchStart;
            
            if(searchStart > 7)
            {
                if(unbreakableLine[searchStart-width + 1] == 1)
                    searchStart = searchStart-width + 1;
            }
            if(searchStart < 40)
            {
                if(unbreakableLine[searchStart+width + 1] == 1)
                    searchStart = searchStart+width + 1;
            }
            
            if(positionChange == searchStart)
                return false;
            
        }while(searchStart%8 != 7);
    }
    else if(numberOfEachPieceType[8] == 1) // black bishop
    {
        for(int i = kingsIndex+1; i < kingsIndex + 1 + numberOfEachPieceType[5]; i++)
            unbreakableLine[(pieces[i].dirRec.y/90-1)*width + pieces[i].dirRec.x/90] = 3;
        
        int searchStart = (pieces[kingsIndex+1].dirRec.y/90 - 1)*width;
        int parity = 0;
        int y = 0;
        int x = 0;
        for(int i = kingsIndex+9; i < pieces.size()-2; i++)
            if(28 < pieces[i].getType() && pieces[i].getType() < 31)
            {
                y = (pieces[i].dirRec.y/90)%2;
                x = (pieces[i].dirRec.x/90)%2;
                parity = (y == x) ? 0 : 1; // 0 - light-square , 1 - dark-square
            }
        
        if(((searchStart+width)/width)%2 != parity)
            return false;
        
        int positionChange = searchStart;
        
        do
        {
            positionChange = searchStart;
            
            if(searchStart > 7)
            {
                if(unbreakableLine[searchStart-width + 1] == 3)
                    searchStart = searchStart-width + 1;
            }
            if(searchStart < 40)
            {
                if(unbreakableLine[searchStart+width + 1] == 3)
                    searchStart = searchStart+width + 1;
            }
            
            if(positionChange == searchStart)
                return false;
            
        }while(searchStart%8 != 7);
    }
    
    return true;
}

void Game::attackFromBlackHorizontalAndVertical(int x, int y, int i)
{
    int piece = 0;
    int cover = 0;
    int tmpCover = 0;
    
    int start = x > 0 ? 0 : 1;
    int ystart = y > 0 ? 0 : 1;
    int stop = x < 7 ? 0 : 1;
    int ystop = y < 7 ? 0 : 1;
    
    int xDiv = pieces[i].dirRec.x/90 - x;
    int yDiv = pieces[i].dirRec.y/90 - y;
    if(yDiv > -2 && yDiv < 2)
    {
        if(yDiv == 0)
        {
            if(xDiv > 0)
            {
                for(int j = 1; j <= xDiv; j++)
                {
                    piece = map[y*width + x+xDiv-j];
                    if(piece > 16)
                    {
                        if(kingMoves[2*width + 3] == x+xDiv-j)
                            kingMoves[3] = -1;
                        break;
                    }
                    else if(piece > 0 && piece < 16)
                    {
                        cover++;
                        if(cover > 1)
                            break;
                        tmpCover = y*10 + x+xDiv-j;
                    }
                    else if(piece == 16)
                    {
                        if(cover == 0)
                        {
                            check = true;
                            checkCounter++;
                            piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                            piecesChecking[lastCheckingPieceIndex*2 + 1] = (pieces[i].dirRec.y/90)*10 + pieces[i].dirRec.x/90;
                            lastCheckingPieceIndex++;
                            if(pieces[i].dirRec.x/90 != x+1)
                                kingMoves[3] = -1;
                            if(start == 0)
                                kingMoves[7] = -1;
                        }
                        else
                        {
                            for(int j = 0; j < 16; j++)
                                if(pieces[j].dirRec.y/90 == tmpCover/10 && pieces[j].dirRec.x/90 == tmpCover%10)
                                {
                                    pinePiece(pieces[j].getType(), tmpCover, 2);
                                    break;
                                }
                        }
                    }
                }
            }
            else if(xDiv < 0)
            {
                for(int j = x+xDiv+1; j <= x; j++)
                {
                    piece = map[y*width + j];
                    if(piece > 16)
                    {
                        if(kingMoves[2*width + 7] == j)
                            kingMoves[7] = -1;
                        break;
                    }
                    else if(piece > 0 && piece < 16)
                    {
                        cover++;
                        if(cover > 1)
                            break;
                        tmpCover = y*10 + j;
                    }
                    else if(piece == 16)
                    {
                        if(cover == 0)
                        {
                            check = true;
                            checkCounter++;
                            piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                            piecesChecking[lastCheckingPieceIndex*2 + 1] = (pieces[i].dirRec.y/90)*10 + pieces[i].dirRec.x/90;
                            lastCheckingPieceIndex++;
                            if(pieces[i].dirRec.x/90 != x-1)
                                kingMoves[7] = -1;
                            if(stop == 0)
                                kingMoves[3] = -1;
                        }
                        else
                        {
                            for(int j = 0; j < 16; j++)
                                if(pieces[j].dirRec.y/90 == tmpCover/10 && pieces[j].dirRec.x/90 == tmpCover%10)
                                {
                                    pinePiece(pieces[j].getType(), tmpCover, 2);
                                    break;
                                }
                        }
                    }
                }
            }
        }
        else if(yDiv == 1)
        {
            if(xDiv > 0)
            {
                for(int j = x+xDiv-1; j >= x-1+start; j--)
                {
                    piece = map[(y+1)*width + j];
                    if(piece > 0)
                    {
                        for(int k = 0; k < 3; k++)
                            if(kingMoves[2*width + 4+k] == j)
                            {
                                kingMoves[4+k] = -1;
                                break;
                            }
                        break;
                    }
                    if(stop == 0)
                    {
                        if(kingMoves[width + 4] == y+1 && kingMoves[2*width + 4] == j)
                            kingMoves[4] = -1;
                    }
                    if(kingMoves[width + 5] == y+1 && kingMoves[2*width + 5] == j)
                        kingMoves[5] = -1;
                    if(start == 0)
                    {
                        if(kingMoves[width + 6] == y+1 && kingMoves[2*width + 6] == j)
                            kingMoves[6] = -1;
                    }
                }
            }
            else if(xDiv < 0)
            {
                for(int j = x+xDiv+1; j <= x+1-stop; j++)
                {
                    piece = map[(y+1)*width + j];
                    if(piece > 0)
                    {
                        for(int k = 0; k < 3; k++)
                            if(kingMoves[2*width + 6-k] == j)
                            {
                                kingMoves[6-k] = -1;
                                break;
                            }
                        break;
                    }
                    if(stop == 0)
                    {
                        if(kingMoves[width + 4] == y+1 && kingMoves[2*width + 4] == j)
                            kingMoves[4] = -1;
                    }
                    if(kingMoves[width + 5] == y+1 && kingMoves[2*width + 5] == j)
                        kingMoves[5] = -1;
                    if(start == 0)
                    {
                        if(kingMoves[width + 6] == y+1 && kingMoves[2*width + 6] == j)
                            kingMoves[6] = -1;
                    }
                }
            }
            else if(xDiv == 0)
            {
                if(start == 0)
                    kingMoves[6] = -1;
                if(stop == 0)
                    kingMoves[4] = -1;
            }
        }
        else if(yDiv == -1)
        {
            if(xDiv > 0)
            {
                for(int j = x+xDiv-1; j >= x-1+start; j--)
                {
                    piece = map[(y-1)*width + j];
                    if(piece > 0)
                    {
                        for(int k = 0; k < 3; k++)
                            if(kingMoves[2*width + 2-k] == j)
                            {
                                kingMoves[2-k] = -1;
                                break;
                            }
                        break;
                    }
                    if(stop == 0)
                    {
                        if(kingMoves[width + 2] == y-1 && kingMoves[2*width + 2] == j)
                            kingMoves[2] = -1;
                    }
                    if(kingMoves[width + 1] == y-1 && kingMoves[2*width + 1] == j)
                        kingMoves[1] = -1;
                    if(start == 0)
                    {
                        if(kingMoves[width] == y-1 && kingMoves[2*width] == j)
                            kingMoves[0] = -1;
                    }
                }
            }
            else if(xDiv < 0)
            {
                for(int j = x+xDiv+1; j <= x+1-stop; j++)
                {
                    piece = map[(y-1)*width + j];
                    if(piece > 0)
                    {
                        for(int k = 0; k < 3; k++)
                            if(kingMoves[2*width + k] == j)
                            {
                                kingMoves[k] = -1;
                                break;
                            }
                        break;
                    }
                    if(stop == 0)
                    {
                        if(kingMoves[width + 2] == y-1 && kingMoves[2*width + 2] == j)
                            kingMoves[2] = -1;
                    }
                    if(kingMoves[width + 1] == y-1 && kingMoves[2*width + 1] == j)
                        kingMoves[1] = -1;
                    if(start == 0)
                    {
                        if(kingMoves[width] == y-1 && kingMoves[2*width] == j)
                            kingMoves[0] = -1;
                    }
                }
            }
            else if(xDiv == 0)
            {
                if(start == 0)
                    kingMoves[0] = -1;
                if(stop == 0)
                    kingMoves[2] = -1;
            }
        }
    }
    if(xDiv > -2 && xDiv < 2)
    {
        if(xDiv == 0)
        {
            if(yDiv > 0)
            {
                for(int j = 1; j <= yDiv; j++)
                {
                    piece = map[(y+yDiv-j)*width + x];
                    if(piece > 16)
                    {
                        if(kingMoves[width + 5] == y+yDiv-j)
                            kingMoves[5] = -1;
                        break;
                    }
                    else if(piece > 0 && piece < 16)
                    {
                        cover++;
                        if(cover > 1)
                            break;
                        tmpCover = (y+yDiv-j)*10 + x;
                    }
                    else if(piece == 16)
                    {
                        if(cover == 0)
                        {
                            check = true;
                            checkCounter++;
                            piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                            piecesChecking[lastCheckingPieceIndex*2 + 1] = (pieces[i].dirRec.y/90)*10 + pieces[i].dirRec.x/90;
                            lastCheckingPieceIndex++;
                            if(pieces[i].dirRec.y/90 != y+1)
                                kingMoves[5] = -1;
                            if(ystart == 0)
                                kingMoves[1] = -1;
                        }
                        else
                        {
                            for(int j = 0; j < 16; j++)
                                if(pieces[j].dirRec.y/90 == tmpCover/10 && pieces[j].dirRec.x/90 == tmpCover%10)
                                {
                                    pinePiece(pieces[j].getType(), tmpCover, 1);
                                    break;
                                }
                        }
                    }
                }
            }
            else if(yDiv < 0)
            {
                for(int j = y+yDiv+1; j <= y; j++)
                {
                    piece = map[j*width + x];
                    if(piece > 16)
                    {
                        if(kingMoves[width + 1] == j)
                            kingMoves[1] = -1;
                        break;
                    }
                    else if(piece > 0 && piece < 16)
                    {
                        cover++;
                        if(cover > 1)
                            break;
                        tmpCover = j*10 + x;
                    }
                    else if(piece == 16)
                    {
                        if(cover == 0)
                        {
                            check = true;
                            checkCounter++;
                            piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                            piecesChecking[lastCheckingPieceIndex*2 + 1] = (pieces[i].dirRec.y/90)*10 + pieces[i].dirRec.x/90;
                            lastCheckingPieceIndex++;
                            if(pieces[i].dirRec.y/90 != y-1)
                                kingMoves[1] = -1;
                            if(ystop == 0)
                                kingMoves[5] = -1;
                        }
                        else
                        {
                            for(int j = 0; j < 16; j++)
                                if(pieces[j].dirRec.y/90 == tmpCover/10 && pieces[j].dirRec.x/90 == tmpCover%10)
                                {
                                    pinePiece(pieces[j].getType(), tmpCover, 1);
                                    break;
                                }
                        }
                    }
                }
            }
        }
        else if(xDiv == 1)
        {
            if(yDiv > 0)
            {
                for(int j = y+yDiv-1; j >= y-1+ystart; j--)
                {
                    piece = map[j*width + x+1];
                    if(piece > 0)
                    {
                        for(int k = 0; k < 3; k++)
                            if(kingMoves[width + 4-k] == j)
                            {
                                kingMoves[4-k] = -1;
                                break;
                            }
                        break;
                    }
                    if(ystop == 0)
                    {
                        if(kingMoves[width + 4] == j && kingMoves[2*width + 4] == x+1)
                            kingMoves[4] = -1;
                    }
                    if(kingMoves[width + 3] == j && kingMoves[2*width + 3] == x+1)
                        kingMoves[3] = -1;
                    if(ystart == 0)
                    {
                        if(kingMoves[width + 2] == j && kingMoves[2*width + 2] == x+1)
                            kingMoves[2] = -1;
                    }
                }
            }
            else if(yDiv < 0)
            {
                for(int j = y+yDiv+1; j <= y+1-ystop; j++)
                {
                    piece = map[j*width + x+1];
                    if(piece > 0)
                    {
                        for(int k = 0; k < 3; k++)
                            if(kingMoves[width + 2+k] == j)
                            {
                                kingMoves[2+k] = -1;
                                break;
                            }
                        break;
                    }
                    if(ystop == 0)
                    {
                        if(kingMoves[width + 4] == j && kingMoves[2*width + 4] == x+1)
                            kingMoves[4] = -1;
                    }
                    if(kingMoves[width + 3] == j && kingMoves[2*width + 3] == x+1)
                        kingMoves[3] = -1;
                    if(ystart == 0)
                    {
                        if(kingMoves[width + 2] == j && kingMoves[2*width + 2] == x+1)
                            kingMoves[2] = -1;
                    }
                }
            }
            else if(yDiv == 0)
            {
                if(ystart == 0)
                    kingMoves[2] = -1;
                if(ystop == 0)
                    kingMoves[4] = -1;
            }
        }
        else if(xDiv == -1)
        {
            if(yDiv > 0)
            {
                for(int j = y+yDiv-1; j >= y-1+ystart; j--)
                {
                    piece = map[j*width + x-1];
                    if(piece > 0)
                    {
                        for(int k = 0; k < 3; k++)
                            if(kingMoves[width + 6+k - (k/2)*8] == j)
                            {
                                kingMoves[6+k - (k/2)*8] = -1;
                                break;
                            }
                        break;
                    }
                    if(ystop == 0)
                    {
                        if(kingMoves[width + 6] == j && kingMoves[2*width + 6] == x-1)
                            kingMoves[6] = -1;
                    }
                    if(kingMoves[width + 7] == j && kingMoves[2*width + 7] == x-1)
                        kingMoves[7] = -1;
                    if(ystart == 0)
                    {
                        if(kingMoves[width] == j && kingMoves[2*width] == x-1)
                            kingMoves[0] = -1;
                    }
                }
            }
            else if(yDiv < 0)
            {
                for(int j = y+yDiv+1; j <= y+1-ystop; j++)
                {
                    piece = map[j*width + x-1];
                    if(piece > 0)
                    {
                        for(int k = 0; k < 3; k++)
                            if(kingMoves[width + 6+k - (k/2)*8] == j)
                            {
                                kingMoves[6+k - (k/2)*8] = -1;
                                break;
                            }
                        break;
                    }
                    if(ystop == 0)
                    {
                        if(kingMoves[width + 6] == j && kingMoves[2*width + 6] == x-1)
                            kingMoves[6] = -1;
                    }
                    if(kingMoves[width + 7] == j && kingMoves[2*width + 7] == x-1)
                        kingMoves[7] = -1;
                    if(ystart == 0)
                    {
                        if(kingMoves[width] == j && kingMoves[2*width] == x-1)
                            kingMoves[0] = -1;
                    }
                }
            }
            else if(yDiv == 0)
            {
                if(ystart == 0)
                    kingMoves[0] = -1;
                if(ystop == 0)
                    kingMoves[6] = -1;
            }
        }
    }
}

void Game::attackFromBlackDiagonal(int x, int y, int i)
{
    int piece = 0;
    int cover = 0;
    int tmpCover = 0;
    
    int start = x > 0 ? 0 : 1;
    int ystart = y > 0 ? 0 : 1;
    int stop = x < 7 ? 0 : 1;
    int ystop = y < 7 ? 0 : 1;
    
    int xDiv = pieces[i].dirRec.x/90 - x;
    int yDiv = pieces[i].dirRec.y/90 - y;
    int relativePosition = std::abs(xDiv) - std::abs(yDiv);

    if(relativePosition < 3)
    {
        if(xDiv >= 0 && yDiv >= 0)
        {
            switch (relativePosition) {
                case 2:
                    if(ystart == 0)
                    {
                        for(int j = 1; j <= yDiv + 1; j++)
                        {
                            piece = map[(y+yDiv-j)*width + x+xDiv-j];
                            
                            if(piece > 0)
                            {
                                if(kingMoves[width + 2] == y+yDiv-j && kingMoves[2*width + 2] == x+xDiv-j)
                                    kingMoves[2] = -1;
                                break;
                            }
                            else if(kingMoves[width + 2] == y+yDiv-j && kingMoves[2*width + 2] == x+xDiv-j)
                                kingMoves[2] = -1;
                        }
                    }
                    break;
                case 1:
                    for(int j = 1; j <= yDiv + 1 - ystart; j++)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv-j];
                        
                        if(piece > 0)
                        {
                            for(int k = 0; k < 2; k++)
                            if(kingMoves[width + 3 - k*2] == y+yDiv-j && kingMoves[2*width + 3 - k*2] == x+xDiv-j)
                                {
                                    kingMoves[3 - k*2] = -1;
                                    break;
                                }
                            break;
                        }
                        else if(kingMoves[width + 3] == y+yDiv-j && kingMoves[2*width + 3] == x+xDiv-j)
                            kingMoves[3] = -1;
                        else if(kingMoves[width + 1] == y+yDiv-j && kingMoves[2*width + 1] == x+xDiv-j && ystart == 0)
                            kingMoves[1] = -1;
                    }
                    break;
                case 0:
                    for(int j = 1; j <= yDiv + 1; j++)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv-j];
                        
                        if(piece == 0 && kingMoves[width + 4] == y+yDiv-j && kingMoves[2*width + 4] == x+xDiv-j && cover == 0)
                        {
                            kingMoves[4] = -1;
                        }
                        else if(piece > 16)
                        {
                            if(kingMoves[width + 4] == y+yDiv-j && kingMoves[2*width + 4] == x+xDiv-j)
                                kingMoves[4] = -1;
                            break;
                        }
                        else if(piece > 0 && piece < 16)
                        {
                            cover++;
                            if(cover > 1)
                                break;
                            tmpCover = (y+yDiv-j)*10 + x+xDiv-j;
                        }
                        else if(piece == 16)
                        {
                            if(cover == 0)
                            {
                                check = true;
                                checkCounter++;
                                piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                                piecesChecking[lastCheckingPieceIndex*2 + 1] = (pieces[i].dirRec.y/90)*10 + pieces[i].dirRec.x/90;
                                lastCheckingPieceIndex++;
                                if(ystart == 0)
                                    kingMoves[0] = -1;
                            }
                            else
                            {
                                for(int j = 0; j < 16; j++)
                                    if(pieces[j].dirRec.y/90 == tmpCover/10 && pieces[j].dirRec.x/90 == tmpCover%10)
                                    {
                                        pinePiece(pieces[j].getType(), tmpCover, 4);
                                        break;
                                    }
                            }
                        }
                    }
                    break;
                case -1:
                    for(int j = 1; j <= xDiv + 1 - start; j++)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv-j];
                        
                        if(piece > 0)
                        {
                            for(int k = 0; k < 2; k++)
                            if(kingMoves[width + 5 + k*2] == y+yDiv-j && kingMoves[2*width + 5 + k*2] == x+xDiv-j)
                                {
                                    kingMoves[5 + k*2] = -1;
                                    break;
                                }
                            break;
                        }
                        else if(kingMoves[width + 5] == y+yDiv-j && kingMoves[2*width + 5] == x+xDiv-j)
                            kingMoves[5] = -1;
                        else if(kingMoves[width + 7] == y+yDiv-j && kingMoves[2*width + 7] == x+xDiv-j && start == 0)
                            kingMoves[7] = -1;
                    }
                    break;
                case -2:
                    if(start == 0)
                    {
                        for(int j = 1; j <= xDiv + 1; j++)
                        {
                            piece = map[(y+yDiv-j)*width + x+xDiv-j];
                            
                            if(piece > 0)
                            {
                                if(kingMoves[width + 6] == y+yDiv-j && kingMoves[2*width + 6] == x+xDiv-j)
                                    kingMoves[6] = -1;
                                break;
                            }
                            else if(kingMoves[width + 6] == y+yDiv-j && kingMoves[2*width + 6] == x+xDiv-j)
                                kingMoves[6] = -1;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        if(xDiv <= 0 && yDiv <= 0)
        {
            switch (relativePosition) {
                case -2:
                    if(stop == 0)
                    {
                        for(int j = -1; j >= yDiv + 1; j--)
                        {
                            piece = map[(y+yDiv-j)*width + x+xDiv-j];
                            
                            if(piece > 0)
                            {
                                if(kingMoves[width + 2] == y+yDiv-j && kingMoves[2*width + 2] == x+xDiv-j)
                                    kingMoves[2] = -1;
                                break;
                            }
                            else if(kingMoves[width + 2] == y+yDiv-j && kingMoves[2*width + 2] == x+xDiv-j)
                                kingMoves[2] = -1;
                        }
                    }
                    break;
                case -1:
                    for(int j = -1; j >= yDiv + stop; j--)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv-j];
                        
                        if(piece > 0)
                        {
                            for(int k = 0; k < 2; k++)
                            if(kingMoves[width + 1 + k*2] == y+yDiv-j && kingMoves[2*width + 1 + k*2] == x+xDiv-j)
                                {
                                    kingMoves[1 + k*2] = -1;
                                    break;
                                }
                            break;
                        }
                        else if(kingMoves[width + 1] == y+yDiv-j && kingMoves[2*width + 1] == x+xDiv-j)
                            kingMoves[1] = -1;
                        else if(kingMoves[width + 3] == y+yDiv-j && kingMoves[2*width + 3] == x+xDiv-j && stop == 0)
                            kingMoves[3] = -1;
                    }
                    break;
                case 0:
                    for(int j = -1; j >= yDiv; j--)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv-j];
                        
                        if(piece == 0 && kingMoves[width] == y+yDiv-j && kingMoves[2*width] == x+xDiv-j && cover == 0)
                        {
                            kingMoves[0] = -1;
                        }
                        else if(piece > 16)
                        {
                            if(kingMoves[width] == y+yDiv-j && kingMoves[2*width] == x+xDiv-j)
                                kingMoves[0] = -1;
                            break;
                        }
                        else if(piece > 0 && piece < 16)
                        {
                            cover++;
                            if(cover > 1)
                                break;
                            tmpCover = (y+yDiv-j)*10 + x+xDiv-j;
                        }
                        else if(piece == 16)
                        {
                            if(cover == 0)
                            {
                                check = true;
                                checkCounter++;
                                piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                                piecesChecking[lastCheckingPieceIndex*2 + 1] = (pieces[i].dirRec.y/90)*10 + pieces[i].dirRec.x/90;
                                lastCheckingPieceIndex++;
                                if(stop == 0)
                                    kingMoves[4] = -1;
                            }
                            else
                            {
                                for(int j = 0; j < 16; j++)
                                    if(pieces[j].dirRec.y/90 == tmpCover/10 && pieces[j].dirRec.x/90 == tmpCover%10)
                                    {
                                        pinePiece(pieces[j].getType(), tmpCover, 4);
                                        break;
                                    }
                            }
                        }
                    }
                    break;
                case 1:
                    for(int j = -1; j >= xDiv + ystop; j--)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv-j];
                        
                        if(piece > 0)
                        {
                            for(int k = 0; k < 2; k++)
                            if(kingMoves[width + 7 - k*2] == y+yDiv-j && kingMoves[2*width + 7 - k*2] == x+xDiv-j)
                                {
                                    kingMoves[7 - k*2] = -1;
                                    break;
                                }
                            break;
                        }
                        else if(kingMoves[width + 7] == y+yDiv-j && kingMoves[2*width + 7] == x+xDiv-j)
                            kingMoves[7] = -1;
                        else if(kingMoves[width + 5] == y+yDiv-j && kingMoves[2*width + 5] == x+xDiv-j && ystop == 0)
                            kingMoves[5] = -1;
                    }
                    break;
                case 2:
                    if(ystop == 0)
                    {
                        for(int j = -1; j >= xDiv + 1; j--)
                        {
                            piece = map[(y+yDiv-j)*width + x+xDiv-j];
                            
                            if(piece > 0)
                            {
                                if(kingMoves[width + 6] == y+yDiv-j && kingMoves[2*width + 6] == x+xDiv-j)
                                    kingMoves[6] = -1;
                                break;
                            }
                            else if(kingMoves[width + 6] == y+yDiv-j && kingMoves[2*width + 6] == x+xDiv-j)
                                kingMoves[6] = -1;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        if(xDiv >= 0 && yDiv <= 0)
        {
            switch (relativePosition) {
                case -2:
                    if(start == 0)
                    {
                        for(int j = -1; j >= yDiv + 1; j--)
                        {
                            piece = map[(y+yDiv-j)*width + x+xDiv+j];
                            
                            if(piece > 0)
                            {
                                if(kingMoves[width] == y+yDiv-j && kingMoves[2*width] == x+xDiv+j)
                                    kingMoves[0] = -1;
                                break;
                            }
                            else if(kingMoves[width] == y+yDiv-j && kingMoves[2*width] == x+xDiv+j)
                                kingMoves[0] = -1;
                        }
                    }
                    break;
                case -1:
                    for(int j = -1; j >= yDiv + start; j--)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv+j];
                        
                        if(piece > 0)
                        {
                            for(int k = 0; k < 2; k++)
                            if(kingMoves[width + 1 + k*6] == y+yDiv-j && kingMoves[2*width + 1 + k*6] == x+xDiv+j)
                                {
                                    kingMoves[1 + k*6] = -1;
                                    break;
                                }
                            break;
                        }
                        else if(kingMoves[width + 1] == y+yDiv-j && kingMoves[2*width + 1] == x+xDiv+j)
                            kingMoves[1] = -1;
                        else if(kingMoves[width + 7] == y+yDiv-j && kingMoves[2*width + 7] == x+xDiv+j && start == 0)
                            kingMoves[7] = -1;
                    }
                    break;
                case 0:
                    for(int j = -1; j >= yDiv; j--)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv+j];
                        
                        if(piece == 0 && kingMoves[width + 2] == y+yDiv-j && kingMoves[2*width + 2] == x+xDiv+j && cover == 0)
                        {
                            kingMoves[2] = -1;
                        }
                        else if(piece > 16)
                        {
                            if(kingMoves[width + 2] == y+yDiv-j && kingMoves[2*width + 2] == x+xDiv+j)
                                kingMoves[2] = -1;
                            break;
                        }
                        else if(piece > 0 && piece < 16)
                        {
                            cover++;
                            if(cover > 1)
                                break;
                            tmpCover = (y+yDiv-j)*10 + x+xDiv+j;
                        }
                        else if(piece == 16)
                        {
                            if(cover == 0)
                            {
                                check = true;
                                checkCounter++;
                                piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                                piecesChecking[lastCheckingPieceIndex*2 + 1] = (pieces[i].dirRec.y/90)*10 + pieces[i].dirRec.x/90;
                                lastCheckingPieceIndex++;
                                if(start == 0)
                                    kingMoves[6] = -1;
                            }
                            else
                            {
                                for(int j = 0; j < 16; j++)
                                    if(pieces[j].dirRec.y/90 == tmpCover/10 && pieces[j].dirRec.x/90 == tmpCover%10)
                                    {
                                        pinePiece(pieces[j].getType(), tmpCover, 3);
                                        break;
                                    }
                            }
                        }
                    }
                    break;
                case 1:
                    for(int j = -1; j >= -xDiv + ystop; j--)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv+j];
                        
                        if(piece > 0)
                        {
                            for(int k = 0; k < 2; k++)
                            if(kingMoves[width + 3 + k*2] == y+yDiv-j && kingMoves[2*width + 3 + k*2] == x+xDiv+j)
                                {
                                    kingMoves[3 + k*2] = -1;
                                    break;
                                }
                            break;
                        }
                        else if(kingMoves[width + 3] == y+yDiv-j && kingMoves[2*width + 3] == x+xDiv+j)
                            kingMoves[3] = -1;
                        else if(kingMoves[width + 5] == y+yDiv-j && kingMoves[2*width + 5] == x+xDiv+j && ystop == 0)
                            kingMoves[5] = -1;
                    }
                    break;
                case 2:
                    if(ystop == 0)
                    {
                        for(int j = -1; j >= -xDiv + 1; j--)
                        {
                            piece = map[(y+yDiv-j)*width + x+xDiv+j];
                            
                            if(piece > 0)
                            {
                                if(kingMoves[width + 4] == y+yDiv-j && kingMoves[2*width + 4] == x+xDiv+j)
                                    kingMoves[4] = -1;
                                break;
                            }
                            else if(kingMoves[width + 4] == y+yDiv-j && kingMoves[2*width + 4] == x+xDiv+j)
                                kingMoves[4] = -1;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        if(xDiv <= 0 && yDiv >= 0)
        {
            switch (relativePosition) {
                case 2:
                    if(ystart == 0)
                    {
                        for(int j = 1; j <= yDiv + 1; j++)
                        {
                            piece = map[(y+yDiv-j)*width + x+xDiv+j];
                            
                            if(piece > 0)
                            {
                                if(kingMoves[width] == y+yDiv-j && kingMoves[2*width] == x+xDiv+j)
                                    kingMoves[0] = -1;
                                break;
                            }
                            else if(kingMoves[width] == y+yDiv-j && kingMoves[2*width] == x+xDiv+j)
                                kingMoves[0] = -1;
                        }
                    }
                    break;
                case 1:
                    for(int j = 1; j <= yDiv + 1 - ystart; j++)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv+j];
                        
                        if(piece > 0)
                        {
                            for(int k = 0; k < 2; k++)
                            if(kingMoves[width + 7 - k*6] == y+yDiv-j && kingMoves[2*width + 7 - k*6] == x+xDiv+j)
                                {
                                    kingMoves[7 - k*6] = -1;
                                    break;
                                }
                            break;
                        }
                        else if(kingMoves[width + 7] == y+yDiv-j && kingMoves[2*width + 7] == x+xDiv+j)
                            kingMoves[7] = -1;
                        else if(kingMoves[width + 1] == y+yDiv-j && kingMoves[2*width + 1] == x+xDiv+j && ystart == 0)
                            kingMoves[1] = -1;
                    }
                    break;
                case 0:
                    for(int j = 1; j <= yDiv + 1; j++)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv+j];

                        if(piece == 0 && kingMoves[width + 6] == y+yDiv-j && kingMoves[2*width + 6] == x+xDiv+j && cover == 0)
                        {
                            kingMoves[6] = -1;
                        }
                        else if(piece > 16)
                        {
                            if(kingMoves[width + 6] == y+yDiv-j && kingMoves[2*width + 6] == x+xDiv+j)
                                kingMoves[6] = -1;
                            break;
                        }
                        else if(piece > 0 && piece < 16)
                        {
                            cover++;
                            if(cover > 1)
                                break;
                            tmpCover = (y+yDiv-j)*10 + x+xDiv+j;
                        }
                        else if(piece == 16)
                        {
                            if(cover == 0)
                            {
                                check = true;
                                checkCounter++;
                                piecesChecking[lastCheckingPieceIndex*2] = pieces[i].getType();
                                piecesChecking[lastCheckingPieceIndex*2 + 1] = (pieces[i].dirRec.y/90)*10 + pieces[i].dirRec.x/90;
                                lastCheckingPieceIndex++;
                                if(stop == 0)
                                    kingMoves[2] = -1;
                            }
                            else
                            {
                                for(int j = 0; j < 16; j++)
                                    if(pieces[j].dirRec.y/90 == tmpCover/10 && pieces[j].dirRec.x/90 == tmpCover%10)
                                    {
                                        pinePiece(pieces[j].getType(), tmpCover, 3);
                                        break;
                                    }
                            }
                        }
                    }
                    break;
                case -1:
                    for(int j = 1; j <= -xDiv + 1 - start; j++)
                    {
                        piece = map[(y+yDiv-j)*width + x+xDiv+j];

                        if(piece > 0)
                        {
                            for(int k = 0; k < 2; k++)
                            if(kingMoves[width + 5 - k*2] == y+yDiv-j && kingMoves[2*width + 5 - k*2] == x+xDiv+j)
                                {
                                    kingMoves[5 - k*2] = -1;
                                    break;
                                }
                            break;
                        }
                        else if(kingMoves[width + 5] == y+yDiv-j && kingMoves[2*width + 5] == x+xDiv+j)
                            kingMoves[5] = -1;
                        else if(kingMoves[width + 3] == y+yDiv-j && kingMoves[2*width + 3] == x+xDiv+j && start == 0)
                            kingMoves[3] = -1;
                    }
                    break;
                case -2:
                    if(stop == 0)
                    {
                        for(int j = 1; j <= -xDiv + 1; j++)
                        {
                            piece = map[(y+yDiv-j)*width + x+xDiv+j];

                            if(piece > 0)
                            {
                                if(kingMoves[width + 4] == y+yDiv-j && kingMoves[2*width + 4] == x+xDiv+j)
                                    kingMoves[4] = -1;
                                break;
                            }
                            else if(kingMoves[width + 4] == y+yDiv-j && kingMoves[2*width + 4] == x+xDiv+j)
                                kingMoves[4] = -1;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::pinePiece(int type, int tmpCover, int pinningDir)
{
    pinnedPices[lastPinnedPieceIndex*6] = type;
    pinnedPices[lastPinnedPieceIndex*6 + 1] = tmpCover;
    
    switch (type) {
        case 1 ... 8:
            pinnedPices[lastPinnedPieceIndex*6 + 1 + pinningDir] = 1;
            break;
        case 9 ... 10:
            pinnedPices[lastPinnedPieceIndex*6 + 1 + pinningDir] = 1;
            break;
        case 11 ... 12:
            pinnedPices[lastPinnedPieceIndex*6 + 2] = 1;
            break;
        case 13 ... 14:
            pinnedPices[lastPinnedPieceIndex*6 + 1 + pinningDir] = 1;
            break;
        case 15:
            pinnedPices[lastPinnedPieceIndex*6 + 1 + pinningDir] = 1;
            break;
        case 17 ... 24:
            pinnedPices[lastPinnedPieceIndex*6 + 1 + pinningDir] = 1;
            break;
        case 25 ... 26:
            pinnedPices[lastPinnedPieceIndex*6 + 1 + pinningDir] = 1;
            break;
        case 27 ... 28:
            pinnedPices[lastPinnedPieceIndex*6 + 2] = 1;
            break;
        case 29 ... 30:
            pinnedPices[lastPinnedPieceIndex*6 + 1 + pinningDir] = 1;
            break;
        case 31:
            pinnedPices[lastPinnedPieceIndex*6 + 1 + pinningDir] = 1;
            break;
            
        default:
            break;
    }
    
    lastPinnedPieceIndex++;
}
