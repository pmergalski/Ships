//
// Created by pawel on 24.03.18.
//

#ifndef SO2ZAD1_PORT_H
#define SO2ZAD1_PORT_H


#include <mutex>
#include <condition_variable>
#include <atomic>

extern std::atomic<bool> quitStroke;
extern const int NUMBER_OF_SHIPS;

class Port {
    int places;
    int *docks;

public:
    std::mutex *portMtx;
    std::condition_variable *cvDock;

    Port();

    explicit Port(int places, std::mutex *, std::condition_variable *);

    ~Port();

    bool takeShip(int shipID);

    void releaseShip(int shipID);

    int shipInDock(int dock_number);

    bool empty();
};


#endif //SO2ZAD1_PORT_H
