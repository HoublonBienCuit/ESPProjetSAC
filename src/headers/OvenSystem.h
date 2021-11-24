#pragma once

#include <HTTPClient.h>
#include <WiFiManager.h>
#include <Arduino.h>
#include <vector>

#include "../headers/MyOled.h"
#include "../headers/TemperatureStub.h"
#include "../headers/MyServer.h"

//get ride of this
//#include "myFunctions.cpp"

#define DHTPIN  15   // Pin utilisée par le senseur DHT11 / DHT22
#define DHTTYPE DHT22  //Le type de senseur utilisé (mais ce serait mieux d'avoir des DHT22 pour plus de précision)

#define DEFAULT_PORT 80

#define ACTION_PIN 33
#define RESET_PIN 32

#define LED_JAUNE 27
#define LED_VERT 14
#define LED_ROUGE 12

//Variables déclarées seulement dans OvenSystem.cpp
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
};

template<typename ValueType>
std::string toString(ValueType v)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << v;
    return ss.str();
}