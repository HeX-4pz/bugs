#pragma once

#include "ant.h"


class AntWarrior : public Ant
{
    Q_OBJECT

    int damage;
    int isStalkingEnemy = false;

    QQueue<location> lookForEnemy();
    Ant * identifyNearbyEnemy();
    void attack(Ant *);
    virtual void eat();

public:
    AntWarrior(QString, location);

    virtual void action();
    virtual void takeDamage(int);
    virtual void drawYourself(QPainter * );

public slots:
    virtual void oneAction();
};

