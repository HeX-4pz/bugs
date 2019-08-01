#pragma once



#include "bug.h"
#include <QPainter>


class Ant : public Bug
{
    Q_OBJECT


    int isLookingForFood = false;
    int food = grass;

    int cargo;
    int cargoLimit;


    void takeCargo();
    void putCargo();


protected:
    QQueue<location> nextSteps;

    int isReturning = false;
    virtual void eat();
    QQueue<location> returnToHome();
    void touchWorld();
    void detouchWorld();

public:


    Ant(QString, location);
    //virtual ~Ant(){};




    virtual void takeDamage(int);
    virtual void buildRoute(location destination);
    virtual void makeStep(int direction);
    virtual void action();
    virtual void drawYourself(QPainter * );
    QQueue<location> lookNearestFood();
    //virtual void makeMove();


public slots:
    virtual void oneAction();
};

