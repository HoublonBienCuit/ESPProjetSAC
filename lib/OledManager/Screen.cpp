/** Screen.cpp 
 * Copyright (C) 2021 Daniel Boisclair
 * All rights reserved.
 *
 * Gère l'écran
 * V1.0 (voir le fichier Screen.h)
 **/

#include "Screen.h"

/**
 * @brief Constructeur par défaut de la classe Screen
 * 
 * @param twi pointeur de l'objet TwoWire
 * @param RST Pin GPIO du Reset
 * @param rawHeight Hauteur de l'écran en pixel
 * @param rawWidth Largeur de l'écran en pixel
 */
Screen::Screen(TwoWire *twi, uint8_t RST, uint8_t rawHeight, uint8_t rawWidth) {
    _display = Adafruit_SSD1306(rawWidth, rawHeight, twi, RST);
}

/**
 * @brief Méthode d'initialization de l'écran et de l'objet display
 * 
 * @param addrI2C l'adresse I2C utilisé
 * @return int 0 : si l'initialization c'est n'a pas d'erreur, 1 : si l'initialization a eu une erreur
 */
int Screen::init(uint8_t addrI2C) {
    if (!_display.begin(SSD1306_SWITCHCAPVCC, addrI2C)) {
        return 1;
    }

    _display.cp437(true);

    //Clearing old memory bug
    clearScreen();
    //create text for system ?

    return 0;
}

/**
 * @brief Méthode qui update l'anneau à chaque frame
 * 
 * @param dt le temps en seconde entre deux frames
 */
void Screen::update() {
    if (_standBy == false) {
        _display.clearDisplay();
        //Seulement pour du texte
        for (int index = 0; index < _instancesPerPage[_currentPage]; index++)
        {
            Text* texte = _pages[_currentPage][index];
            if (texte->getVisible()) {
                _display.setTextColor(SSD1306_WHITE);
                _display.setTextSize(texte->getSize());

                Vector2 position = texte->getPosition();
                _display.setCursor(position.getX(), position.getY());

                // Permet l'écriture de spécials caractères comme é, É, è //
                //_display.print(texte->getText().c_str());
                string stringText = texte->getText();
                const char* charText = stringText.c_str();

                for (int i = 0; i < stringText.length(); i++){
                    if (i == stringText.find("é")) {
                        _display.print((char)130);
                        i++;
                    } else if (i == stringText.find("É")) {
                        _display.print((char)144);
                        i++;
                    } else if (i == stringText.find("è")) {
                        _display.print((char)138);
                        i++;
                    }
                    else {
                        _display.print(charText[i]);
                    }
                }
                
            }
        }
        _display.display();
    }
}

/**
 * @brief Méthode qui nettoye l'écran (Retire tous ce qui est à l'écran)
 * 
 */
void Screen::clearScreen() {
    _display.clearDisplay();
    _display.display();
}

/**
 * @brief Méthode qui permet de mettre l'écran en veille
 * 
 * @param standBy vrai pour mettre en veille, faux pour ne pas mettre en veille
 */
void Screen::setStandBy(bool standBy) {
    _standBy = standBy;

    if (_standBy == true) {
        clearScreen();
    }
}

/**
 * @brief Méthode qui permet de s'avoir si l'écran est en veille
 * 
 * @return true si l'écran est en veille
 * @return false si l'écran n'est pas en veille
 */
bool Screen::getStandBy() {
    return _standBy;
}

/**
 * @brief Ajoute une page à l'écran
 * 
 * @param te nombre d'éléments(d'objet de type Text) dans la page
 * @param elements un array contenent tous les textes à afficher
 */
void Screen::addPage(int te, Text* elements[]) {
    _instancesPerPage[_amtPages] = te;
    for (int index = 0; index < te; index++) {
        _pages[_amtPages][index] = elements[index];
    }

    _amtPages++;
}

/**
 * @brief Méthode qui permet de changer de page
 * 
 * @param page numéro de la page à afficher
 */
void Screen::changePage(int page) {
    if (_currentPage != page) {
        _currentPage = page;
        Serial.print(page);
    }
}

/**
 * @brief Méthode de service qui renvoit l'élément focusé dans la page
 * 
 * @return Text* un pointeur sur l'objet de type Text
 */
Text* Screen::getFocusedElement() {
    for (int index = 0; index < _instancesPerPage[_currentPage]; index++)
    {
        Text* text = _pages[_currentPage][index];
        if (text->isFocus()) {
            return text;
        }
    }

    return nullptr;
}

/**
 * @brief Méthode qui permet de recevoir un Text par son id
 * 
 * @param id id unique du Text
 * @return Text* un pointeur sur le Text
 */
Text* Screen::getElementById(string id) {
    for (int i = 0; i < _amtPages; i++) {
        for (int j = 0; j < _instancesPerPage[i]; j++) {
            if (_pages[i][j]->getId() == id) {
                return _pages[i][j];
            }
        }
    }

    return nullptr;
}

/**
 * @brief Méthode qui permet de recevoir la page actuelle
 * 
 * @return int la page actuelle
 */
int Screen::getCurrentPage() {
    return _currentPage;
}

/**
 * @brief Méthode qui permet de set une événement pour reset une page
 * 
 * @param event l'événement du reset
 * @param page la page à reseter
 */
/*void Screen::setResetEvent(Event* event, int page) {
    _resetEvents[page] = event;
}*/

/**
 * @brief Méthode qui permet d'activer l'événement pour reset la page
 * 
 * @param currentSystem le systeme actuelle
 * @param page numéro de la page à reset
 */
/*void Screen::resetPage(DoorSystem* currentSystem, int page) {
    if (_resetEvents[page] != nullptr)
        _resetEvents[page]->forceEvent(currentSystem);
}*/