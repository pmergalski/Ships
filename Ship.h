//
// Created by pawel on 24.03.18.
//

#ifndef SO2ZAD1_SHIP_H
#define SO2ZAD1_SHIP_H

#include <thread>

#include "Port.h"


class Ship {
    static int id_counter;
    static const int DISTANCE_TO_DESTINATION;
    int id;
    std::thread t;
    Port destination;
    bool inPort;
    int cargo;
    int state;
    int distance;
    std::condition_variable *cvCargo;
    std::mutex *cargoMtx;
    Port second_port;
    bool even;


public:

    bool sank;

    Ship();

    Ship(Port port, Port, std::mutex *, std::condition_variable *);

    ~Ship();

    void sailToPort();

    bool isInPort();

    void unload(int);

    void repair(int);

    void load(int);

    int getCargo();

    int getState();

    int getDistance();

    int getDestination();
};


#endif //SO2ZAD1_SHIP_H
