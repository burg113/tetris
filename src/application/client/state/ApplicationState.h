//
// Created by Burg on 09.12.2023.
//

#pragma once


class ApplicationState {
public:
    virtual void update() = 0;

    virtual ~ApplicationState() = default;

};

