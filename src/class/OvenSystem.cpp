#define OVENSYSTEM_CPP

#include "../headers/OvenSystem.h"

OvenSystem::OvenSystem() {
    init();
}

void OvenSystem::init() {
    //init MyOled
    myOled = new MyOled();

    //init TemperatureStub
    temperatureStub = new TemperatureStub();
    temperatureStub->init(DHTPIN, DHTTYPE); //Pin 15 et Type DHT22

    //Définir le texte des boutons
    Screen* currentScreen = myOled->getScreen();
    currentScreen->getElementById("action_P1")->changeText(toString(ACTION_PIN));
    currentScreen->getElementById("reset_P1")->changeText(toString(RESET_PIN));

    //Définir le texte de connexion sur l'ESP32
    currentScreen->getElementById("ssid_P2")->changeText(string(SSID));
    currentScreen->getElementById("pass_P2")->changeText(string(PASSWORD));

    //init wifiManager
    wifiManager = new WiFiManager();

    if (!wifiManager->autoConnect(SSID, PASSWORD)) {
        Serial.println("Erreur de connexion.");
        currentScreen->changePage(WIFI_PAGE);
    } else {
        Serial.println("Connexion Établie.");
    }

    //init MyServer
    myServer = new MyServer(DEFAULT_PORT);
    myServer->initAllRoutes();
    //myServer->initCallback(&CallBackMessageListener);

    currentScreen->changePage(READY_PAGE);

    string ipv4Adress = WiFi.localIP().toString().c_str();
    currentScreen->getElementById("ip_adress_P3")->changeText(ipv4Adress);
    currentScreen->getElementById("ip_adress_P4")->changeText(ipv4Adress);
}

void OvenSystem::update(float dt) {
    myOled->update(dt);

    double lastTemp = ovenTemp;
    ovenTemp = temperatureStub->getTemperature();
    if (lastTemp != ovenTemp) {
        Screen* currentScreen = myOled->getScreen();
        currentScreen->getElementById("temp_P3")->changeText(toString(ovenTemp));
        currentScreen->getElementById("temp_P4")->changeText(toString(ovenTemp));
    }
}