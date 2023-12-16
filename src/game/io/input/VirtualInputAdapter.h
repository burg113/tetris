//
// Created by Burg on 16.12.2023.
//

#ifndef TETRIS_VIRTUALKEYBOARDADAPTER_H
#define TETRIS_VIRTUALKEYBOARDADAPTER_H

#include "InputAdapter.h"

class VirtualInputAdapter : public InputAdapter {

public:
    explicit VirtualInputAdapter(int size);

    void update(int key,bool state) override;

};


#endif //TETRIS_VIRTUALKEYBOARDADAPTER_H
