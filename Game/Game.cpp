#include "Game.hpp"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event event;
SDL_Rect* selected = NULL;
SDL_Point clickOffset;
int lastPosition = 0;
int movedPieceId = -1;
static const int width = 8;
int turn = 0;
bool whitePromotion = false;
bool blackPromotion = false;

Piece dopler(15, 3, 7);

void Game::onInit(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_Init(IMG_INIT_PNG);
    
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        this->window = window;
        this->renderer = renderer;
        SDL_SetRenderDrawColor(this->renderer, 242, 242, 242, 255);
        SDL_RenderClear(this->renderer);
        isRunning = true;
    }
    else isRunning = false;
    
    square.h = 90; square.w = 90;
    castlingPossibility[0] = false;
    castlingPossibility[1] = false;
    setBoard();
}

void Game::drawScreen()
{
    SDL_RenderClear(renderer);
    
    drawBoard();
    for(int i = 0; i < pieces.size(); i++)
        pieces[i].Render();
    dopler.Render();
    if(whitePromotion || blackPromotion)
        drawPiecesToChoose(blackPromotion);

    SDL_RenderPresent(renderer);
}

void Game::execute()
{
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
            
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE)
                isRunning = false;
            break;
            
        case SDL_MOUSEMOTION:
            {
                mousePos = { event.motion.x, event.motion.y };
                 
                if (isClicked && selected != NULL && (!whitePromotion && !blackPromotion))
                {
                    selected->x = mousePos.x - clickOffset.x;
                    selected->y = mousePos.y - clickOffset.y;
                    dopler.Update(mousePos.x - clickOffset.x, mousePos.y - clickOffset.y);
                }
                else if(whitePromotion || blackPromotion)
                {
                    if(315 < mousePos.y && mousePos.y < 405)
                    {
                        switch (mousePos.x) {
                            case 181 ... 270:
                                if(whitePromotion)
                                    chosenPiece = 1;
                                else chosenPiece = 5;
                                break;
                            case 271 ... 360:
                                if(whitePromotion)
                                    chosenPiece = 2;
                                else chosenPiece = 6;
                                break;
                            case 361 ... 450:
                                if(whitePromotion)
                                    chosenPiece = 3;
                                else chosenPiece = 7;
                                break;
                            case 451 ... 540:
                                if(whitePromotion)
                                    chosenPiece = 4;
                                else chosenPiece = 8;
                                break;
                                
                            default:
                                chosenPiece = 0;
                                break;
                        }
                    }
                    else chosenPiece = 0;
                }
            }
            break;
            
        case SDL_MOUSEBUTTONUP:
            if (isClicked && event.button.button == SDL_BUTTON_LEFT && (!whitePromotion && !blackPromotion))
            {
                int x = mousePos.x/90; int y = mousePos.y/90;
                
                if(inBoundaries && (mapMark[(y)*width + x] == 0 || mapMark[(y)*width + x] == 1))
                {
                    for(int i = 0; i < 8; i++)
                        for(int j = 0; j < 8; j++)
                            allowedMoves[i*width + j] = 1;
                    
                    // erasing pieces
                    for(int i = 0; i < pieces.size()-1; i++)
                        if(pieces[i].dirRec.x/90 == x && pieces[i].dirRec.y/90 == y && map[y*width + x] != type && map[y*width + x] > 0 && i != movedPieceId)
                        {
                            decrementPiecesQuantity(pieces[i].getType());
                            pieces.erase(pieces.begin() + i);
                            if(i < movedPieceId)
                                movedPieceId--;
                            break;
                        }
                    
                    // looking for white king
                    int kingsIndex = 0;
                    
                    for(int i = 0; i < 16; i++)
                        if(pieces[i].getType() == 16)
                        {
                            kingsIndex = i;
                            break;
                        }
                    
                    // erasing pawn captured in en passant
                    if(0 < type && type < 9 && y == 2 && x == blackEnPassantPosition)
                    {
                        for(int i = kingsIndex + 1; i < kingsIndex + 8; i++)
                            if(pieces[i].dirRec.y/90 == y+1 && pieces[i].dirRec.x/90 == blackEnPassantPosition)
                            {
                                decrementPiecesQuantity(pieces[i].getType());
                                pieces.erase(pieces.begin() + i);
                                if(i < movedPieceId)
                                    movedPieceId--;
                                map[y*width + x] = type;
                                map[(y + 1)*width + x] = 0;
                                break;
                            }
                    }
                    else if(16 < type && type < 25 && y == 5 && x == whiteEnPassantPosition)
                    {
                        for(int i = 0; i < 8; i++)
                            if(pieces[i].dirRec.y/90 == y-1 && pieces[i].dirRec.x/90 == whiteEnPassantPosition)
                            {
                                decrementPiecesQuantity(pieces[i].getType());
                                pieces.erase(pieces.begin() + i);
                                if(i < movedPieceId)
                                    movedPieceId--;
                                map[y*width + x] = type;
                                map[(y - 1)*width + x] = 0;
                                break;
                            }
                    }
                    // promoting white pawn
                    else if(0 < type && type < 9 && y == 0)
                    {
                        whitePromotion = true;
                        map[(lastPosition/10)*width + lastPosition%10] = 0;
                        lastPosition = y*10 + x;
                    }
                    // promoting black pawn
                    else if(16 < type && type < 25 && y == 7)
                    {
                        blackPromotion = true;
                        map[(lastPosition/10)*width + lastPosition%10] = 0;
                        lastPosition = y*10 + x;
                    }
                    else map[y*width + x] = type;
                    
                    // if move to unallowed position
                    pieces[movedPieceId].Update(x*90 + 5, y*90 + 5);
                    dopler.Update(x*90 + 5, y*90 + 5);
                    map[(lastPosition/10)*width + lastPosition%10] = 0;
                    
                    // castlings
                    if(type == 16 && y == 7 && x == 2 && castlingPossibility[0])
                    {
                        for(int i = 0; i < 16; i++)
                            if(pieces[i].getType() == 9)
                                pieces[i].Update(3*90 + 5, 7*90 + 5);
                        map[59] = 10;
                        map[56] = 0;
                        kingsAndRooksMoved[0] = true;
                    }
                    else if(type == 16 && y == 7 && x == 6 && castlingPossibility[1])
                    {
                        for(int i = 0; i < 16; i++)
                            if(pieces[i].getType() == 10)
                                pieces[i].Update(5*90 + 5, 7*90 + 5);
                        map[61] = 9;
                        map[63] = 0;
                        kingsAndRooksMoved[0] = true;
                    }
                    else if(type == 32 && y == 0 && x == 2 && castlingPossibility[2])
                    {
                        for(int i = 0; i < 16; i++)
                            if(pieces[i].getType() == 25)
                                pieces[i].Update(3*90 + 5, 7*90 + 5);
                        map[3] = 25;
                        map[0] = 0;
                        kingsAndRooksMoved[3] = true;
                    }
                    else if(type == 32 && y == 0 && x == 6 && castlingPossibility[3])
                    {
                        for(int i = 0; i < 16; i++)
                            if(pieces[i].getType() == 26)
                                pieces[i].Update(5*90 + 5, 7*90 + 5);
                        map[5] = 26;
                        map[7] = 0;
                        kingsAndRooksMoved[3] = true;
                    }
                    //******************************************************************
                    
                    // en passant
                    if(whiteEnPassantPosition > -2)
                        whiteEnPassantPosition = -2;
                    else if(blackEnPassantPosition > -2)
                        blackEnPassantPosition = -2;

                    if(0 < type && type < 9 && lastPosition/10 - y == 2)
                    {
                        blackEnPassant(y, x, (int)pieces.size() - 2);
                    }
                    else if(16 < type  && type < 25 && y - lastPosition/10 == 2)
                    {
                        whiteEnPassant(y, x, kingsIndex);
                    }
                    
                    turn++;
                    
                    // king's safety
                    if(turn%2 == 0)
                        whiteKing(pieces[kingsIndex].dirRec.y/90, pieces[kingsIndex].dirRec.x/90, kingsIndex);
                    else
                        blackKing((int)pieces[pieces.size() - 2].dirRec.y/90, (int)pieces[pieces.size() - 2].dirRec.x/90,
                                  (int)pieces.size() - 2, kingsIndex);
                    
                }
                else
                {
                    pieces[movedPieceId].Update((lastPosition%10)*90 + 5, (lastPosition/10)*90 + 5);
                    dopler.Update((lastPosition%10)*90 + 5, (lastPosition/10)*90 + 5);
                }
                
                for(int i = 0; i < 8; i++)
                    for(int j = 0; j < 8; j++)
                        mapMark[i*width + j] = -1;
                
                inBoundaries = false;
                isClicked = false;
                selected = NULL;
                        
            }
            else if((whitePromotion || blackPromotion) && chosenPiece != 0)
            {
                switch (chosenPiece) {
                    case 1:
                        updatePromotedPiece(9);
                        break;
                    case 2:
                        updatePromotedPiece(11);
                        break;
                    case 3:
                        updatePromotedPiece(13);
                        break;
                    case 4:
                        updatePromotedPiece(15);
                        break;
                    case 5:
                        updatePromotedPiece(25);
                        break;
                    case 6:
                        updatePromotedPiece(27);
                        break;
                    case 7:
                        updatePromotedPiece(29);
                        break;
                    case 8:
                        updatePromotedPiece(31);
                        break;
                        
                    default:
                        break;
                }
                
                blackPromotion = false;
                whitePromotion = false;
            }
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            if (!isClicked && event.button.button == SDL_BUTTON_LEFT && (!whitePromotion && !blackPromotion))
            {
                int x = mousePos.x/90; int y = mousePos.y/90;
                
                bool isPinned = false;
                for(int i = 0; i < 5; i++)
                    if(map[y*width + x] == pinnedPices[i*6])
                    {
                        isPinned = true;
                        break;
                    }
                
                if(isPinned)
                    mapMark[y*width + x] = 2;

                if(map[y*width + x] != 0)
                {
                    if(turn%2 == 0 && 0 < map[y*width + x] && map[y*width + x] < 17)
                    {
                        isClicked = true;
                        
                        for(int i = 0; i < pieces.size()-1; i++)
                            if(pieces[i].dirRec.x/90 == x && pieces[i].dirRec.y/90 == y)
                            {
                                if(map[y*width + x] != type)
                                {
                                    dopler.updateTexture(map[y*width + x]);
                                    dopler.Update(x*90 + 5, y*90 + 5);
                                }
                                type = map[y*width + x];
                                renderPossibleMoves(y, x);
                                lastPosition = y*10 + x;
                                
                                inBoundaries = true;
                                selected = &pieces[i].dirRec;
                                clickOffset.x = mousePos.x - pieces[i].dirRec.x;
                                clickOffset.y = mousePos.y - pieces[i].dirRec.y;
                                
                                movedPieceId = i;
                                
                                break;
                            }
                    }
                    else if(turn%2 == 1 && 16 < map[y*width + x] && map[y*width + x] < 33)
                    {
                        isClicked = true;
                        
                        for(int i = 0; i < pieces.size()-1; i++)
                            if(pieces[i].dirRec.x/90 == x && pieces[i].dirRec.y/90 == y)
                            {
                                if(map[y*width + x] != type)
                                {
                                    dopler.updateTexture(map[y*width + x]);
                                    dopler.Update(x*90 + 5, y*90 + 5);
                                }
                                type = map[y*width + x];
                                renderPossibleMoves(y, x);
                                lastPosition = y*10 + x;
                                
                                inBoundaries = true;
                                selected = &pieces[i].dirRec;
                                clickOffset.x = mousePos.x - pieces[i].dirRec.x;
                                clickOffset.y = mousePos.y - pieces[i].dirRec.y;
                                
                                movedPieceId = i;
                                
                                break;
                            }
                    }
                }
            }
            break;
            
        default:
            break;
    }
}

