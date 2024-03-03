//
// Created by Burg on 09.12.2023.
//

#ifndef TETRIS_GAMELOGIC_H
#define TETRIS_GAMELOGIC_H

#include "application/io/input/InputAdapter.h"
#include "application/client/state/game/Board.h"
#include "utils/Vec2.h"


class GameLogic {
    friend class Game;
    friend class MirrorServer;

private:
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
        int colorId;
        std::vector<Vec2> tiles;
        int type;

        explicit Piece(std::vector<Vec2> tiles, int type, int colorId);
    };
    Piece jPiece = Piece({Vec2(-1, -1), Vec2(-1, 0), Vec2(0, 0), Vec2(1, 0)}, 0, 0);
    Piece lPiece = Piece({Vec2(-1, 0), Vec2(0, 0), Vec2(1, 0), Vec2(1, -1)}, 0, 1);
    Piece sPiece = Piece({Vec2(-1, 0), Vec2(0, 0), Vec2(0, -1), Vec2(1, -1)}, 0, 2);
    Piece tPiece = Piece({Vec2(-1, 0), Vec2(0, 0), Vec2(0, -1), Vec2(1, 0)}, 0, 3);

    Piece zPiece = Piece({Vec2(-1, -1), Vec2(0, -1), Vec2(0, 0), Vec2(1, 0)}, 0, 4);
    Piece iPiece = Piece({Vec2(-1, 0), Vec2(0, 0), Vec2(1, 0), Vec2(2, 0)}, 1, 5);

    Piece oPiece = Piece({Vec2(0, 0), Vec2(1, 0), Vec2(0, -1), Vec2(1, -1)}, 2, 6);

    InputAdapter *inputAdapter = nullptr;

    int holdFrameCooldown = 5;
    int inpL = -1, inpR = -1;
    int holdFrameRotationCooldown = 15;
    int inpRl = -1, inpRr = -1;

    bool instaDrop = false;
    int lockLeniency = 8;
    int fastFallExtraSpeed = 5;

    void reset();

    bool checkPos(Vec2 pos, Piece &piece, int rotation);

    bool checkPos(Vec2 pos);
    void handleInput();

    void handleInputCallback(bool b, int input);

    void lockPiece();

    bool tryRotate(bool clockwise);

    Piece getNextTetromino();
    void resetPiece();
    bool checkOffset(Vec2 posOffset);

    int framesPerFall = 10;
    int frameCount = 0;
    int framesToFall = 0;
    Vec2 position;
    int rotation = 0;
    Piece piece;

    Board board;

public:

    enum Key {
        LEFT,
        RIGHT,
        ROTATE_LEFT,
        ROTATE_RIGHT,
        INSTA_DROP,
        FAST_FALL,
        SIZE
    };

    GameLogic();
    void setInputAdapter(InputAdapter* inputAdapter);

    void update();
};


#endif //TETRIS_GAMELOGIC_H
