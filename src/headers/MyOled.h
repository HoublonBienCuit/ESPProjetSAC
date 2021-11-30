/**
    Class MyOled :  Pour la gestion du OLED
    @file MyOled.h 
    @author Daniel Boisclair
    @version 1.0 21/11/29
    
    Historique des versions   
                            Versions  Date      Auteur           Description
                            1.0      21/11/29   Daniel B         Première version de la classe
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        adafruit/Adafruit GFX Library @ ^1.10.1
        adafruit/Adafruit SSD1306 @ ^2.4.0
    Autres librairies (à copier dans le répertoire lib)
        OledManager v1.0
    
    Exemple d'utilisation 
        #include "MyOled.h"
        MyOled *myOled = nullptr;
        myOled = new MyOled();
        Screen* screen = myOled->getScreen();

        void loop() {
            myOled->update();
        }

**/

#pragma once

#include "Screen.h"

//Définition des pages
const int INIT_PAGE = 0;
const int WIFI_PAGE = 1;
const int READY_PAGE = 2;
const int OVEN_PAGE = 3;

class MyOled {
    public:
        MyOled();
        ~MyOled() {};

        void update();
        Screen* getScreen();

    private:
        void init();

        Screen* _screen;
};