#include "Game.hpp"
#include "TextureMaker.hpp"

void Game::drawBoard()
{
    const int width = 8;
    square.h = 90;
    square.w = 90;
    
    SDL_SetRenderDrawColor(renderer, 176, 109, 37, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 219, 176, 129, 0);
    for(int i = 0; i < 8; i++)
    {
        square.y = i*90;
        square.x = (i%2)*90;
        SDL_RenderFillRect(renderer, &square);
        square.x = (i%2)*90 + 180;
        SDL_RenderFillRect(renderer, &square);
        square.x = (i%2)*90 + 360;
        SDL_RenderFillRect(renderer, &square);
        square.x = (i%2)*90 + 540;
        SDL_RenderFillRect(renderer, &square);
    }
    
    if(inBoundaries)
    {
        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++)
            {
                if(mapMark[i*width + j] == 0)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 234, 0, 0);
                    square.y = i*90;
                    square.x = j*90;
                    SDL_RenderFillRect(renderer, &square);
                }
                else if(mapMark[i*width + j] == 1)
                {
                    SDL_SetRenderDrawColor(renderer, 255, 60, 0, 0);
                    square.y = i*90;
                    square.x = j*90;
                    SDL_RenderFillRect(renderer, &square);
                }
                else if(mapMark[i*width + j] == 2)
                {
                    SDL_SetRenderDrawColor(renderer, 51, 102, 255, 0);
                    square.y = i*90;
                    square.x = j*90;
                    SDL_RenderFillRect(renderer, &square);
                }
            }
    }
}

void Game::drawPiecesToChoose(bool blackPromotion)
{
    square.h = 94;
    square.w = 364;
    square.x = 178;
    square.y = 313;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, &square);
    
    square.h = 90;
    square.w = 360;
    square.x = 180;
    square.y = 315;
    SDL_SetRenderDrawColor(renderer, 123, 135, 156, 0);
    SDL_RenderFillRect(renderer, &square);
    
    square.w = 90;
    
    if(chosenPiece != 0)
    {
        SDL_SetRenderDrawColor(renderer, 0, 234, 0, 0);
        switch (chosenPiece) {
            case 1:
                SDL_RenderFillRect(renderer, &square);
                break;
            case 2:
                square.x = 270;
                SDL_RenderFillRect(renderer, &square);
                break;
            case 3:
                square.x = 360;
                SDL_RenderFillRect(renderer, &square);
                break;
            case 4:
                square.x = 450;
                SDL_RenderFillRect(renderer, &square);
                break;
            case 5:
                SDL_RenderFillRect(renderer, &square);
                break;
            case 6:
                square.x = 270;
                SDL_RenderFillRect(renderer, &square);
                break;
            case 7:
                square.x = 360;
                SDL_RenderFillRect(renderer, &square);
                break;
            case 8:
                square.x = 450;
                SDL_RenderFillRect(renderer, &square);
                break;
                
            default:
                break;
        }
    }
    
    square.h = 80;
    square.w = 80;
    square.x = 95;
    square.y = 320;
    
    int blackPieces = 0;
    
    if(blackPromotion)
        blackPieces = 4;
        
    for(int i = blackPieces; i < 4 + blackPieces; i++)
    {
        square.x += 90;
        SDL_RenderCopy(Game::renderer, piecesToChoose[i], NULL, &square);
    }
}

void Game::setBoard()
{
    pieces.reserve(33);
    const int width = 8;
    
    for(int i = 1; i < 9; i++)
        pieces.emplace_back(i, i-1, 6);
    for(int i = 0; i < 2; i++)
        pieces.emplace_back(i+9, i*7, 7);
    for(int i = 0; i < 2; i++)
        pieces.emplace_back(i+11, 1+i*5, 7);
    for(int i = 0; i < 2; i++)
        pieces.emplace_back(i+13, 2+i*3, 7);
    pieces.emplace_back(15, 3, 7);
    pieces.emplace_back(16, 4, 7);
    for(int i = 1; i < 9; i++)
        pieces.emplace_back(i + 16, i-1, 1);
    for(int i = 0; i < 2; i++)
        pieces.emplace_back(i+25, i*7, 0);
    for(int i = 0; i < 2; i++)
        pieces.emplace_back(i+27, 1+i*5, 0);
    for(int i = 0; i < 2; i++)
        pieces.emplace_back(i+29, 2+i*3, 0);
    pieces.emplace_back(31, 3, 0);
    pieces.emplace_back(32, 4, 0);
    pieces.emplace_back(0, 4, -1); // ?
    
    map[0] = 25;
    map[7] = 26;
    map[1] = 27;
    map[6] = 28;
    map[2] = 29;
    map[5] = 30;
    map[3] = 31;
    map[4] = 32;
    
    map[56] = 9;
    map[63] = 10;
    map[57] = 11;
    map[62] = 12;
    map[58] = 13;
    map[61] = 14;
    map[59] = 15;
    map[60] = 16;
    
    piecesToChoose[0] = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/chess_pieces/white_rook.png");
    piecesToChoose[1] = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/chess_pieces/white_knight.png");
    piecesToChoose[2] = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/chess_pieces/white_bishop.png");
    piecesToChoose[3] = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/chess_pieces/white_queen.png");
    piecesToChoose[4] = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/chess_pieces/black_rook.png");
    piecesToChoose[5] = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/chess_pieces/black_knight.png");
    piecesToChoose[6] = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/chess_pieces/black_bishop.png");
    piecesToChoose[7] = TextureMaker::retTexture("/Users/bartek/xcode-workspace/Game/chess_pieces/black_queen.png");
    
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 8; j++)
            map[(1 + i*5)*width + j] = 17+j - i*16;
    }
    for(int i = 2; i < 6; i++)
    {
        for(int j = 0; j < 8; j++)
            map[i*width + j] = 0;
    }
    
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
            mapMark[i*width + j] = -1;
    }
    
    for(int i = 0; i < 4; i++)
        kingMoves[i] = -1;
    for(int i = 4; i < 7; i++)
        kingMoves[i] = -1;
    kingMoves[3] = -1;
    kingMoves[7] = -1;
    for(int i = 0; i < 3; i++)
    {
        kingMoves[width + i] = 6;
        kingMoves[2*width + i] = 3 + i;
    }
    kingMoves[width + 3] = 7;
    kingMoves[2*width + 3] = 5;
    kingMoves[width + 7] = 7;
    kingMoves[2*width + 7] = 3;
    
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            allowedMoves[i*width + j] = 1;
    
    for(int i = 0; i < 2; i++)
    {
        numberOfEachPieceType[i*5] = 8;
        numberOfEachPieceType[i*5 + 1] = 2;
        numberOfEachPieceType[i*5 + 2] = 2;
        numberOfEachPieceType[i*5 + 3] = 2;
        numberOfEachPieceType[i*5 + 4] = 1;
    }
    
    for(int i = 0; i < 30; i++)
        pinnedPices[i] = -1;
    
    for(int i = 0; i < 6; i++)
        kingsAndRooksMoved[i] = false;
    
}
