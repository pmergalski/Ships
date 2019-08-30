//
// Created by pawel on 24.03.18.
//

#include "Port.h"


Port::Port() = default;


Port::Port(int places, std::mutex *portMutex, std::condition_variable *cvDock) {
    this->places = places;
    docks = new int[places];

    portMtx = portMutex;
    this->cvDock = cvDock;
    for (int i = 0; i < places; i++)
        docks[i] = -1;
}


Port::~Port() = default;


bool Port::takeShip(int shipID) {

    for (int i = 0; i < places; i++) {
        if (docks[i] == -1) {
            docks[i] = shipID;
            return true;
        }
    }

    return false;
}


void Port::releaseShip(int shipID) {

    for (int i = 0; i < places; i++)
        if (docks[i] == shipID) {
            docks[i] = -1;
            break;
        }
}


int Port::shipInDock(int dock_number) {
    return docks[dock_number];
}

bool Port::empty() {
    for (int i = 0; i < places; i++)
        if (docks[i] == -1)
            return true;

    return false;
}