void Game::decrementPiecesQuantity(int type)
{
    switch (type) {
        case 1 ... 8:
            numberOfEachPieceType[0]--;
            break;
        case 9 ... 10:
            numberOfEachPieceType[1]--;
            break;
        case 11 ... 12:
            numberOfEachPieceType[2]--;
            break;
        case 13 ... 14:
            numberOfEachPieceType[3]--;
            break;
        case 15:
            numberOfEachPieceType[4]--;
            break;
        case 17 ... 24:
            numberOfEachPieceType[5]--;
            break;
        case 25 ... 26:
            numberOfEachPieceType[6]--;
            break;
        case 27 ... 28:
            numberOfEachPieceType[7]--;
            break;
        case 29 ... 30:
            numberOfEachPieceType[8]--;
            break;
        case 31:
            numberOfEachPieceType[9]--;
            break;
        default:
            break;
    }
}

void Game::updatePromotedPiece(int pieceType)
{
    pieces[movedPieceId].updateTexture(pieceType);
    dopler.updateTexture(pieceType);
    map[(lastPosition/10)*width + lastPosition%10] = pieceType;
    if(type < 16)
    {
        numberOfEachPieceType[0]--;
        numberOfEachPieceType[chosenPiece]++;
    }
    else
    {
        numberOfEachPieceType[5]--;
        numberOfEachPieceType[1 + chosenPiece]++;
    }
}

