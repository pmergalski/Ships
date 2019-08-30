//
// Created by pawel on 24.03.18.
//

#include "Ship.h"

#include <iostream>

int Ship::id_counter = 0;
const int Ship::DISTANCE_TO_DESTINATION = 40;

Ship::Ship() = default;


Ship::Ship(Port port, Port second_port, std::mutex *cargoMtx, std::condition_variable *cvCargo) {
    id = id_counter++;
    destination = port;
    cargo = 0;
    state = 100;
    distance = DISTANCE_TO_DESTINATION;
    even = true;
    sank = false;
    this->cargoMtx = cargoMtx;
    this->cvCargo = cvCargo;
    this->second_port = second_port;
    t = std::thread(&Ship::sailToPort, this);
}


Ship::~Ship() {
    t.join();
}


void Ship::sailToPort() {
    while (!(quitStroke) && !(sank)) {

        for (distance; distance > 0; distance--) {
            state -= rand() % 5;
            if (state <= 0) {
                sank = true;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(rand()%51 + 50));

        }
        if(sank)
            break;
        std::unique_lock<std::mutex> portLock(*destination.portMtx, std::adopt_lock);
        destination.cvDock->wait(portLock, [&]() { return (destination.empty() || (quitStroke)); });
        inPort = destination.takeShip(id);
        portLock.unlock();


        std::unique_lock<std::mutex> cargoLock(*cargoMtx, std::adopt_lock);

        if (even)
            cvCargo->wait(cargoLock, [&]() { return (cargo == 100 || (quitStroke)); });
        else
            cvCargo->wait(cargoLock, [&]() { return (cargo == 0 || (quitStroke)); });

        std::unique_lock<std::mutex> stateLock(*cargoMtx, std::adopt_lock);

        cvCargo->wait(stateLock, [&]() { return (state == 100 || (quitStroke)); });



        portLock.lock();
        destination.releaseShip(id);
        inPort = false;
        portLock.unlock();
        destination.cvDock->notify_one();


        distance = DISTANCE_TO_DESTINATION;
        Port tmp = destination;
        destination = second_port;
        second_port = tmp;
        even = !even;

    }

}


bool Ship::isInPort() {
    return inPort;
}

void Ship::unload(int load) {

    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1501 + 500));

    std::lock_guard<std::mutex> cargoLock(*cargoMtx);
    if (((cargo -= load) <= 0) || (quitStroke)) {
        cargo = 0;
        cvCargo->notify_one();
    }
}

void Ship::repair(int load) {

    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1501 + 500));

    std::lock_guard<std::mutex> cargoLock(*cargoMtx);

    if (((state += load) >= 100) || (quitStroke)) {
        state = 100;
        cvCargo->notify_one();
    }
}

void Ship::load(int load) {

    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1501 + 500));

    std::lock_guard<std::mutex> cargoLock(*cargoMtx);
    if (((cargo += load) >= 100) || (quitStroke)) {
        cargo = 100;
        cvCargo->notify_one();
    }
}

int Ship::getCargo() {
    return cargo;
}

int Ship::getState() {
    return state;
}

int Ship::getDistance() {
    return distance;
}

int Ship::getDestination() {
    if (even)
        return 1;
    else
        return 0;
}
