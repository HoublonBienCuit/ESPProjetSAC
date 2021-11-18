/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Alain Dubé
    @version 1.2 21/10/20
*/
#include "../headers/OvenSystem.h"

OvenSystem* MyServer::currentSystem = nullptr;

void MyServer::initAllRoutes() {
    //Initialisation du SPIFF.
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    //Mettre la route de base (dans le fichier data) dans le dossier assets
    this->serveStatic("/", SPIFFS, "/assets/");

    //Route initiale (page html de connexion)
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/views/connexion.html", "text/html");
    });

    //Route de l'application (page html)
    this->on("/application", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/views/application.html", "text/html");
    });

    this->on("/getAllWoodOptions", HTTP_GET, [](AsyncWebServerRequest *request) {
        HTTPClient http;
        String woodApiRestAddress = "http://10.0.0.78:8000/api/ObtenirTousBois";
        http.begin(woodApiRestAddress);
        http.GET();
        String response = http.getString();
        
        request->send(200, "application/json", response);
    });

    this->on("/getFourTemp", HTTP_GET, [](AsyncWebServerRequest *request) {
        std::string temperatureString = toString(currentSystem->getOvenTemp());

        request->send(200, "application/json", String(temperatureString.c_str()));
    });

    this->on("/startOven", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("") && request->hasParam("")) {
            AsyncWebParameter* cookingTimeParam = request->getParam("cookingTime");
            AsyncWebParameter* minTempParam = request->getParam("minTemp");
            const char* stringCookingTime = cookingTimeParam->value().c_str();
            const char* stringOvenMinTemp = minTempParam->value().c_str();

            currentSystem->startOven(std::strtod(stringCookingTime, nullptr), std::strtod(stringOvenMinTemp, nullptr));

            request->send(200);
        } else {
            request->send(500);
        }
        
    });

    //Route non existante
    this->onNotFound([](AsyncWebServerRequest *request){
        if (request->url().compareTo(String("connexion")))
            request->redirect("/");
        else
            request->send(404, "text/plain", "Page Not Found");
    });
    
    this->begin();
};