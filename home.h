#pragma once

#include <QObject>
#include "map.h"


class Ant;
class AntWarrior;



class home : public QObject
{
    Q_OBJECT

public:
    QColor color;
    unsigned long long amount;
    QList<Ant *> ants;
    void setPopulation(int);
    void addAnt();
    void setFood(int);
    int getPartFood(int);
    map * world;
    home(location, map*);
    location place;
    location getLocation();

public slots:
    void oneAction();

signals:
    void antsOneAct();
};


