#pragma once

#include "map.h"
#include "location.h"
#include <ctime>
#include <QDebug>

class ScreenMap
{
    int height;
    int width;
    QVector<QVector<cell>> fieldMater;
    QVector<QVector<int>> seenTime;

public:
    ScreenMap(int = 140, int = 90);

    void mutalUpdate(ScreenMap *);
    void lookAround(map*,location, int);
    cell getCell(int y, int x);
};


