#define OVENSYSTEM_CPP

#include "../headers/OvenSystem.h"

OvenSystem::OvenSystem() {
    initAll();
}

void OvenSystem::initOled() {
    //init MyOled
    myOled = new MyOled();

    //Définir le texte des boutons
    Screen* currentScreen = myOled->getScreen();
    currentScreen->getElementById("action_P1")->changeText(toString(ACTION_PIN));
    currentScreen->getElementById("reset_P1")->changeText(toString(RESET_PIN));

    //Définir le texte de connexion sur l'ESP32
    currentScreen->getElementById("ssid_P2")->changeText(string(SSID));
    currentScreen->getElementById("pass_P2")->changeText(string(PASSWORD));
}

//init TemperatureStub
void OvenSystem::initTempStub() {
    
    temperatureStub = new TemperatureStub();
    temperatureStub->init(DHTPIN, DHTTYPE); //Pin 15 et Type DHT22
}

void OvenSystem::initWifi() {
    //init wifiManager
    wifiManager = new WiFiManager();

    Screen* currentScreen = myOled->getScreen();

    if (!wifiManager->autoConnect(SSID, PASSWORD)) {
        Serial.println("Erreur de connexion.");
        currentScreen->changePage(WIFI_PAGE);
    } else {
        Serial.println("Connexion Établie.");
    }
}

void OvenSystem::initServer() {
    //init MyServer
    myServer = new MyServer(DEFAULT_PORT);
    MyServer::currentSystem = this;
    myServer->initAllRoutes();

    Screen* currentScreen = myOled->getScreen();

    string ipv4Adress = WiFi.localIP().toString().c_str();
    currentScreen->getElementById("ip_adress_P3")->changeText(ipv4Adress);
    currentScreen->getElementById("ip_adress_P4")->changeText(ipv4Adress);
}

void OvenSystem::initAll() {
    initOled();
    initTempStub();
    initWifi();
    initServer();
}

void OvenSystem::update(float dt) {
    myOled->update();

    double lastTemp = ovenTemp;
    ovenTemp = temperatureStub->getTemperature();
    if (lastTemp != ovenTemp) {
        Screen* currentScreen = myOled->getScreen();
        currentScreen->getElementById("temp_P3")->changeText(toString(ovenTemp));
        currentScreen->getElementById("temp_P4")->changeText(toString(ovenTemp));
    }
}

double OvenSystem::getOvenTemp() {
    return ovenTemp;
}