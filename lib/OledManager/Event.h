/**
    Class Event : Class représentant un évènement
    @file Event.h
    @author Daniel Boisclair
    @version 1.0 22/04/2021  
    
    Historique des versions   
                            Versions  Date           Auteur            Description
                            1.0       22/04/2021     Daniel B          Première version de la classe  
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino

    lib_deps = 
        string

    Autres librairies (à copier dans le répertoire lib)
        Aucune

    _time par défaut = 0.0f

    Exemple d'utilisation
        #include "Event.h"

        //Fonction pour un évènement pour un test
        void eventExemple(Event* self, DoorSystem* currentSystem) {
            cout << "exemple d'une fonction pour évènement" << endl;
        }

        Event eventExemple = Event((EventCall)eventExemple, "id_unique");
        eventExemple.forceEvent(new DoorSystem()); //exemple d'une fonction pour évènement
        cout << eventExemple.getId() << endl; // id_unique

        eventExemple.active(true);
        cout << eventExemple.isActive() << endl; // true
        eventExemple.addTime(1.0f);
        cout << eventExemple.getTime() << endl; // 1.0f
        eventExemple.reset()
        cout << eventExemple.getTime() << endl; // 0.0f
**/

#pragma once // Même chose qu'un #ifndef #define
#include <string>

using namespace std;

//Réécriture des noms des classes pour pouvoir y accéder pour le typedef
class DoorSystem;
class Event;

//Ajout d'un "synonyme" du type void pour créer des fonctions unique
typedef void (*EventCall)(Event*, DoorSystem*);

/**
 * @brief Classe représentant un évènement
 * 
 */
class Event {
    //Commentaires des fonctions dans le fichier Event.cpp
    public:
        Event(EventCall event, string id);
        ~Event() {};

        void active(bool enabled);
        bool isActive();
        void forceEvent(DoorSystem* currentSystem);
        string getId();

        void addTime(float aTime);
        void reset();
        float getTime();
    private:
        //Vrai si l'évènement est activé
        bool _enabled = false;

        //Id unique d'un évènement
        string _id;

        //Fonction à appeller si l'évènement est activé
        EventCall _event;

        //Temps en seconde du temps que l'évènement est activé
        float _time = 0.0f;
};