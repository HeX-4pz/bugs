#include "bug.h"

map* Bug::world = NULL;

Bug::Bug(QString name,  location locate) :
    name(name),
    loc(locate)
{
}

location Bug::getLocation()
{
    return loc;
}

int Bug::getEnergy()
{
    return energy;
}

void Bug::buildRoute(location destination)
{

}

void Bug::makeStep(int direction)
{

}

void Bug::action()
{

}



//void Bug::makeMove()
//{

//}


void Bug::setLocation(location nLocate)
{
    loc = nLocate;
}

int Bug::getViewRadius()
{
    return viewRadius;
}

void Bug::oneAction()
{

}

