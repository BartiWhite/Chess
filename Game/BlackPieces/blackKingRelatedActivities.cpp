#include "Game.hpp"

const int width = 8;

void Game::blackKing(int y, int x, int kingsIndex, int whiteKingID)
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
    
    // set black kings starting available moves
    blackKingStartingMoves(y, x);
    
    // setting distance between kings
    distanceBetweenBlackAndWhite(y, x, pieces[whiteKingID].dirRec.y/90, pieces[whiteKingID].dirRec.x/90);
    
    for(int i = 0; i < whiteKingID; i++)
    {
        switch (pieces[i].getType()) {
            case 1 ... 8:
                attackFromWhitekPawns(i, y, x);
                break;
            case 9 ... 10:
                attackFromWhiteHorizontalAndVertical(x, y, i, whiteKingID);
                break;
            case 11 ... 12:
                attackFromKnight(i, y, x);
                break;
            case 13 ... 14:
                attackFromWhiteDiagonal(x, y, i, whiteKingID);
                break;
            case 15:
                attackFromWhiteDiagonal(x, y, i, whiteKingID);
                attackFromWhiteHorizontalAndVertical(x, y, i, whiteKingID);
                break;
                
            default:
                break;
        }
    }
    
    // checking if castling is possible
    if(!check)
        blackKingCastling();
    else
    {
        castlingPossibility[2] = false;
        castlingPossibility[3] = false;
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
            if(blackKingCheckmate(piecesChecking[1]/10, piecesChecking[1]%10, piecesChecking[0], y, x, possibleMoves, kingsIndex))
                checkmate = true;
    }
    
    // checking stalemate
    if(possibleMoves == 0)
    {
        int numberOfMoves = 0;
        
        for(int i = whiteKingID; i < pieces.size()-2; i++)
            numberOfMoves += allowedMovesForBlackCounter(pieces[i].dirRec.y/90, pieces[i].dirRec.x/90, pieces[i].getType());
        
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
            
            for(int i = whiteKingID+1; i < whiteKingID+1 + numberOfEachPieceType[5]; i++)
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
                if(deadGame(whiteKingID))
                    draw = true;
            }
        }
    
    // game over
    if(checkmate || stalemate || draw)
        gameOver = true;
    if(checkmate)
        std::cout << "White won";

}

