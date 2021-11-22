#include "../headers/MyOled.h"

MyOled::MyOled() {
    _screen = new Screen(&Wire);
    _screen->init();

    init();
}

void MyOled::init() {
    //Page d'initialization
    Text* pageInit[MAX_INSTANCES_PER_PAGE];
    pageInit[0] = new Text(Vector2(32, 0), 1, "SAC Système", "titre_P1");
    pageInit[1] = new Text(Vector2(0, 15), 1, "Id: ID3944", "id_P1");
    pageInit[2] = new Text(Vector2(0, 27), 1, "INITIALISATION", "init_P1");
    pageInit[3] = new Text(Vector2(0, 39), 1, "Bouton ACTION:", "action_label_P1");
    pageInit[4] = new Text(Vector2(85, 39), 1, "", "action_P1");
    pageInit[5] = new Text(Vector2(0, 51), 1, "Bouton RESET:", "reset_label_P1");
    pageInit[6] = new Text(Vector2(80, 51), 1, "", "reset_P1");
    _screen->addPage(7, pageInit);

    //Page du wifi
    Text* pageWifi[MAX_INSTANCES_PER_PAGE];
    pageWifi[0] = new Text(Vector2(32, 0), 1, "SAC Système", "titre_P2");
    pageWifi[1] = new Text(Vector2(0, 15), 1, "Id: SAC système", "id_P2");
    pageWifi[2] = new Text(Vector2(0, 27), 1, "AP configuration", "ap_P2");
    pageWifi[3] = new Text(Vector2(10, 39), 1, "SSID:", "ssid_label_P2");
    pageWifi[4] = new Text(Vector2(43, 39), 1, "", "ssid_P2");
    pageWifi[5] = new Text(Vector2(10, 51), 1, "PASS:", "pass_label_P2");
    pageWifi[6] = new Text(Vector2(43, 51), 1, "", "pass_P2");
    _screen->addPage(7, pageWifi);

    //Page ready
    Text* pageReady[MAX_INSTANCES_PER_PAGE];
    pageReady[0] = new Text(Vector2(32, 0), 1, "SAC Système", "titre_P3");
    pageReady[1] = new Text(Vector2(0, 15), 1, "Id: 92834", "id_P3");
    pageReady[2] = new Text(Vector2(90, 15), 1, "Pret", "ap_P3");
    pageReady[3] = new Text(Vector2(90, 27), 1, "", "temp_P3");
    pageReady[4] = new Text(Vector2(50, 51), 1, "", "ip_adress_P3");
    _screen->addPage(5, pageReady);

    //Page four
    Text* pageOven[MAX_INSTANCES_PER_PAGE];
    pageOven[0] = new Text(Vector2(32, 0), 1, "SAC Système", "titre_P4");
    pageOven[1] = new Text(Vector2(0, 15), 1, "Id: 92834", "id_P4");
    pageOven[2] = new Text(Vector2(70, 15), 1, "", "state_P4");
    pageOven[3] = new Text(Vector2(70, 27), 1, "-o-o-o-", "animation_P4");
    pageOven[4] = new Text(Vector2(20, 27), 1, "", "temp_P4");
    pageOven[5] = new Text(Vector2(50, 51), 1, "", "ip_adress_P4");
    _screen->addPage(6, pageOven);

    _screen->changePage(INIT_PAGE);
}

void MyOled::update() {
    _screen->update();
}

Screen* MyOled::getScreen() {
    return _screen;
}