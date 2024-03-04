//
// Created by Burg on 16.12.2023.
//

#pragma once

#include "InputAdapter.h"

class [[deprecated]] VirtualInputAdapter : public InputAdapter {

public:
    explicit VirtualInputAdapter(int size);

    void update(int key,bool state) override;

};

