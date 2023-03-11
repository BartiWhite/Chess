#include "Piece.hpp"
#include "TextureMaker.hpp"

Piece::Piece(int type, int x, int y)
{
    texture = TextureMaker::retTexture(retPath(type));
    dirRec.x = x*90 + 5;
    dirRec.y = y*90 + 5;
    this->type = type;
}

void Piece::updateTexture(int type)
{
    texture = TextureMaker::retTexture(retPath(type));
}

Piece::~Piece()
{
    SDL_DestroyTexture(texture);
}

void Piece::Render()
{
    SDL_RenderCopy(Game::renderer, texture, NULL, &dirRec);
}

void Piece::Update(int xPos, int yPos)
{
    dirRec.x = xPos;
    dirRec.y = yPos;
}

const char* Piece::retPath(int type)
{
    switch (type) {
        case 1 ... 8:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/white_pawn.png";
        case 9 ... 10:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/white_rook.png";
        case 11 ... 12:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/white_knight.png";
        case 13 ... 14:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/white_bishop.png";
        case 15:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/white_queen.png";
        case 16:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/white_king.png";
        case 17 ... 24:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/black_pawn.png";
        case 25 ... 26:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/black_rook.png";
        case 27 ... 28:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/black_knight.png";
        case 29 ... 30:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/black_bishop.png";
        case 31:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/black_queen.png";
        case 32:
            return "/Users/bartek/xcode-workspace/Game/chess_pieces/black_king.png";
        default:
            return "";
    }
}

int Piece::getType() {return type;}
