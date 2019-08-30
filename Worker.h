//
// Created by pawel on 25.03.18.
//

#ifndef SO2ZAD1_WORKER_H
#define SO2ZAD1_WORKER_H

#include <thread>
#include "Port.h"
#include "Ship.h"

class Worker {
    int workplace;
    Port port;
    int efficiency;
    Ship **ships;
    std::thread t;

public:
    Worker(int, Port, Ship **,int,bool);

    ~Worker();

    void unload();

    void load();

    void repair();
};


#endif //SO2ZAD1_WORKER_H
