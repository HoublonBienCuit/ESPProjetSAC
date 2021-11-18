/* Copyright (C) 2021 Daniel Boisclair
 * All rights reserved.
 *
 * Projet Sac
 * Ecole du Web
 * Cours Objets connectés (c)2021
 *  
    @file     main.cpp
    @author   Alain Dubé
    @version  1.1 21/08/15 

    Historique des versions
           Version    Date       Auteur       Description
           1.1        21/08/15  Alain        Première version du logiciel
		   1.2        21/09/15  Daniel       Ajout de fonctionnalités du logiciel

    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
                      
            ESPAsyncWebServer-esphome                   (Pour accéder au Wifi)
            AsyncTCP-esphome                            (Pour utiliser les focntionnalités TCP)
            bblanchon/ArduinoJson@^6.17.2               (Pour accéder au fonctionnalités Json)

    Autres librairies (à copier dans le répertoire lib)
         WifiManagerDevelopment
            //Remarques
            //Pour trouver le WifiManager (dans la branche development)
            //   https://github.com/tzapu/WiFiManager/tree/development
            //   Ne pas oublier d'appuyez sur l'ampoule et choisir : ajouter Lib
    
    Fonctions utiles (utilitaires)
        /lib/MYLIB/myFunctions.cpp
            //Pour vérifier plus simplement que deux chaines sont identiques
            bool isEqualString(std::string line1, std::string line2)
            //Pour extraire une partie d'une chaine de caractères avec l'aide d'un index
            std::string getValue(std::string data, char separator, int index)
            //Pour remplacer plus facilement une sous chaine
            bool replaceAll(std::string& source, const std::string& from, const std::string& to)
            //Pour obtenir un chaine aléatoire d'une certaine longeur
            std::string get_random_string(unsigned int len)

    Classes du système
         
        MyServer                        V1.0    Pour la gestion des routes pour le site WEB
            /data                               Répertoire qui contient les fichiers du site WEB 
                index.html              V1.0    Page index du site WEB
                index.css               V1.0    CSS
                script.js               V1.0    JS (fonctions JavaScript)
              
 * */
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