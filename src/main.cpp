/* Copyright (C) 2021 Daniel Boisclair
 * All rights reserved.
 *
 * Projet Sac
 * Ecole du Web
 * Cours Objets connectés (c)2021
 *  
    @file     main.cpp
    @author   Daniel Boisclair
    @version  1.2 21/09/15 

    Historique des versions
           Version    Date       Auteur       Description
           1.1        21/08/15  Alain        Première version du logiciel
		   1.2        21/09/15  Daniel       Ajout de fonctionnalités du logiciel

    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        ESPAsyncWebServer-esphome
        AsyncTCP-esphome
        bblanchon/ArduinoJson@^6.17.2
        adafruit/Adafruit GFX Library @ ^1.10.1
        adafruit/Adafruit SSD1306 @ ^2.4.0

    Autres librairies (à copier dans le répertoire lib)
         WifiManagerDevelopment
            //Remarques
            //Pour trouver le WifiManager (dans la branche development)
            //   https://github.com/tzapu/WiFiManager/tree/development
            //   Ne pas oublier d'appuyez sur l'ampoule et choisir : ajouter Lib
         OledManager
            Framework privé qui gère la petite écran OLED

    Classes du système
         
        MyServer                        V1.0    Pour la gestion des routes pour le site WEB
            /data                               Répertoire qui contient les fichiers du site WEB 
                /css
                    signin.css              V1.0    CSS de la page de connexion
                    style.css               V1.0    CSS de la page de l'application
                /images
                    logo.png                V1.0    Logo de l'entreprise
                /js
                    application.js          V1.0    JS de la page de l'application
                    index.js                V1.0    JS de la page de connexion
                /views
                    connexion.html          V1.0    Page index du site WEB
                    application.html        V1.0    Page de l'application
                
              
 **/
#include <Arduino.h>

#include "headers/OvenSystem.h"
OvenSystem* ovenSystem;

void setup() { 
    Serial.begin(9600);

    //Création du système principale
    ovenSystem = new OvenSystem();
 }

 //Variables pour calculer le temps entre deux updates
float lastUpdate = 0, now = 0;

//Variable pour barrer la boucle à 30 frames par seconde
const int FPS = 30;
const int frameDelay = 1000 / FPS;

/**
 * @brief loop du programme, utilisé par Arduino
 * 
 */
void loop() {
     now = millis();

    //Temps entre deux updates en milliseconde
    int dt = now - lastUpdate;
    
    //Condition qui permet de barrer la boucle à 30 frames par secondes
    if (dt < frameDelay) {
        delay(frameDelay - dt);

        now = millis();
    }
    lastUpdate = now;

    //update l'objet ovenSystem et passe en paramètre le dt en seconde
    ovenSystem->update((dt + (frameDelay - dt)) / 1000.0f);// convertir en seconde
}