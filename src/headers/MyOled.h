#pragma once

#include "Screen.h"

const int INIT_PAGE = 0;
const int WIFI_PAGE = 1;
const int READY_PAGE = 2;
const int OVEN_PAGE = 3;

class MyOled {
    public:
        MyOled();
        ~MyOled() {};

        void update(float dt);
        Screen* getScreen();
        
    private:
        void init();

        Screen* _screen;
};