/** Text.cpp 
 * Copyright (C) 2021 Daniel Boisclair
 * All rights reserved.
 *
 * Gère l'objet Text pour l'écran
 * V1.0 (voir le fichier Text.h)
 **/

#include "../headers/Text.h"

/**
 * @brief Constructeur par défaut de la classe Text
 * 
 * @param position la position en Vector2
 * @param size le size en int (représente les pixels)
 * @param texte le texte à écrire
 * @param id l'id unique du texte
 */
Text::Text(Vector2 position, int size, string texte, string id) {
    _position = position;
    _size = size;
    _texte = texte;

    _id = id;
}

/**
 * @brief Méthode qui permet de rendre visible ou non un Text
 * 
 * @param visible vrai pour rendre visible, false pour invisible
 */
void Text::setVisible(bool visible) {
    _visible = visible;
}

/**
 * @brief Méthode qui renvoit la visibilité du Text
 * 
 * @return true si le Text est visible
 * @return false si le Text est invisible
 */
bool Text::getVisible() {
    return _visible;
}

/**
 * @brief Renvoit le size du Text
 * 
 * @return int le size en pixel
 */
int Text::getSize() {
    return _size;
}

/**
 * @brief Méthode qui renvoit la position du Text
 * 
 * @return Vector2 position x, y 
 */
Vector2 Text::getPosition() {
    return _position;
}

/**
 * @brief Méthode qui renvoit le texte du Text
 * 
 * @return string le texte
 */
string Text::getText() {
    return _texte;
}

/**
 * @brief Méthode qui permet de changer le texte du Text
 * 
 * @param texte le nouveau texte
 */
void Text::changeText(string texte) {
    _texte = texte;
}

/**
 * @brief Méthode qui permet de set le focus ou non
 * 
 * @param focus vrai pour mettre le focus sur ce Text, faux pour enlever le focus
 */
void Text::setFocus(bool focus) {
    _focus = focus;
}

/**
 * @brief Méthode qui renvoit si le Text est focusé
 * 
 * @return true si le Text est focusé
 * @return false si le Text n'est pas focusé
 */
bool Text::isFocus() {
    return _focus;
}

/**
 * @brief Méthode qui renvoit l'id du Text
 * 
 * @return string l'id unique
 */
string Text::getId() {
    return _id;
}