void Game::whiteEnPassant(int y, int x, int kingsIndex)
{
    int kingsY = pieces[kingsIndex].dirRec.y/90;
    int kingsX = pieces[kingsIndex].dirRec.x/90;
    int yDiv = y - kingsY;
    int xDiv = x - kingsX;

    if(xDiv != 0)
    {
        // to the right
        if(yDiv == 0 && xDiv > 0)
        {
            int cover = 0;
            for(int i = 1; i < 8 - x; i++)
            {
                int piece = map[y*width + x + i];
                if(0 < piece && piece < 16)
                {
                    if(piece > 8)
                    {
                        blackEnPassantPosition = x;
                        break;
                    }
                    else if(cover == 0)
                        cover++;
                    else
                    {
                        blackEnPassantPosition = x;
                        break;
                    }
                }
                else if(piece > 16)
                {
                    if((24 < piece && piece < 27) || piece == 31)
                    {
                        for(int j = 1; j <= x; j++)
                        {
                            piece = map[y*width + x - j];
                            if(piece == 16)
                                break;
                            else if(piece > 0)
                            {
                                if(piece < 9 && j != 1)
                                {
                                    blackEnPassantPosition = x;
                                    break;
                                }
                                else if(piece > 8)
                                {
                                    blackEnPassantPosition = x;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    else
                    {
                        blackEnPassantPosition = x;
                        break;
                    }
                }
                else if(x + i == 7)
                    blackEnPassantPosition = x;
            }
        }
        // to the left
        else if(yDiv == 0 && xDiv < 0)
        {
            int cover = 0;
            for(int i = 1; i <= x; i++)
            {
                int piece = map[y*width + x - i];
                if(0 < piece && piece < 16)
                {
                    if(piece > 8)
                    {
                        blackEnPassantPosition = x;
                        break;
                    }
                    else if(cover == 0)
                        cover++;
                    else
                    {
                        blackEnPassantPosition = x;
                        break;
                    }
                }
                else if(piece > 16)
                {
                    if((24 < piece && piece < 27) || piece == 31)
                    {
                        for(int j = 1; j < 8 - x; j++)
                        {
                            piece = map[y*width + x + j];
                            if(piece == 16)
                                break;
                            else if(piece > 0)
                            {
                                if(piece < 9 && j != 1)
                                {
                                    blackEnPassantPosition = x;
                                    break;
                                }
                                else if(piece > 8)
                                {
                                    blackEnPassantPosition = x;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    else
                    {
                        blackEnPassantPosition = x;
                        break;
                    }
                }
                else if(x - i == 0)
                    blackEnPassantPosition = x;
            }
        }
        else blackEnPassantPosition = x;
    }
    else blackEnPassantPosition = x;
}

void Game::blackEnPassant(int y, int x, int kingsIndex)
{
    int kingsY = pieces[kingsIndex].dirRec.y/90;
    int kingsX = pieces[kingsIndex].dirRec.x/90;
    int yDiv = y - kingsY;
    int xDiv = x - kingsX;
    
    if(xDiv != 0)
    {
        // to the right
        if(yDiv == 0 && xDiv > 0)
        {
            for(int i = 1; i < 8 - x; i++)
            {
                int cover = 0;
                for(int i = 1; i < 8 - x; i++)
                {
                    int piece = map[y*width + x + i];
                    if(16 < piece && piece < 32)
                    {
                        if(piece > 24)
                        {
                            whiteEnPassantPosition = x;
                            break;
                        }
                        else if(cover == 0)
                            cover++;
                        else
                        {
                            whiteEnPassantPosition = x;
                            break;
                        }
                    }
                    else if(0 < piece && piece < 16)
                    {
                        if((8 < piece && piece < 11) || piece == 15)
                        {
                            for(int j = 1; j <= x; j++)
                            {
                                piece = map[y*width + x - j];
                                if(piece == 32)
                                    break;
                                else if(piece > 0)
                                {
                                    if(16 < piece && piece < 25 && j != 1)
                                    {
                                        whiteEnPassantPosition = x;
                                        break;
                                    }
                                    else if(!(16 < piece && piece < 25))
                                    {
                                        whiteEnPassantPosition = x;
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        else
                        {
                            whiteEnPassantPosition = x;
                            break;
                        }
                    }
                    else if(x + i == 7)
                        whiteEnPassantPosition = x;
                }
            }
        }
        // to the left
        else if(yDiv == 0 && xDiv < 0)
        {
            int cover = 0;
            for(int i = 1; i <= x; i++)
            {
                int piece = map[y*width + x - i];
                if(16 < piece && piece < 32)
                {
                    if(piece > 24)
                    {
                        whiteEnPassantPosition = x;
                        break;
                    }
                    else if(cover == 0)
                        cover++;
                    else
                    {
                        whiteEnPassantPosition = x;
                        break;
                    }
                }
                else if(0 < piece && piece < 16)
                {
                    if((8 < piece && piece < 11) || piece == 15)
                    {
                        for(int j = 1; j < 8 - x; j++)
                        {
                            piece = map[y*width + x + j];
                            if(piece == 32)
                                break;
                            else if(piece > 0)
                            {
                                if(16 < piece && piece < 25 && j != 1)
                                {
                                    whiteEnPassantPosition = x;
                                    break;
                                }
                                else if(!(16 < piece && piece < 25))
                                {
                                    whiteEnPassantPosition = x;
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    else
                    {
                        whiteEnPassantPosition = x;
                        break;
                    }
                }
                else if(x - i == 0)
                    whiteEnPassantPosition = x;
            }
        }
        else whiteEnPassantPosition = x;
    }
    
    else whiteEnPassantPosition = x;
}

void Game::clean()
{
    dopler.Update(5, 545);
    dopler.updateTexture(1);
    selected = NULL;
    if(selected != NULL)
        delete selected;
    lastPosition = 0;
    movedPieceId = -1;
    turn = 0;
    whitePromotion = false;
    blackPromotion = false;
    for(int i = 0; i++; i < 8)
        SDL_DestroyTexture(piecesToChoose[i]);
    pieces.clear();
}

bool Game::getRun() {return isRunning;}
