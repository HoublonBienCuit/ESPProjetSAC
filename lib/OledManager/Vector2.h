/**
    Class Vector2 : Gestion des vecteurs
    @file Vector2.h
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

    _x, _y par défaut = 0

    Exemple d'utilisation
        #include "Vector2.h"
        Vector2 exempleVector2 = Vector2(5, 2);
        cout << exempleVector2.getX() << ", " << exempleVector2.getY() << endl; // 5, 2
        exempleVector2.setX(8);
        exempleVector2.setY(8);
        cout << exempleVector2.getX() << ", " << exempleVector2.getY() << endl; // 8, 8
**/

#pragma once // Même chose qu'un #ifndef #define

class Vector2 {
    //Commentaires des fonctions dans le fichier Vector2.cpp
    public:
        Vector2(int x, int y);

        /**
         * @brief Constructeur vide de Vector2
         */
        Vector2() {};
        /**
         * @brief Destructeur vide de Vector2
         */
        ~Vector2() {};

        int getX();
        int getY();
        void setX(int x);
        void setY(int y);

    private:
        //position ou size x du vecteur
        int _x = 0;
        //position ou size y du vecteur
        int _y = 0;
};