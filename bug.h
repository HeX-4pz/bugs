#pragma once

#include <QObject>
#include "location.h"
#include <QQueue>
#include <QDebug>
#include "map.h"
#include "home.h"
#include <QSet>
#include <QMap>

class home;

class Bug : public QObject
{
    Q_OBJECT

protected:
    int HP;
    int limitHP;
    int energy;
    int energyCostsPerAction;
    int limitEnergy;
    QString name;
    location loc;
    int viewRadius = 2;


//ScreenMap observableWorld;


public:

    Bug(QString, location);
    //virtual ~Bug(){};
    QColor color;
    bool isAlive;
    static map* world;

    location getLocation();
    int getHP();
    int getEnergy();

    home * myHome;

    virtual void buildRoute(location destination);
    virtual void makeStep(int direction);
    virtual void action();


    void setLocation(location);
    int getViewRadius();

public slots:
    virtual void oneAction();



};

