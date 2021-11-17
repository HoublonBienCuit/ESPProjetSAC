#include "../headers/MyOled.h"

MyOled::MyOled() {
    _screen = new Screen(&Wire);
    _screen->init();

    init();
}

void MyOled::init() {
    //Page d'initialization
    Text* pageInit[MAX_INSTANCES_PER_PAGE];
    pageInit[0] = new Text(Vector2(), 2, "SAC Système", "titre_P1");
    
    _screen->addPage(1, pageInit);

    _screen->changePage(INIT_PAGE);
}

void MyOled::update(float dt) {
    _screen->update();
}