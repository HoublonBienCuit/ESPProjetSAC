/**
    Class Screen : Gestion de l'écran connecté à l'esp32
    @file Screen.h
    @author Daniel Boisclair
    @version 1.0 22/04/2021  
    
    Historique des versions   
                            Versions  Date           Auteur            Description
                            1.0       22/04/2021     Daniel B          Première version de la classe
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino

    lib_deps =
        -Adafruit_GFX
        -Adafruit_SSD1306
        -Adafruit_I2CDevice

    Autres librairies (à copier dans le répertoire lib)
        Aucune

    _amtPages = 0 par défaut

    Exemple d'utilisation
        #include "Screen.h"

        #define SCREEN_WIDTH 128        // OLED display width, in pixels
        #define SCREEN_HEIGHT 64        // OLED display height, in pixels
        #define OLED_RESET 4            // Reset pin # (or -1 if sharing Arduino reset pin)
        #define OLED_I2C_ADDRESS 0x3C   // Adresse I2C de l'écran Oled

        //Fonction pour un évènement pour reset une page
        void resetExemple(Event* self, DoorSystem* currentSystem) {
            cout << "reset effectué sur la page" << endl;
        }

        Screen monEcran = Screen(&Wire);
        monEcran.init(OLED_I2C_ADDRESS);

        monEcran.setStandBy(true); //Met en veille
        cout << monEcran.getStandBy() << endl; // true
        monEcran.setStandBy(false); //Sort de la veille

        Text* page[MAX_INSTANCES_PER_PAGE];
        page[0] = new Text(Vector2(), 1,     "Ceci est un titre", "titre");  //Titre
        page[1] = new Text(Vector2(0, 8), 1, "Entre le texte ici", "texte");  //du texte

        //Met le focus du clavier sur le texte
        page[1].setFocus(true);

        //Ajoute la page à l'écran
        monEcran.addPage(3, page); // ajoute une page avec 3 instances de type Text

        monEcran.changePage(0);//Numéro de la page = 0 puisque c'est la première page

        cout << monEcran.getFocusedElement()->getText() << endl; // Entre le texte ici
        cout << monEcran.getElementById("titre")->getText() << endl; // Ceci est un titre
        cout << monEcran.getCurrentPage() << endl; // 0

        //Ajoute l'évènement pour reset la page
        monEcran.setResetEvent(new Event((EventCall)resetExemple, "reset_page_exemple"), 0);
        monEcran.resetPage(new DoorSystem(), 0); //reset effectué sur la page
        
        void loop() {
            monEcran.update(); // update l'écran à chaque frame
        }
**/

#pragma once // Même chose qu'un #ifndef #define

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>

#include "Text.h"

//#include "Event.h"

using namespace std;

//Le nombre maximum de pages pour l'écran
int const MAX_PAGES = 6;
//Le nombre d'instances (d'objet texte) maximum par page
int const MAX_INSTANCES_PER_PAGE = 10;

//Réécriture de la classe DoorSystem pour pouvoir accéder à cette classe
//class DoorSystem;

//Variables d'initialization de l'écran
#define DEFAULT_SCREEN_WIDTH 128        // OLED display width, in pixels
#define DEFAULT_SCREEN_HEIGHT 64        // OLED display height, in pixels
#define DEFAULT_OLED_RESET 4            // Reset pin # (or -1 if sharing Arduino reset pin)
#define DEFAULT_OLED_I2C_ADDRESS 0x3C   // Adresse I2C de l'écran Oled

/**
 * @brief Classe représentant l'écran du système
 * 
 */
class Screen {
    //Commentaires des fonctions dans le fichier Screen.cpp
    public:
        Screen(TwoWire *twi, uint8_t RST = DEFAULT_OLED_RESET, uint8_t rawHeight = DEFAULT_SCREEN_HEIGHT, uint8_t rawWidth = DEFAULT_SCREEN_WIDTH);
        /**
         * @brief Déconstructeur de la classe Screen
         */
        ~Screen() {};

        int init(uint8_t addrI2C = DEFAULT_OLED_I2C_ADDRESS);
        void update();

        void clearScreen();

        void setStandBy(bool standBy);
        bool getStandBy();

        void addPage(int te, Text* elements[]);
        void changePage(int page);

        Text* getFocusedElement();
        Text* getElementById(string id);

        int getCurrentPage();

        //void setResetEvent(Event* event, int page);
        //void resetPage(DoorSystem* currentSystem, int page);
    private:
        //Objet représentant le display
        Adafruit_SSD1306 _display;

        //Array contentant les pointeurs des objets Text dans chaque page
        Text* _pages[MAX_PAGES][MAX_INSTANCES_PER_PAGE];
        //Array contenant le nombre d'instances par page
        int _instancesPerPage[MAX_PAGES];
        //Page actuelle
        int _currentPage;
        //Nombre de pages
        int _amtPages = 0;

        //Les évènements pour reset une page
        //Event* _resetEvents[MAX_PAGES] = {nullptr};

        //Vrai si l'écran est en veille
        bool _standBy = false;
};