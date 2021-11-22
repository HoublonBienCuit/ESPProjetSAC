#define OVENSYSTEM_CPP

#include "../headers/OvenSystem.h"

OvenSystem::OvenSystem() {
    initAll();
}

void OvenSystem::initOled() {
    //init MyOled
    myOled = new MyOled();

    Screen* currentScreen = myOled->getScreen();
    temp_P3 = currentScreen->getElementById("temp_P3");
    temp_P4 = currentScreen->getElementById("temp_P4");
    state_P4 = currentScreen->getElementById("state_P4");

    //Définir le texte des boutons
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
    //wifiManager->setDebugOutput(false);
    wifiManager->setConfigPortalBlocking(false);

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

    serverInit = true;
}

void OvenSystem::initAll() {
    pinMode(LED_JAUNE, OUTPUT);
    pinMode(LED_VERT, OUTPUT);
    pinMode(LED_ROUGE, OUTPUT);

    initOled();
    initTempStub();
    initWifi();

    if (isWifiConnected())
        initServer();
}

void OvenSystem::update(float dt) {
    myOled->update();

    double lastTemp = ovenTemp;
    ovenTemp = temperatureStub->getTemperature();
    if (lastTemp != ovenTemp) {
        temp_P3->changeText(toString(ovenTemp));
        temp_P4->changeText(toString(ovenTemp));
    }

    if (!isLedAnimationDone() && isWifiConnected()) {
        if (!serverInit)
            initServer();
        ledsAnimation(dt);
    } else if (!serverInit) {
        Serial.println("[1]-Server not init, processing...");
        wifiManager->process();
        Serial.println("[2]-Server not init, processing...");
    }

    if (isOvenStarted) {
        if (ovenTemp >= ovenMinTemp) {
            ovenTime += dt;
            if (ovenTime >= ovenCookingTime) {
                stopOven();
            }

            if (state_P4->getText() != "Chauffage") {
                int activeLeds[] = {LED_ROUGE};
                activeSpecificLeds(activeLeds);
                state_P4->changeText("Chauffage");
            }
                    
        } else {
            if (ovenTemp != 0) {
                ovenTime -= dt;
                ovenTime = ovenTime <= 0 ? 0 : ovenTime;

                if (state_P4->getText() != "Attente") {
                    int activeLeds[] = {LED_ROUGE, LED_JAUNE};
                    activeSpecificLeds(activeLeds);
                    state_P4->changeText("Attente");
                }
            }
        }
    }
}

bool OvenSystem::isLedAnimationDone() {
    return amtAnimationTimes == 2;
}

bool OvenSystem::isWifiConnected() {
    return !wifiManager->getWiFiSSID().isEmpty();
}

void OvenSystem::ledsAnimation(float dt) {
    dtLedAnimation += dt;
    if (dtLedAnimation >= 1) {
        if (dtLedAnimation >= 2) {
            activeLeds(false);

            dtLedAnimation = 0;
            amtAnimationTimes++;

            if (amtAnimationTimes == 2) {
                myOled->getScreen()->changePage(READY_PAGE);
                
                int activeLeds[] = {LED_VERT};
                activeSpecificLeds(activeLeds);
            }
        } else {
            activeLeds(true);
        }
    }
}

double OvenSystem::getOvenTemp() {
    return ovenTemp;
}

void OvenSystem::activeLeds(bool active) {
    digitalWrite(LED_JAUNE, active ? HIGH : LOW);
    digitalWrite(LED_VERT, active ? HIGH : LOW);
    digitalWrite(LED_ROUGE, active ? HIGH : LOW);
}

void OvenSystem::activeSpecificLeds(int led[]) {
    activeLeds(false);

    for (int i = 0; i < sizeof(led) / sizeof(led[0]); i++) {
        digitalWrite(led[i], HIGH);
    }
}

void OvenSystem::startOven(double time, double minCelsius) {
    isOvenStarted = true;
    ovenMinTemp = minCelsius;
    ovenCookingTime = time;

    Screen* screen = myOled->getScreen();
    screen->changePage(OVEN_PAGE);

    std::string state = ovenTemp >= ovenMinTemp ? "Chauffage" : "Attente";
    state_P4->changeText(state);

    if (state == "Chauffage") {
        int activeLeds[] = {LED_ROUGE, LED_JAUNE};
        activeSpecificLeds(activeLeds);
    } else {
        int activeLeds[] = {LED_ROUGE};
        activeSpecificLeds(activeLeds);
    }
}

void OvenSystem::stopOven() {
    isOvenStarted = false;
    ovenTime = 0;
    ovenMinTemp = 0;
    ovenCookingTime = 0;

    myOled->getScreen()->changePage(READY_PAGE);

    int activeLeds[] = {LED_VERT};
    activeSpecificLeds(activeLeds);
}

bool OvenSystem::isOvenStartedFunc() {
    return isOvenStarted;
}

int OvenSystem::getOvenTime() {
    return ovenTime;
}