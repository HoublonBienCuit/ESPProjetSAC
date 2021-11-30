/**
    Gestion de l'écran par la lib Screen
    @file MyOled.cpp
    @author Daniel Boisclair
    @version 1.0 21/11/29
*/


#define OVENSYSTEM_CPP

#include "../headers/OvenSystem.h"

OvenSystem::OvenSystem() {
    initAll();
}

//Permet d'initializer le OLED
void OvenSystem::initOled() {
    //init MyOled
    myOled = new MyOled();

    //Obtenir tous les éléments de l'écran
    Screen* currentScreen = myOled->getScreen();
    temp_P3 = currentScreen->getElementById("temp_P3");
    temp_P4 = currentScreen->getElementById("temp_P4");
    state_P4 = currentScreen->getElementById("state_P4");
    animation_P4 = currentScreen->getElementById("animation_P4");

    //Définir le texte des boutons
    currentScreen->getElementById("action_P1")->changeText(toString(ACTION_PIN));
    currentScreen->getElementById("reset_P1")->changeText(toString(RESET_PIN));

    //Définir le texte de connexion sur l'ESP32
    currentScreen->getElementById("ssid_P2")->changeText(string(SSID));
    currentScreen->getElementById("pass_P2")->changeText(string(PASSWORD));
}

//Initialization du TemperatureStub
void OvenSystem::initTempStub() {
    temperatureStub = new TemperatureStub();
    temperatureStub->init(DHTPIN, DHTTYPE); //Pin 15 et Type DHT22
}

//Initialization du wifi
void OvenSystem::initWifi() {
    //init wifiManager
    wifiManager = new WiFiManager();
    //wifiManager->setDebugOutput(false);
    wifiManager->setConfigPortalBlocking(false);

    Screen* currentScreen = myOled->getScreen();

    //Gestion de la connexion
    if (!wifiManager->autoConnect(SSID, PASSWORD)) {
        Serial.println("Erreur de connexion.");
        currentScreen->changePage(WIFI_PAGE);
    } else {
        Serial.println("Connexion Établie.");
    }
}

//Initialization du serveur et des routes
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

//Permet d'initializer les boutons
void OvenSystem::initBtns() {
    resetBtn = new MyButton();
    actionBtn = new MyButton();

    resetBtn->init(RESET_PIN);
    resetBtn->autoSensibilisation();

    actionBtn->init(ACTION_PIN);
    actionBtn->autoSensibilisation();
}


//Permet d'initializer tous le système
void OvenSystem::initAll() {
    pinMode(LED_JAUNE, OUTPUT);
    pinMode(LED_VERT, OUTPUT);
    pinMode(LED_ROUGE, OUTPUT);

    initOled();
    initTempStub();
    initWifi();
    initBtns();

    if (isWifiConnected())
        initServer();
}

//Fonction loop
void OvenSystem::update(float dt) {
    myOled->update();

    //Gère la température à afficher sur le OLED
    double lastTemp = ovenTemp;
    ovenTemp = temperatureStub->getTemperature();
    if (lastTemp != ovenTemp) {
        temp_P3->changeText(toString(ovenTemp));
        temp_P4->changeText(toString(ovenTemp));
    }

    //Gère l'animation des LEDS
    if (!isLedAnimationDone() && isWifiConnected()) {
        if (!serverInit)
            initServer();
        ledsAnimation(dt);
    } else if (!serverInit) {
        //Initialize le serveur à la connexion au wifi
        wifiManager->process();
    }

    //Gère le four lors de son démarrage
    if (isOvenStarted) {
        if (ovenTemp >= ovenMinTemp) {
            //gestion du temps que le four doit rester démarrer
            ovenTime += dt;
            if (ovenTime >= ovenCookingTime) {
                stopOven();
            }

            //gestion de l'animation du OLED
            if (animation_P4->getText() == "IiiIiiI")
                animation_P4->changeText("iIIiIIi");
            else
                animation_P4->changeText("IiiIiiI");
            
            //gestion de l'affichage de l'état du four
            if (state_P4->getText() != "Chauffage") {
                int activeLeds[] = {LED_ROUGE, LED_JAUNE};
                activeSpecificLeds(activeLeds, 2);
                state_P4->changeText("Chauffage");
            }
                    
        } else {
            if (ovenTemp != 0) {
                ovenTime -= dt;
                ovenTime = ovenTime <= 0 ? 0 : ovenTime;

                //gestion de l'affichage de l'état du four
                if (state_P4->getText() != "Attente") {
                    int activeLeds[] = {LED_ROUGE};
                    activeSpecificLeds(activeLeds, 1);
                    state_P4->changeText("Attente");
                    animation_P4->changeText("-------");
                }
            }
        }
    }

    //bouton reset
    if (resetBtn->checkMyButton() > 2) {
        ESP.restart();
    }

    //Bouton pour sortir du mode veille
    if (actionBtn->checkMyButton() > 2) {
        Screen* screen = myOled->getScreen();
        if (screen->getStandBy()) {
            screen->setStandBy(false);
        }
    }
}

