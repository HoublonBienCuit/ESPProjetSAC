/**
    Class OvenSystem :  Pour la gestion du four complète
    @file OvenSystem.h 
    @author Daniel Boisclair
    @version 1.0 21/11/29
    
    Historique des versions   
                            Versions  Date      Auteur           Description
                            1.0      21/11/29   Daniel B         Première version de la classe
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        ESPAsyncWebServer-esphome
        AsyncTCP-esphome
    Autres librairies (à copier dans le répertoire lib)
        Aucune
    
    Exemple d'utilisation 
        #include "OvenSystem.h"
        ovenSystem *ovenSystem = nullptr;
        ovenSystem = new OvenSystem();

        Serial.println(ovenSystem->getOvenTemp());

        ovenSystem->startOven(10.5, 26);
        ovenSystem->stopOven();

        Serial.println(ovenSystem->getOvenTime());

        Serial.println(ovenSystem->isOvenStartedFunc());
        Serial.println(ovenSystem->isAuth(AsyncWebServerRequest* request))

        float lastUpdate = 0, now = 0;

        const int FPS = 30;
        const int frameDelay = 1000 / FPS;

        void loop() {
            now = millis();

            int dt = now - lastUpdate;
            
            if (dt < frameDelay) {
                delay(frameDelay - dt);

                now = millis();
            }
            lastUpdate = now;

            ovenSystem->update((dt + (frameDelay - dt)) / 1000.0f);
        }

**/

#pragma once

#include <HTTPClient.h>
#include <WiFiManager.h>
#include <Arduino.h>
#include <vector>

#include "../headers/MyOled.h"
#include "../headers/TemperatureStub.h"
#include "../headers/MyServer.h"
#include "../headers/MyButton.h"


#define DHTPIN  15   // Pin utilisée par le senseur DHT11 / DHT22
#define DHTTYPE DHT22  //Le type de senseur utilisé (mais ce serait mieux d'avoir des DHT22 pour plus de précision)

//Port du serveur
#define DEFAULT_PORT 80

//Pin pour action et reset
#define ACTION_PIN 33
#define RESET_PIN 32

//Pin des leds
#define LED_JAUNE 27
#define LED_VERT 14
#define LED_ROUGE 12

//Variables à déclarer seulement dans OvenSystem.cpp
#ifdef OVENSYSTEM_CPP

const char *SSID = "SAC_Daniel";
const char *PASSWORD = "sac_password";

#endif

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

class OvenSystem {
    public:
        OvenSystem();
        ~OvenSystem() {};

        void update(float dt);

        double getOvenTemp();

        void startOven(double time, double minCelsius);
        void stopOven();

        int getOvenTime();

        bool isOvenStartedFunc();
        bool isAuth(AsyncWebServerRequest* request);
    private:
        void initAll();

        void initOled();
        void initTempStub();
        void initWifi();
        void initServer();
        void initBtns();

        void activeLeds(bool);

        void ledsAnimation(float dt);
        bool isWifiConnected();
        bool isLedAnimationDone();

        void activeSpecificLeds(int* led, int length);

        MyOled* myOled;
        TemperatureStub* temperatureStub;
        MyServer* myServer;
        WiFiManager* wifiManager;

        double ovenTemp;

        double dtLedAnimation = 0;
        int amtAnimationTimes = 0;

        bool serverInit = false;

        double ovenTime = 0;
        double ovenCookingTime = 0;
        double ovenMinTemp = 0;

        bool isOvenStarted = false;

        Text* temp_P3;
        Text* temp_P4;
        Text* state_P4;
        Text* animation_P4;

        MyButton* resetBtn;
        MyButton* actionBtn;
};

//Converti une valeur en string
template<typename ValueType>
std::string toString(ValueType v)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << v;
    return ss.str();
}