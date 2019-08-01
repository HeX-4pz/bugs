#pragma once

#include <QtMath>


enum direction {
    NW,
    NE,
    E,
    SE,
    SW,
    W
};

class location {



public:
    location( int j = 0, int i = 0);

    double X;
    double Y;

    int i;
    int j;

    double getX();
    double getY();

    int getI();
    int getJ();

    double range(location destination);
    double angle(location destination);
    void step(int direction);

    void operator=(location);
    bool operator==(location) const;
    bool operator!=(location) const;
};

