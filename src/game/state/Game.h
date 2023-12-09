//
// Created by Burg on 09.12.2023.
//

#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include "State.h"
#include "../Tetris.h"
#include "Board.h"
#include "../utils/Vec2.h"
#include "../io/KeyboardAdapter.h"


class Game : public State {
    // see https://tetris.wiki/Super_Rotation_System#How_Guideline_SRS_Really_Works
    std::vector<std::vector<std::vector<Vec2>>> rotationTable = {
            {{Vec2(0, 0), Vec2(0, 0),  Vec2(0, 0), Vec2(0, 0),  Vec2(0, 0)},
                    {Vec2(0, 0),  Vec2(1, 0), Vec2(1, 1), Vec2(0, -2), Vec2(1, -2)},
                    {Vec2(0, 0),   Vec2(0, 0),  Vec2(0, 0),   Vec2(0, 0), Vec2(0, 0)},
                    {Vec2(0, 0),  Vec2(-1, 0), Vec2(-1, 1), Vec2(0, -2), Vec2(-1, -2)}
            },
            {
             {Vec2(0, 0), Vec2(-1, 0), Vec2(2, 0), Vec2(-1, 0), Vec2(2, 0)},
                    {Vec2(-1, 0), Vec2(0, 0), Vec2(0, 0), Vec2(0, -1), Vec2(0, 2)},
                    {Vec2(-1, -1), Vec2(1, -1), Vec2(-2, -1), Vec2(1, 0), Vec2(-2, 0)},
                    {Vec2(0, -1), Vec2(0, -1), Vec2(0, -1), Vec2(0, 1),  Vec2(0, -2)}
            },
            {
             {Vec2(0, 0)},
                    {Vec2(0, 1)},
                    {Vec2(-1, 1)},
                    {Vec2(-1, 0)}
            }
    };


    struct Piece {
        SDL_Color col;
        std::vector<Vec2> tiles;
        int type;

        explicit Piece(std::vector<Vec2> tiles, int type, SDL_Color col);
    };


    Piece jPiece = Piece({Vec2(-1, -1), Vec2(-1, 0), Vec2(0, 0), Vec2(1, 0)}, 0, {20, 20, 200, 255});
    Piece lPiece = Piece({Vec2(-1, 0), Vec2(0, 0), Vec2(1, 0), Vec2(1, -1)}, 0, {240, 180, 20, 255});
    Piece sPiece = Piece({Vec2(-1, 0), Vec2(0, 0), Vec2(0, -1), Vec2(1, -1)}, 0, {15, 230, 15, 255});
    Piece tPiece = Piece({Vec2(-1, 0), Vec2(0, 0), Vec2(0, -1), Vec2(1, 0)}, 0, {140, 20, 180, 255});
    Piece zPiece = Piece({Vec2(-1, -1), Vec2(0, -1), Vec2(0, 0), Vec2(1, 0)}, 0, {200, 20, 20, 255});

    Piece iPiece = Piece({Vec2(-1, 0), Vec2(0, 0), Vec2(1, 0), Vec2(2, 0)}, 1, {20, 200, 240, 255});
    Piece oPiece = Piece({Vec2(0, 0), Vec2(1, 0), Vec2(0, -1), Vec2(1, -1)}, 2, {215, 220, 0, 255});

    Tetris *tetris;
    KeyboardAdapter *keyboard;
    Board board;

    bool smoothAnimation = true;

    int framesPerFall = 10;
    int lockLeniency = 8;
    int frameCount = 0;
    int framesToFall = 0;
    Vec2 position;
    int rotation = 0;
    Piece piece;

    int holdFrameCooldown = 5;
    int inpL = 0, inpR = 0;
    int holdFrameRotationCooldown = 15;
    int inpRl = 0, inpRr = 0;
public:
    explicit Game(Tetris *tetris);

    bool checkOffset(Vec2 posOffset);

    bool checkPos(Vec2 pos, Piece &piece, int rotation);

    bool checkPos(Vec2 pos);

    void update() override;

    void tryRotate(bool clockwise);

    Piece getNextTetromino();

    void render();

    void reset() override;

    void resetPiece();
};


#endif //TETRIS_GAME_H
