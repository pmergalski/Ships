#include <iostream>
#include <ncurses.h>
#include <thread>
#include <mutex>
#include <future>

#include "Ship.h"
#include "Worker.h"


const int NUMBER_OF_SHIPS = 8;
const int PLACES_IN_FIRST_PORT = 5;
const int PLACES_IN_SECOND_PORT = 3;
const int WORKERS_PER_DOCK_IN_FIRST_PORT = 2;
const int WORKERS_PER_DOCK_IN_SECOND_PORT = 3;
const int MECHANICS_PER_DOCK_IN_FIRST_PORT = 2;
const int MECHANICS_PER_DOCK_IN_SECOND_PORT = 3;

std::atomic<bool> quitStroke;

void draw(Ship **ships, Port first_port, Port second_port) {
    const int SAILING_SHIPS_COLUMN = 36;
    int waitingShipsRow = 2;
    const int FIRST_PORT_COLUMN = 0;
    const int SECOND_PORT_COLUMN = 81;
    int portRow = 2;
    const int FIRST_PORT_CARGO_COLUMN = FIRST_PORT_COLUMN+10;
    const int SECOND_PORT_CARGO_COLUMN = SECOND_PORT_COLUMN+10;
    const int FIRST_PORT_STATE_COLUMN = FIRST_PORT_CARGO_COLUMN+13;
    const int SECOND_PORT_STATE_COLUMN = SECOND_PORT_CARGO_COLUMN+13;

    int ship;

    initscr();
    curs_set(0);
    noecho();

    start_color();

    init_pair(1, COLOR_BLACK, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    assume_default_colors(COLOR_WHITE, COLOR_BLUE);

    while (!quitStroke) {
        mvprintw(1, SAILING_SHIPS_COLUMN, "<--------------SAILING  SHIPS-------------->");
        for (int i = 0; i < NUMBER_OF_SHIPS; i++) {
            if (!(ships[i]->isInPort()) && !(ships[i]->sank)) {
                if(ships[i]->getDestination()==1)
                    mvprintw(waitingShipsRow, SAILING_SHIPS_COLUMN + ships[i]->getDistance(), "  S%d  ", i);
                else
                    mvprintw(waitingShipsRow, SAILING_SHIPS_COLUMN + 40 - ships[i]->getDistance(), "  S%d  ", i);

            }
            else
                mvprintw(waitingShipsRow, SAILING_SHIPS_COLUMN, "                                        ");

            waitingShipsRow++;
        }
        waitingShipsRow = 2;


        mvprintw(1, FIRST_PORT_COLUMN, "<-PORT-A->");
        for (int i = 0; i < PLACES_IN_FIRST_PORT; i++) {
            mvprintw(portRow, FIRST_PORT_COLUMN+2, "========");
            portRow++;

            if ((ship = first_port.shipInDock(i)) == -1)
                mvprintw(portRow, FIRST_PORT_COLUMN + 4, "  X  ");
            else
                mvprintw(portRow, FIRST_PORT_COLUMN + 4, " S%d ", ship);

            portRow++;
        }

        mvprintw(portRow, FIRST_PORT_COLUMN+2, "========");
        portRow = 2;

        for (int i = 0; i < 2 * PLACES_IN_FIRST_PORT; i++) {
            mvprintw(portRow, FIRST_PORT_COLUMN + 9, "|");
            portRow++;
        }

        mvprintw(portRow, FIRST_PORT_COLUMN + 9, "|");
        portRow = 3;

        mvprintw(1, FIRST_PORT_CARGO_COLUMN, "<--CARGO-->");

        for (int i = 0; i < PLACES_IN_FIRST_PORT; i++) {
            int cargo;
            if ((ship = first_port.shipInDock(i)) == -1)
                cargo = 0;
            else
                cargo = ships[ship]->getCargo();
            move(portRow, FIRST_PORT_CARGO_COLUMN);
            attron (COLOR_PAIR(1));
            for (int j = 0; j < cargo; j += 10) printw(" ");
            attroff(COLOR_PAIR(1));

            attron (COLOR_PAIR(2));
            for (int j = cargo; j < 100; j += 10) printw(" ");
            attroff(COLOR_PAIR(2));
            mvprintw(portRow, FIRST_PORT_CARGO_COLUMN + 10, cargo>=10?"%d ":" %d ", cargo);
            portRow += 2;
        }

        portRow = 3;

        mvprintw(1, FIRST_PORT_STATE_COLUMN, "<--STATE-->");

        for (int i = 0; i < PLACES_IN_FIRST_PORT; i++) {
            int state;
            if ((ship = first_port.shipInDock(i)) == -1)
                state = 0;
            else
                state = ships[ship]->getState();
            move(portRow, FIRST_PORT_STATE_COLUMN);
            attron (COLOR_PAIR(1));
            for (int j = 0; j < state; j += 10) printw(" ");
            attroff(COLOR_PAIR(1));

            attron (COLOR_PAIR(2));
            for (int j = state; j < 100; j += 10) printw(" ");
            attroff(COLOR_PAIR(2));
            mvprintw(portRow, FIRST_PORT_STATE_COLUMN + 10, state>=10?"%d ":" %d ", state);
            portRow += 2;
        }


        portRow = 2;

        mvprintw(1, SECOND_PORT_COLUMN, "<-PORT-B->");

        for (int i = 0; i < PLACES_IN_SECOND_PORT; i++) {
            mvprintw(portRow, SECOND_PORT_COLUMN+2, "========");
            portRow++;

            if ((ship = second_port.shipInDock(i)) == -1)
                mvprintw(portRow, SECOND_PORT_COLUMN + 4, "  X  ");
            else
                mvprintw(portRow, SECOND_PORT_COLUMN + 4, " S%d ", ship);

            portRow++;
        }

        mvprintw(portRow, SECOND_PORT_COLUMN+2, "========");
        portRow = 2;

        for (int i = 0; i < 2 * PLACES_IN_SECOND_PORT; i++) {
            mvprintw(portRow, SECOND_PORT_COLUMN + 9, "|");
            portRow++;
        }

        mvprintw(portRow, SECOND_PORT_COLUMN + 9, "|");
        portRow = 3;

        mvprintw(1, SECOND_PORT_CARGO_COLUMN, "<--CARGO-->");

        for (int i = 0; i < PLACES_IN_SECOND_PORT; i++) {
            int cargo;
            if ((ship = second_port.shipInDock(i)) == -1)
                cargo = 0;
            else
                cargo = ships[ship]->getCargo();
            move(portRow, SECOND_PORT_CARGO_COLUMN);
            attron (COLOR_PAIR(1));
            for (int j = 0; j < cargo; j += 10) printw(" ");
            attroff(COLOR_PAIR(1));

            attron (COLOR_PAIR(2));
            for (int j = cargo; j < 100; j += 10) printw(" ");
            attroff(COLOR_PAIR(2));
            mvprintw(portRow, SECOND_PORT_CARGO_COLUMN + 10, cargo>=10?"%d ":" %d ", cargo);
            portRow += 2;
        }

        portRow = 3;

        mvprintw(1, SECOND_PORT_STATE_COLUMN, "<--STATE-->");

        for (int i = 0; i < PLACES_IN_SECOND_PORT; i++) {
            int state;
            if ((ship = second_port.shipInDock(i)) == -1)
                state = 0;
            else
                state = ships[ship]->getState();
            move(portRow, SECOND_PORT_STATE_COLUMN);
            attron (COLOR_PAIR(1));
            for (int j = 0; j < state; j += 10) printw(" ");
            attroff(COLOR_PAIR(1));

            attron (COLOR_PAIR(2));
            for (int j = state; j < 100; j += 10) printw(" ");
            attroff(COLOR_PAIR(2));
            mvprintw(portRow, SECOND_PORT_STATE_COLUMN + 10, state>=10?"%d ":" %d ", state);
            portRow += 2;

        }


        portRow = 2;

        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        clear();

    }

    endwin();
}

void keyboardListener() {
    while (!quitStroke) {
        if (getch() == 'q')
            quitStroke = true;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::mutex firstPortMtx;
    std::condition_variable cvDockFirstPort;
    Port firstPort(PLACES_IN_FIRST_PORT, &firstPortMtx, &cvDockFirstPort);

    std::mutex secondPortMtx;
    std::condition_variable cvDockSecondPort;
    Port secondPort(PLACES_IN_SECOND_PORT,&secondPortMtx, &cvDockSecondPort);

    auto **ships = new Ship *[NUMBER_OF_SHIPS];

    for (int i = 0; i < NUMBER_OF_SHIPS; i++) {
        ships[i] = new Ship(firstPort, secondPort, new std::mutex(), new std::condition_variable());
    }

    auto **first_port_workers = new Worker *[PLACES_IN_FIRST_PORT * WORKERS_PER_DOCK_IN_FIRST_PORT];

    for (int i = 0; i < PLACES_IN_FIRST_PORT; i++)
        for (int j = 0; j < WORKERS_PER_DOCK_IN_FIRST_PORT; j++)
            first_port_workers[i * WORKERS_PER_DOCK_IN_FIRST_PORT + j] = new Worker(i, firstPort, ships, 1, false);


    auto **second_port_workers = new Worker *[PLACES_IN_SECOND_PORT * WORKERS_PER_DOCK_IN_SECOND_PORT];

    for (int i = 0; i < PLACES_IN_SECOND_PORT; i++)
        for (int j = 0; j < WORKERS_PER_DOCK_IN_SECOND_PORT; j++)
            second_port_workers[i * WORKERS_PER_DOCK_IN_SECOND_PORT + j] = new Worker(i, secondPort, ships, 2, false);


    auto **first_port_mechanics = new Worker *[PLACES_IN_FIRST_PORT * MECHANICS_PER_DOCK_IN_FIRST_PORT];

    for (int i = 0; i < PLACES_IN_FIRST_PORT; i++)
        for (int j = 0; j < MECHANICS_PER_DOCK_IN_FIRST_PORT; j++)
            first_port_workers[i * MECHANICS_PER_DOCK_IN_FIRST_PORT + j] = new Worker(i, firstPort, ships, 1, true);


    auto **second_port_mechanics = new Worker *[PLACES_IN_SECOND_PORT * MECHANICS_PER_DOCK_IN_SECOND_PORT];

    for (int i = 0; i < PLACES_IN_SECOND_PORT; i++)
        for (int j = 0; j < MECHANICS_PER_DOCK_IN_SECOND_PORT; j++)
            second_port_workers[i * MECHANICS_PER_DOCK_IN_SECOND_PORT + j] = new Worker(i, secondPort, ships, 2, true);

    std::thread keyboardThread(keyboardListener);

    std::thread drawThread(draw, ships, firstPort, secondPort);

    keyboardThread.join();
    quitStroke = true;

    drawThread.join();


    for (int i = 0; i < PLACES_IN_FIRST_PORT * WORKERS_PER_DOCK_IN_FIRST_PORT; i++) {
        delete first_port_workers[i];

    }
    delete first_port_workers;

    for (int i = 0; i < PLACES_IN_SECOND_PORT * WORKERS_PER_DOCK_IN_SECOND_PORT; i++) {
        delete second_port_workers[i];

    }
    delete second_port_workers;


    for (int i = 0; i < PLACES_IN_FIRST_PORT * MECHANICS_PER_DOCK_IN_FIRST_PORT; i++) {
        delete first_port_mechanics[i];
    }
    delete first_port_mechanics;

    for (int i = 0; i < PLACES_IN_SECOND_PORT * MECHANICS_PER_DOCK_IN_SECOND_PORT; i++) {
        delete second_port_mechanics[i];
    }
    delete second_port_mechanics;


    for (int i = 0; i < NUMBER_OF_SHIPS; i++) {
        delete ships[i];
    }
    delete ships;
}
