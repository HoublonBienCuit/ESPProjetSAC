#pragma once

#include <HTTPClient.h>
#include <WiFiManager.h>

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
    private:
        void initAll();

        void initOled();
        void initTempStub();
        void initWifi();
        void initServer();

        MyOled* myOled;
        TemperatureStub* temperatureStub;
        MyServer* myServer;
        WiFiManager* wifiManager;

        double ovenTemp;
};

template<typename ValueType>
std::string toString(ValueType v)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << v;
    return ss.str();
}