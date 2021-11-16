/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Alain Dubé
    @version 1.2 21/10/20
*/
#include <Arduino.h>
#include "../headers/MyServer.h"
using namespace std;

typedef std::string (*CallbackType)(std::string);
CallbackType MyServer::ptrToCallBackFunction = NULL;

//Exemple pour appeler une fonction CallBack
//if (ptrToCallBackFunction) (*ptrToCallBackFunction)(stringToSend);
void MyServer::initCallback(CallbackType callback) {
    ptrToCallBackFunction = callback;
}

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
        String woodApiRestAddress = "http://172.16.206.86:8000/api/ObtenirTousBois";
        http.begin(woodApiRestAddress);
        http.GET();
        String response = http.getString();
        
        request->send(200, "application/json", response);
    });

    this->on("/getFourTemp", HTTP_GET, [](AsyncWebServerRequest *request) {
        string temperatureString = "";
        if (ptrToCallBackFunction) temperatureString = (*ptrToCallBackFunction)("ObtenirTemperature");

        request->send(200, "application/json", String(temperatureString.c_str()));
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