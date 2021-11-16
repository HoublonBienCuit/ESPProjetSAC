/** Event.cpp 
 * Copyright (C) 2021 Daniel Boisclair
 * All rights reserved.
 *
 * Gère les évènements
 * V1.0 (voir le fichier Event.h)
 **/

#include "headers/Event.h"

Event::Event(EventCall tevent, string id) {
    _event = tevent;
    _id = id;
}

void Event::active(bool enabled) {
    _enabled = enabled;

    if (!_enabled)
        reset();
}

bool Event::isActive() {
    return _enabled;
}

void Event::forceEvent(DoorSystem* currentSystem) {
    _event(this, currentSystem);
}

string Event::getId() {
    return _id;
}

void Event::addTime(float aTime) {
    _time += aTime;
}

void Event::reset() {
    _time = 0.0f;
}

float Event::getTime() {
    return _time;
}