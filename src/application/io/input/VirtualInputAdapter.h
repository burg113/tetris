//
// Created by Burg on 16.12.2023.
//

#pragma once

#include "InputAdapter.h"

class [[maybe_unused]] [[deprecated]] VirtualInputAdapter : public InputAdapter {

public:
    [[maybe_unused]] explicit VirtualInputAdapter(int size);

    void update(int key,bool state) override;

};

