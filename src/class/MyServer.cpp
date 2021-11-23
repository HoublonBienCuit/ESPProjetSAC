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
        if (!currentSystem->isAuth(""))
            request->send(SPIFFS, "/views/connexion.html", "text/html");
        else
            request->redirect("/application");
        
    });

    //Route de l'application (page html)
    this->on("/application", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (currentSystem->isAuth(""))
            request->send(SPIFFS, "/views/application.html", "text/html");
        else
            request->redirect("/");
    });

    this->on("/getAllWoodOptions", HTTP_GET, [](AsyncWebServerRequest *request) {
        HTTPClient http;
        String woodApiRestAddress = "http://10.0.0.78:8000/api/ObtenirTousBois";
        http.begin(woodApiRestAddress);
        http.GET();
        String response = http.getString();
        
        request->send(200, "application/json", response);
    });

    this->on("/startOven", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("cookingTime") && request->hasParam("minTemp")) {
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

    this->on("/stopOven", HTTP_GET, [](AsyncWebServerRequest *request) {
        currentSystem->stopOven();

        request->send(200);
    });

    this->on("/getOvenCookingInformations", HTTP_GET, [](AsyncWebServerRequest *request) {
        String ovenCookingTime = toString(currentSystem->getOvenTime()).c_str();
        String temperatureString = toString(currentSystem->getOvenTemp()).c_str();
        String isOvenStartedString = currentSystem->isOvenStartedFunc() ? "true" : "false";

        request->send(200, "application/json", String("{\"isStarted\": " + isOvenStartedString + ", \"time\":" + ovenCookingTime + ", \"temp\": " + temperatureString + " }"));
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