void Game::blackKingStartingMoves(int y, int x)
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
            else if(piece < 16)
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
            else if(piece < 16)
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
        else if(piece < 16)
            this->kingMoves[7] = 1;
    }
    if(stop != 1)
    {
        piece = map[y*width + x+1];
        
        if(piece== 0)
            this->kingMoves[3] = 0;
        else if(piece < 16)
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

void Game::distanceBetweenBlackAndWhite(int y, int x, int whiteKingY, int whiteKingX)
{
    if(y == whiteKingY)
    {
        if(x == whiteKingX - 2)
            for(int i = 2; i < 5; i++)
                kingMoves[i] = -1;
        else if(x == whiteKingX + 2)
        {
            kingMoves[6] = -1;
            kingMoves[7] = -1;
            kingMoves[0] = -1;
        }
    }
    else if(x == whiteKingX)
    {
        if(y == whiteKingY - 2)
            for(int i = 4; i < 7; i++)
                kingMoves[i] = -1;
        else if(y == whiteKingY + 2)
            for(int i = 0; i < 3; i++)
                kingMoves[i] = -1;
    }
    else if(std::abs(x - whiteKingX) + std::abs(y - whiteKingY) == 3)
    {
        if(y == whiteKingY + 2)
        {
            if(x == whiteKingX - 1)
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
        else if(y == whiteKingY - 2)
        {
            if(x == whiteKingX - 1)
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
        else if(x == whiteKingX - 2)
        {
            if(y == whiteKingY - 1)
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
        else if(x == whiteKingX + 2)
        {
            if(y == whiteKingY - 1)
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
    else if(std::abs(x - whiteKingX) + std::abs(y - whiteKingY) == 4)
    {
        int xDiv = x - whiteKingX;
        int yDiv = y - whiteKingY;
        
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

void Game::attackFromWhitekPawns(int i, int y, int x)
{
    int yDiv = pieces[i].dirRec.y/90 - y;
    int xDiv = pieces[i].dirRec.x/90 - x;
    if(yDiv >= 0 && yDiv < 3 && xDiv < 3 && xDiv > -3)
    {
        if(yDiv == 2)
        {
            switch (xDiv) {
                case -2:
                    kingMoves[6] = -1;
                    break;
                case -1:
                    kingMoves[5] = -1;
                    break;
                case 0:
                    kingMoves[6] = -1;
                    kingMoves[4] = -1;
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
        else if(yDiv == 1)
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
                    kingMoves[0] = -1;
                    break;
                case -1:
                    kingMoves[1] = -1;
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
    }
}

void Game::attackFromWhiteHorizontalAndVertical(int x, int y, int i, int whiteKingID)
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
                    if(0 < piece && piece < 16)
                    {
                        if(kingMoves[2*width + 3] == x+xDiv-j)
                            kingMoves[3] = -1;
                        break;
                    }
                    else if(16 < piece && piece < 32)
                    {
                        cover++;
                        if(cover > 1)
                            break;
                        tmpCover = y*10 + x+xDiv-j;
                    }
                    else if(piece == 32)
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
                            for(int j = whiteKingID + 1; j < 32; j++)
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
                    if(0 < piece && piece < 16)
                    {
                        if(kingMoves[2*width + 7] == j)
                            kingMoves[7] = -1;
                        break;
                    }
                    else if(16 < piece && piece < 32)
                    {
                        cover++;
                        if(cover > 1)
                            break;
                        tmpCover = y*10 + j;
                    }
                    else if(piece == 32)
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
                            for(int j = whiteKingID + 1; j < 32; j++)
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
                    if(0 < piece && piece < 16)
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
                    if(0 < piece && piece < 16)
                    {
                        if(kingMoves[width + 5] == y+yDiv-j)
                            kingMoves[5] = -1;
                        break;
                    }
                    else if(16 < piece && piece < 32)
                    {
                        cover++;
                        if(cover > 1)
                            break;
                        tmpCover = (y+yDiv-j)*10 + x;
                    }
                    else if(piece == 32)
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
                            for(int j = whiteKingID + 1; j < 32; j++)
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
                    if(0 < piece && piece < 16)
                    {
                        if(kingMoves[width + 1] == j)
                            kingMoves[1] = -1;
                        break;
                    }
                    else if(16 < piece && piece < 32)
                    {
                        cover++;
                        if(cover > 1)
                            break;
                        tmpCover = j*10 + x;
                    }
                    else if(piece == 32)
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
                            for(int j = whiteKingID + 1; j < 32; j++)
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

void Game::attackFromWhiteDiagonal(int x, int y, int i, int whiteKingID)
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
                        else if(0 < piece && piece < 17)
                        {
                            if(kingMoves[width + 4] == y+yDiv-j && kingMoves[2*width + 4] == x+xDiv-j)
                                kingMoves[4] = -1;
                            break;
                        }
                        else if(16 < piece && piece < 32)
                        {
                            cover++;
                            if(cover > 1)
                                break;
                            tmpCover = (y+yDiv-j)*10 + x+xDiv-j;
                        }
                        else if(piece == 32)
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
                                for(int j = whiteKingID + 1; j < 32; j++)
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
                        else if(0 < piece && piece < 17)
                        {
                            if(kingMoves[width] == y+yDiv-j && kingMoves[2*width] == x+xDiv-j)
                                kingMoves[0] = -1;
                            break;
                        }
                        else if(16 < piece && piece < 32)
                        {
                            cover++;
                            if(cover > 1)
                                break;
                            tmpCover = (y+yDiv-j)*10 + x+xDiv-j;
                        }
                        else if(piece == 32)
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
                                for(int j = whiteKingID + 1; j < 32; j++)
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
                        else if(0 < piece && piece < 17)
                        {
                            if(kingMoves[width + 2] == y+yDiv-j && kingMoves[2*width + 2] == x+xDiv+j)
                                kingMoves[2] = -1;
                            break;
                        }
                        else if(16 < piece && piece < 32)
                        {
                            cover++;
                            if(cover > 1)
                                break;
                            tmpCover = (y+yDiv-j)*10 + x+xDiv+j;
                        }
                        else if(piece == 32)
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
                                for(int j = whiteKingID + 1; j < 32; j++)
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
                        else if(0 < piece && piece < 17)
                        {
                            if(kingMoves[width + 6] == y+yDiv-j && kingMoves[2*width + 6] == x+xDiv+j)
                                kingMoves[6] = -1;
                            break;
                        }
                        else if(16 < piece && piece < 32)
                        {
                            cover++;
                            if(cover > 1)
                                break;
                            tmpCover = (y+yDiv-j)*10 + x+xDiv+j;
                        }
                        else if(piece == 32)
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
                                for(int j = whiteKingID + 1; j < 32; j++)
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

void Game::blackKingCastling()
{
    if(!kingsAndRooksMoved[3] && !kingsAndRooksMoved[4] && map[1] == 0 && map[2] == 0 && map[3] == 0)
    {
        if(map[17] != 11 && map[17] != 12 && map[18] != 11 && map[18] != 12
           && map[20] != 11 && map[20] != 12 && map[8] != 11 && map[8] != 12
           && map[9] != 11 && map[9] != 12 && map[12] != 11 && map[12] != 12
           && map[13] != 11 && map[13] != 12)
        {
            for(int i = 2; i < 4; i++)
                blackCastlingPrevention(i, 2);
        }
        else castlingPossibility[2] = false;
    }
    else castlingPossibility[2] = false;
    
    if(!kingsAndRooksMoved[3] && !kingsAndRooksMoved[5] && map[5] == 0 && map[6] == 0)
    {
        if(map[20] != 11 && map[20] != 12 && map[22] != 11 && map[22] != 12
           && map[23] != 11 && map[23] != 12 && map[11] != 11 && map[11] != 12
           && map[12] != 11 && map[12] != 12 && map[15] != 11 && map[15] != 12)
        {
            for(int i = 5; i < 7; i++)
                blackCastlingPrevention(i, 3);
        }
        else castlingPossibility[3] = false;
    }
    else castlingPossibility[3] = false;
}

void Game::blackCastlingPrevention(int i, int side)
{
    const int width = 8;
    int y = i/8;
    int x = i%8;
    int lenght = 8 - x < 8 - y ? 8 - x : 8 - y;
    
    // down and right
    for(int j = 1; j < lenght; j++)
    {
        int piece = map[(y+j)*width + x+j];
        
        if((15 < piece && piece < 32) || (8 < piece && piece < 13))
            break;
        else if((0 < piece && piece < 9) || (12 < piece && piece < 16))
        {
            if(0 < piece && piece < 9)
            {
                if(j == 1)
                    castlingPossibility[side] = false;
                break;
            }
            castlingPossibility[side] = false;
            break;
        }
    }
    // down
    for(int j = 1; j <= 7; j++)
    {
        int piece = map[j*width + x];
        
        if((15 < piece && piece < 32) || (0 < piece && piece < 9) || (10 < piece && piece < 15))
            break;
        else if((8 < piece && piece < 11) || piece == 15)
        {
            castlingPossibility[side] = false;
            break;
        }
    }
    lenght = x < 7 - y ? x : 7 - y;
    // down and left
    for(int j = 1; j <= lenght; j++)
    {
        int piece = map[(y+j)*width + x-j];
        
        if((15 < piece && piece < 32) || (8 < piece && piece < 13))
            break;
        else if((0 < piece && piece < 9) || (12 < piece && piece < 16))
        {
            if(0 < piece && piece < 9)
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
