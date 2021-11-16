/**
    Class Text : Gestion du Texte à afficher à l'écran
    @file Text.h
    @author Daniel Boisclair
    @version 1.0 22/04/2021  
    
    Historique des versions   
                            Versions  Date           Auteur            Description
                            1.0       22/04/2021     Daniel B          Première version de la classe  
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino

    lib_deps = <Aucune>

    Autres librairies (à copier dans le répertoire lib)
        Aucune

    _size par défaut = 1 pixel
    _visible par défaut = true
    _focus par défaut = false
    _texte par défaut = ""

    Exemple d'utilisation
        #include "Text.h"
        Text exempleTexte = Text(Vector2(5, 2), 2, "Un texte", "id_unique");
        cout << exempleTexte.getVisible() << endl; // true
        exempleTexte.setVisible(false);
        cout << exempleTexte.getVisible() << endl; // false

        cout << exempleTexte.getSize() << endl; // 2 (en pixel)
        Vector2 vec = exempleTexte.getPosition();

        cout << exempleTexte.getText() << endl; // Un texte
        exempleTexte.changeText("nouveau texte");
        cout << exempleTexte.getText() << endl; // nouveau texte

        cout << exempleTexte.isFocus() << endl; // false
        exempleTexte.seFocus(true);
        cout << exempleTexte.isFocus() << endl; // true
        cout << exempleTexte.getId() << endl; // id_unique

**/

#pragma once // Même chose qu'un #ifndef #define

#include <string>
#include "Vector2.h"

using namespace std;

class Text {
    //Commentaires des fonctions dans le fichier Text.cpp
    public:
        /**
         * @brief Constructeur vide de Text
         * 
         */
        Text() {};

        Text(Vector2 position, int size, string texte, string id);

        void setVisible(bool visible);
        bool getVisible();

        int getSize();
        Vector2 getPosition();

        string getText();
        void changeText(string texte);

        void setFocus(bool focus);
        bool isFocus();

        string getId();
    private:
        //Grosseur du texte en pixel
        int _size = 1;
        //La position du texte
        Vector2 _position;
        //Vrai si visible
        bool _visible = true;

        //Vrai si le focus du clavier est sur le texte
        bool _focus = false;

        //Le texte
        string _texte = "";
        //L'id de l'objet
        string _id;
};