//
// Created by pawel on 25.03.18.
//

#include "Worker.h"


Worker::Worker(int workplace, Port work_port, Ship **ships, int port, bool mechanic) {
    efficiency = rand() % 5 + 1;
    this->workplace = workplace;
    this->port = work_port;
    this->ships = ships;
    if(mechanic)
        t = std::thread(&Worker::repair, this);
    else
    if(port == 1)
        t = std::thread(&Worker::load, this);
    else
        t = std::thread(&Worker::unload, this);
}

Worker::~Worker() {
    t.join();
}

void Worker::unload() {
    while (!(quitStroke)) {
        int shipId = port.shipInDock(workplace);
        if (shipId != -1) {
            Ship *ship = ships[shipId];
            ship->unload(efficiency);
        }
    }

}


void Worker::repair() {
    while (!(quitStroke)) {
        int shipId = port.shipInDock(workplace);
        if (shipId != -1) {
            Ship *ship = ships[shipId];
            ship->repair(efficiency);
        }
    }
}

void Worker::load() {

    while (!quitStroke) {
        int shipId = port.shipInDock(workplace);
        if (shipId != -1) {
            Ship *ship = ships[shipId];
            ship->load(efficiency);
        }
    }
}