//Si l'animation est terminé
bool OvenSystem::isLedAnimationDone() {
    return amtAnimationTimes == 2;
}

//Si le wifi est connecté
bool OvenSystem::isWifiConnected() {
    return WiFi.localIP().toString() != "0.0.0.0";
}

//Animation des lumières au démarrage de l'ESP
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
                activeSpecificLeds(activeLeds, 1);
            }
        } else {
            activeLeds(true);
        }
    }
}

//récupère la température du four
double OvenSystem::getOvenTemp() {
    return ovenTemp;
}

//allume ou éteint toutes les leds 
void OvenSystem::activeLeds(bool active) {
    digitalWrite(LED_JAUNE, active ? HIGH : LOW);
    digitalWrite(LED_VERT, active ? HIGH : LOW);
    digitalWrite(LED_ROUGE, active ? HIGH : LOW);
}

//Allume des leds spécifiques
void OvenSystem::activeSpecificLeds(int* led, int length) {
    activeLeds(false);

    for (int i = 0; i < length; i++) {
        digitalWrite(led[i], HIGH);
    }
}

//Fonction pour gérer le démarrage du four
void OvenSystem::startOven(double time, double minCelsius) {
    isOvenStarted = true;
    ovenMinTemp = minCelsius;
    ovenCookingTime = time;

    Screen* screen = myOled->getScreen();
    screen->changePage(OVEN_PAGE);

    //Déduit l'état du four
    std::string state = ovenTemp >= ovenMinTemp ? "Chauffage" : "Attente";
    state_P4->changeText(state);

    //allume les leds dépendament de l'état 
    if (state == "Chauffage") {
        int activeLeds[] = {LED_ROUGE, LED_JAUNE};
        activeSpecificLeds(activeLeds, 2);
    } else {
        int activeLeds[] = {LED_ROUGE};
        activeSpecificLeds(activeLeds, 1);
        animation_P4->changeText("-------");
    }
}

//Fonction pour gérer la fermeture du four
void OvenSystem::stopOven() {
    isOvenStarted = false;
    ovenTime = 0;
    ovenMinTemp = 0;
    ovenCookingTime = 0;

    myOled->getScreen()->changePage(READY_PAGE);

    int activeLeds[] = {LED_VERT};
    activeSpecificLeds(activeLeds, 1);
    animation_P4->changeText("-------");
}

//Si le four est démarré
bool OvenSystem::isOvenStartedFunc() {
    return isOvenStarted;
}

//Le temps présent que le four chauffe le bois
int OvenSystem::getOvenTime() {
    return ovenTime;
}


//Voir si l'utilisateur est authentifié
bool OvenSystem::isAuth(AsyncWebServerRequest* request) {
    if (request->hasHeader("Authorization")) {
        HTTPClient http;
        String loginRoute = "http://10.0.0.78:8000/api/isAuth";
        http.begin(loginRoute);
        AsyncWebHeader* header = request->getHeader("Authorization");
        String headerString = header->value().c_str();
        http.addHeader("Authorization", headerString);
        return http.GET() == 200;
    } else return false;
}