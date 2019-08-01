#pragma once

#include <QObject>
#include <QtMath>
#include <QtWidgets>
#include <QDebug>
#include <QSet>
#include "location.h"
#include <QList>

class Ant;

struct cell {
    int type;
    int limit;
    int amount;
    int passability;
    int visible;
    QList<cell * > neighbors;
    location * place;
};

enum mater {
    unknown,
    ground,
    grass,
    flower,
    water,
};



void oneStep(int, int &, int &);

class map : public QObject
{
    Q_OBJECT

    int sizeX = 140;
    int sizeY = 90;
    qreal cellSize = 10;
    QVector<QVector<QPolygonF>> fieldGeometry;
    QVector<QVector<cell>> fieldMater;

    int grassLimit = 100;
    int tacktsPass = 0;

    void calcInitCoord();
    void fillWorldMater();
    void grassGenerate(int, int);
    void waterGenerate();
    void fillWaterEmptys();
    void grassGrowth();




public:
    map();

    void init();

    QVector<QVector<QSet<Ant *>>> fieldAnts;

    int pickUpPart(location, int);
    int getType(int, int);
    int getType(location);
    int getPossability(location);
    cell * getCellAdress(location);
    location * getLocationAdress(location);
    cell getCell(int y, int x);
    int getWidth();
    int getHeight();
    QPointF getPoint(int, int);
    QPointF getPoint(location *);
    QPolygonF getQPolygon(int, int);

    QSet<Ant *> getTouches(location *);

public slots:
    void tick();

};

