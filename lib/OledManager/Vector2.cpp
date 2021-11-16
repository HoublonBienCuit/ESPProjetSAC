/** Vector2.cpp 
 * Copyright (C) 2021 Daniel Boisclair
 * All rights reserved.
 *
 * Gère les vecteurs (position ou autres)
 * V1.0 (voir le fichier Vector2.h)
 **/

#include "../headers/Vector2.h"

/**
 * @brief Constructeur par défaut de la classe Vector2
 * 
 * @param x valeur en horizontale
 * @param y valeur en verticale
 */
Vector2::Vector2(int x, int y) {
    _x = x;
    _y = y;
}

/**
 * @brief Reçoit la valeur de x
 * 
 * @return int valeur de x
 */
int Vector2::getX() {
    return _x;
}

/**
 * @brief Reçoit la valeur de y
 * 
 * @return int valeur de y
 */
int Vector2::getY() {
    return _y;
}

/**
 * @brief Set la valeur de x
 * 
 * @param x la nouvelle valeur
 */
void Vector2::setX(int x) {
    _x = x;
}

/**
 * @brief Set la valeur de y
 * 
 * @param y la nouvelle valeur
 */
void Vector2::setY(int y) {
    _y = y;
}