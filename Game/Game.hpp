#ifndef Game_hpp
#define Game_hpp

#include "/Library/Frameworks/SDL2.framework/Headers/SDL.h"
#include "/Library/Frameworks/SDL2_image.framework/Headers/SDL_image.h"
#include <iostream>
#include <vector>
#include "Piece.hpp" // order and placing of imports is important

class Game{
public:
    
    // basic functionality
    void onInit(SDL_Window* window, SDL_Renderer* renderer);
    void setBoard();
    void drawScreen();
    void clean();
    void execute();
    bool getRun();
    
    // testing
    void countMoves(int depth);
    void blackTurn(int y, int x, int* pointer, int step);
    // white
    void testRenderWhitePawnMoves(int y, int x, int* pointer, int step);
    void testMarkWhitePawnMoves(int y, int x, int* pointer, int step);
    void testRenderWhiteHorizontalAndVertical(int y, int x, int* pointer, int step);
    void testMarkWhiteHorizontalAndVertical(int y, int x, int* pointer, int step);
    void testRenderWhiteKnightMoves(int y, int x, int* pointer, int step);
    void testMarkWhiteKnightMoves(int y, int x, int* pointer, int step);
    // black
    void testRenderBlackPawnMoves(int y, int x, int* pointer, int step);
    void testMarkBlackPawnMoves(int y, int x, int* pointer, int step);
    // drawing
    void drawBoard();
    void drawPiecesToChoose(bool blackPromotion);
    
    // white king
    void whiteKing(int y, int x, int kingsIndex);
    // setup
    void whiteKingStartingMoves(int y, int x);
    void distanceBetweenWhiteAndBlack(int y, int x, int blackKingY, int blackKingX);
    // attack detection
    void attackFromBlackPawns(int i, int y, int x);
    void attackFromKnight(int i, int y, int x);
    void attackFromBlackHorizontalAndVertical(int x, int y, int i);
    void attackFromBlackDiagonal(int x, int y, int i);
    // castling
    void whiteKingCastling();
    void whiteCastlingPrevention(int i, int side);
    // checkmate
    bool whiteKingCheckmate(int y, int x, int type, int kingY, int kingX,
                            int possibleMoves, int kingsIndex);
    void kingHorizontalAndVerticalCheckmate(int xDiv, int yDiv, int distance, int y, int x);
    void kingDiagonalCheckmate(int xDiv, int yDiv, int distance, int y, int x);
    int allowedMovesForWhiteCounter(int y, int x, int type);
    int countPossibleWhitePawns(int y, int x, int type);
    int countPossibleWhiteKnights(int y, int x, int type);
    int countPossibleWhiteHorizontalAndVertical(int y, int x, int type);
    int countPossibleWhiteDiagonal(int y, int x, int type);
    
    // black king
    void blackKing(int y, int x, int kingsIndex, int whiteKingID);
    // setup
    void blackKingStartingMoves(int y, int x);
    void distanceBetweenBlackAndWhite(int y, int x, int whiteKingY, int whiteKingX);
    // attack detection
    void attackFromWhitekPawns(int i, int y, int x);
    void attackFromWhiteHorizontalAndVertical(int x, int y, int i, int whiteKingID);
    //void attackFromKnight(int i, int y, int x);
    void attackFromWhiteDiagonal(int x, int y, int i, int whiteKingID);
    // castling
    void blackKingCastling();
    void blackCastlingPrevention(int i, int side);
    // checkmate
    bool blackKingCheckmate(int y, int x, int type, int kingY, int kingX,
                            int possibleMoves, int kingsIndex);
    int allowedMovesForBlackCounter(int y, int x, int type);
    int countPossibleBlackPawns(int y, int x, int type);
    int countPossibleBlackHorizontalAndVertical(int y, int x, int type);
    int countPossibleBlackKnights(int y, int x, int type);
    int countPossibleBlackDiagonal(int y, int x, int type);
    // draw
    void checkDraw();
    // dead position
    bool deadGame(int kingsIndex);
    
    // en passant
    void whiteEnPassant(int y, int x, int kingsIndex);
    void blackEnPassant(int y, int x, int kingsIndex);
    
    // supportive methods
    void decrementPiecesQuantity(int type);
    void updatePromotedPiece(int pieceType);
    void pinePiece(int type, int tmpCover, int pinningDir);
    
    // pieces moves
    void renderPossibleMoves(int y, int x);
    // white
    void renderWhitePawnMoves(int y, int x);
    void markWhitePawnMoves(int y, int x);
    void renderWhiteKnightMoves(int y, int x);
    void markWhiteKnightMoves(int y, int x);
    void renderWhiteHorizontalAndVertical(int y, int x);
    void markWhiteHorizontalAndVertical(int y, int x);
    void renderWhiteDiagonal(int y, int x);
    void markWhiteDiagonal(int y, int x);
    void renderKingMoves(int y, int x);
    // black
    void renderBlackPawnMoves(int y, int x);
    void markBlackPawnMoves(int y, int x);
    void renderBlackKnightMoves(int y, int x);
    void markBlackKnightMoves(int y, int x);
    void renderBlackHorizontalAndVertical(int y, int x);
    void markBlackHorizontalAndVertical(int y, int x);
    void renderBlackDiagonal(int y, int x);
    void markBlackDiagonal(int y, int x);
    
    static SDL_Renderer* renderer;
    
private:    
//    pinned pieces:
//    1 - piece type
//    2 - piece coordinates
//    3 - vertical pin
//    4 - horizontal pin
//    5 - up and right
//    6 - up and left
    std::vector<Piece> pieces;
    SDL_Window* window;
    SDL_Point mousePos;
    SDL_Rect square;
    
    SDL_Texture* piecesToChoose[8];
    int map[64];
    int mapMark[64];
    int kingMoves[24];
    int pinnedPices[30];
    int allowedMoves[64];
    int numberOfEachPieceType[10];
    bool kingsAndRooksMoved[6];
    bool castlingPossibility[4];
    int piecesChecking[4];
    
    int counter = 0;
    int whiteEnPassantPosition = -2;
    int blackEnPassantPosition = -2;
    int type = 0;
    int lastPinnedPieceIndex = 0;
    int chosenPiece = 0;
    int allowedMoveForWhitePawn = -1;
    int checkCounter = 0;
    int lastCheckingPieceIndex = 0;
    
    bool isRunning = true;
    bool pawnCheck = false;
    bool isClicked = false;
    bool inBoundaries = false;
    bool check = false;
    bool checkmate = false;
    bool stalemate = false;
    bool draw = false;
    bool gameOver = false;
};

#endif /* Game_hpp */

// TO DO

// check correctness of possible moves
// apply repetition of position
// apply 50+ moves rule
// add some endgame screen and play againg option
// add option to choose side
// fix dropping pieces
// implement board reversing
// implement timer
